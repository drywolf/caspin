/*
-----------------------------------------------------------------------------
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

The Original Code is [caspin - AVM2+ Wrapper Library].

The Initial Developer of the Original Code is Fuse-Software (tm).
Portions created by the Initial Developer are Copyright (C) 2009-2010
the Initial Developer. All Rights Reserved.

Contributor(s):
caspin Developer Team

Alternatively, the contents of this file may be used under the terms of
either the GNU General Public License Version 2 or later (the "GPL"), or
the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
in which case the provisions of the GPL or the LGPL are applicable instead
of those above. If you wish to allow use of your version of this file only
under the terms of either the GPL or the LGPL, and not to allow others to
use your version of this file under the terms of the MPL, indicate your
decision by deleting the provisions above and replace them with the notice
and other provisions required by the GPL or the LGPL. If you do not delete
the provisions above, a recipient may use your version of this file under
the terms of any one of the MPL, the GPL or the LGPL.
-----------------------------------------------------------------------------
*/

#include "cspVmCore.h"
#include "cspNativePackage.h"
#include "cspOutputLogger.h"
#include "cspScriptDefinition.h"

#include "FileInputStream.h"

#ifdef WIN32
#	include "cspWinPlatform.h"
#else
#	include "cspUnixPlatform.h"
#endif

//-----------------------------------------------------------------------
namespace avmshell
{
#ifdef WIN32
	WinPlatform gPlatformHandle;
#else
	char* dummy;
	UnixPlatform gPlatformHandle(&dummy);
#endif

	Platform* avmshell::Platform::GetInstance()
	{
		AvmAssert(&gPlatformHandle != NULL);
		return &gPlatformHandle;
	}
}
//-----------------------------------------------------------------------
namespace csp
{
	//-----------------------------------------------------------------------
	MMgc::EnterFrame* VmCore::mEF = NULL;
	//-----------------------------------------------------------------------
	VmCore::VmCore(MMgc::GC* gc) 
		: avmplus::AvmCore(gc), 
		mUserData(NULL), 
		mNextPackageID(0)
	{
		mOutputLogger = new (gc) OutputLogger;
		setConsoleStream(mOutputLogger);

		mArgumentArray = new Atom[128];

		const char* uris[] = {"",};
		const int32_t api_compat [] = {0x1,};
		setAPIInfo(0, 1, 0, uris, api_compat);

		setStackLimit(avmshell::Platform::GetInstance()->getMainThreadStackLimit());

		AvmCore::CacheSizes sizes;
		setCacheSizes(sizes);

#ifdef DEBUGGER
		initBuiltinPool(avmplus::Debugger::TRACE_OFF);
#else
		initBuiltinPool();
#endif
	}
	//-----------------------------------------------------------------------
	VmCore::~VmCore()
	{
		MMGC_GCENTER(GetGC());
		delete[] mArgumentArray;
		delete mOutputLogger;
	}
	//-----------------------------------------------------------------------
	void VmCore::createGcHeap()
	{
		if(!mEF)
		{
			MMgc::GCHeap::EnterLockInit();
			MMgc::GCHeapConfig conf;
			MMgc::GCHeap::Init(conf);

			if(MMgc::GCHeap::ShouldNotEnter())
				return;

			mEF = new MMgc::EnterFrame;
			mEF->status = setjmp(mEF->jmpbuf);

			if(mEF->status != 0)
				return;
		}
	}
	//-----------------------------------------------------------------------
	void VmCore::destroyGcHeap()
	{
		if(mEF)
		{
			delete mEF;
			MMgc::GCHeap::Destroy();
			MMgc::GCHeap::EnterLockDestroy();
		}
	}
	//-----------------------------------------------------------------------
	int VmCore::addPackage(NativePackageBase* package)
	{
		mPackages.push_back(package);
		return mNextPackageID++;
	}
	//-----------------------------------------------------------------------
	bool VmCore::initializePackages()
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			mToplevel = initToplevel();

			const BugCompatibility* bugCompatibility = mToplevel->abcEnv()->codeContext()->bugCompatibility();
			mCodeContext = new(GetGC()) CodeContext(mToplevel->domainEnv(), bugCompatibility);

			NativePackageList::iterator it = mPackages.begin();
			for( ; it != mPackages.end(); ++it)
			{
				handleActionPool((*it)->getPool(), mToplevel, mCodeContext);
			}

			// Create a new Domain for the user code
			mDomain = Domain::newDomain(this, mToplevel->domainEnv()->domain());

			// Return a new DomainEnv for the user code
			mDomainEnv = DomainEnv::newDomainEnv(this, mDomain, mToplevel->domainEnv());

			// an internal array to store stickied AS3 objects
			mStickyRefArray = static_cast<ArrayObject*>(createObject(avmplus::NativeID::abcclass_Array));

			return true;
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			return false;
		}
		END_CATCH;
		END_TRY;

		return false;
	}
	//-----------------------------------------------------------------------
	ScriptDefinition VmCore::getDefinition(avmplus::Stringp identifier, avmplus::Stringp package)
	{
		MMGC_GCENTER(GetGC());

		ScriptDefinition result(this, identifier, package);

		TRY(this, kCatchAction_ReportAsError)
		{
			avmplus::ScriptEnv* script = domainMgr()->findScriptEnvInDomainEnvByMultiname(mDomainEnv, result.multi_name);

			if(script == (ScriptEnv*)BIND_AMBIGUOUS)
			{
				mToplevel->throwReferenceError(kAmbiguousBindingError, result.multi_name);
			}

			if(script == (ScriptEnv*)BIND_NONE)
			{
				mToplevel->throwReferenceError(kUndefinedVarError, result.multi_name);
			}

			if(script == NULL)
			{
				std::stringstream str;
				str << "Unable to find script definition \"" << identifier << "\"";
				throwException(str.str());
			}

			if(script->global == NULL)
			{
				script->initGlobal();
				script->coerceEnter(script->global->atom());
			}

			result.as3_env = script;
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return result;
	}
	//-----------------------------------------------------------------------
	ScriptDefinition VmCore::getDefinition(const String& identifier, const String& package)
	{
		MMGC_GCENTER(GetGC());

		Stringp id = newString(identifier.c_str());
		Stringp pckg = newString(package.c_str());

		return getDefinition(id, pckg);
	}
	//-----------------------------------------------------------------------
	ScriptDefinition VmCore::getMethodDefinition(avmplus::ScriptObject* object, avmplus::Stringp method_name)
	{
		VmCore* core = CSP_CORE_EX(object->core());
		GC* gc = core->GetGC();

		MMGC_GCENTER(gc);

		ScriptDefinition result;

		TRY(core, kCatchAction_ReportAsError)
		{
			// search for the method binding at the object
			avmplus::Binding method_bind = object->traits()->getTraitsBindings()->findBinding(method_name);

			if(avmplus::AvmCore::isMethodBinding(method_bind))
			{
				// found it, get the MethodEnv for the method
				result.method_env = object->vtable->methods[avmplus::AvmCore::bindingToMethodId(method_bind)];
			}
			else
			{
				core->throwException("Invalid method binding at VmCore::getMethodDefinition(...)");
			}
		}
		CATCH(Exception* exception)
		{
			core->printException(exception);
		}
		END_CATCH;
		END_TRY;

		return result;
	}
	//-----------------------------------------------------------------------
	ScriptDefinition VmCore::getMethodDefinition(avmplus::ScriptObject* object, const String& method_name)
	{
		MMGC_GCENTER(object->gc());

		VmCore* core = CSP_CORE_EX(object->core());
		Stringp mthd_nm = core->newString(method_name.c_str());

		return getMethodDefinition(object, mthd_nm);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callGlobalFunction(const ScriptDefinition& definition, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		avmplus::Atom result = nullObjectAtom;

		TRY(this, kCatchAction_ReportAsError)
		{
			if(definition.as3_env == NULL)
			{
				throwException("Invalid script definition for global function");
			}

			ScriptEnv* script = definition.as3_env;
			Atom atom = mToplevel->getproperty(script->global->atom(), &definition.multi_name, script->global->vtable);
			ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(atom);

			mArgumentArray[0] = class_closure->atom();

			for(size_t i=0; i<args.size(); ++i)
			{
				mArgumentArray[i+1] = args.at(i);
			}

			result = class_closure->call(args.size(), mArgumentArray);
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return result;
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callGlobalFunction(avmplus::Stringp function_name, avmplus::Stringp package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		ScriptDefinition def = getDefinition(function_name, package);
		return callGlobalFunction(def, args);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callGlobalFunction(const String& function_name, const String& package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		Stringp fn_name = newString(function_name.c_str());
		Stringp pckg = newString(package.c_str());

		return callGlobalFunction(fn_name, pckg, args);
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callStaticFunction(avmplus::Stringp class_name, avmplus::Stringp function_name, avmplus::Stringp package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		avmplus::Atom result = nullObjectAtom;

		TRY(this, kCatchAction_ReportAsError)
		{
			ScriptDefinition def = getDefinition(class_name, package);

			if(def.as3_env == NULL)
			{
				throwException("Unable to find class");
			}

			ScriptEnv* script = def.as3_env;
			Atom atom = mToplevel->getproperty(script->global->atom(), &def.multi_name, script->global->vtable);
			ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(atom);

			Binding binding = class_closure->traits()->getTraitsBindings()->findBinding(function_name);

			if(AvmCore::isMethodBinding(binding))
			{
				MethodEnv* method = class_closure->vtable->methods[AvmCore::bindingToMethodId(binding)];

				mArgumentArray[0] = class_closure->atom();

				for(size_t i=0; i<args.size(); ++i)
				{
					mArgumentArray[i+1] = args.at(i);
				}

				result = method->coerceEnter(args.size(), mArgumentArray);
			}
			else
			{
				throwException("Unable to find static function at class");
			}
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return result;
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callStaticFunction(const String& class_name, const String& function_name, const String& package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		Stringp cls_nm = newString(class_name.c_str());
		Stringp fn_nm = newString(function_name.c_str());
		Stringp pckg = newString(package.c_str());

		return callStaticFunction(cls_nm, fn_nm, pckg, args);
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(const ScriptDefinition& definition, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			ScriptEnv* script = definition.as3_env;
			Atom global_atom = mToplevel->getproperty(script->global->atom(), &definition.multi_name, script->global->vtable);
			ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(global_atom);

			return constructObject(class_closure, args);
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return NULL;
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(Stringp class_name, Stringp package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		ScriptDefinition def = getDefinition(class_name, package);
		return createObject(def, args);
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(const String& class_name, const String& package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		ScriptDefinition def = getDefinition(class_name, package);
		return createObject(def, args);
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(const uint& native_class_id, const uint& package_id, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			if(package_id > mPackages.size()-1)
			{
				throwException("Error at 'VmCore::createObject(...)': package_id out of bounds");
			}

			NativePackageBase* package = mPackages[package_id];

			if(native_class_id > package->getNumClasses()-1)
			{
				throwException("Error at 'VmCore::createObject(...)': native_class_id out of bounds");
			}

			ClassClosure* class_closure = mToplevel->findClassInPool(native_class_id, package->getPool());

			return constructObject(class_closure, args);
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return NULL;
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(const uint& toplevel_class_id, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			if(toplevel_class_id > avmplus::NativeID::builtin_abc_class_count-1)
			{
				throwException("Error at 'VmCore::createObject(...)': toplevel_class_id out of bounds");
				return NULL;
			}

			ClassClosure* class_closure = mToplevel->getBuiltinClass(toplevel_class_id);

			return constructObject(class_closure, args);
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return NULL;
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction2(avmplus::ScriptObject* obj, const ScriptDefinition& method_definition, Atom* args, uint num_args)
	{
		MMGC_GCENTER(obj->gc());

		avmplus::Atom result = nullObjectAtom;

		TRY(obj->core(), kCatchAction_ReportAsError)
		{
			MethodEnv* method = method_definition.method_env;

			if(method == NULL)
			{
				CSP_CORE_EX(obj->core())->throwException("Invalid method definition at VmCore::callObjectFunction(...)");
			}

			if(args == NULL)
			{
				Atom thys = obj->atom();
				result = method->coerceEnter(0, &thys);
			}
			else
				result = method->coerceEnter(num_args, args);
		}
		CATCH(avmplus::Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
		}
		END_CATCH;
		END_TRY;

		return result;
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction2(avmplus::ScriptObject* obj, avmplus::Stringp function_name, Atom* args, uint num_args)
	{
		MMGC_GCENTER(obj->gc());

		ScriptDefinition def = getMethodDefinition(obj, function_name);
		return callObjectFunction2(obj, def, args, num_args);
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction2(avmplus::ScriptObject* object, const String& function_name, Atom* args, uint num_args)
	{
		MMGC_GCENTER(object->gc());

		ScriptDefinition def = getMethodDefinition(object, function_name);
		return callObjectFunction2(object, def, args, num_args);
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction(avmplus::ScriptObject* obj, const ScriptDefinition& method_definition, ArgumentList args)
	{
		MMGC_GCENTER(obj->gc());

		Atom* atom_args = new Atom[args.size()+1];

		atom_args[0] = obj->atom();

		for(size_t i=0; i<args.size(); ++i)
		{
			atom_args[i+1] = args.at(i);
		}

		Atom result = callObjectFunction2(obj, method_definition, atom_args, args.size());

		delete[] atom_args;

		return result;
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction(avmplus::ScriptObject* obj, avmplus::Stringp function_name, ArgumentList args)
	{
		MMGC_GCENTER(obj->gc());

		ScriptDefinition def = getMethodDefinition(obj, function_name);
		return callObjectFunction(obj, def, args);
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction(avmplus::ScriptObject* object, const String& function_name, ArgumentList args)
	{
		MMGC_GCENTER(object->gc());

		ScriptDefinition def = getMethodDefinition(object, function_name);
		return callObjectFunction(object, def, args);
	}
	//-----------------------------------------------------------------------
	bool VmCore::setSlotObject(avmplus::ScriptObject* obj, const String& slot_name, avmplus::ScriptObject* slot_obj)
	{
		MMGC_GCENTER(obj->gc());

		TRY(obj->core(), kCatchAction_ReportAsError)
		{
			avmplus::Stringp slot = CSP_CORE_EX(obj->core())->newString(slot_name.c_str());

			// search for the slot at the object
			avmplus::Binding slot_bind = obj->traits()->getTraitsBindings()->findBinding(slot);
			if(avmplus::AvmCore::isSlotBinding(slot_bind))
			{
				// found it, now set the slot to contain our given object
				obj->coerceAndSetSlotAtom(avmplus::AvmCore::bindingToSlotId(slot_bind), slot_obj->atom());
			}
		}
		CATCH(avmplus::Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
			return false;
		}
		END_CATCH;
		END_TRY;

		return true;
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::getSlotObject(avmplus::ScriptObject* obj, const String& slot_name)
	{
		MMGC_GCENTER(obj->gc());

		TRY(obj->core(), kCatchAction_ReportAsError)
		{
			avmplus::Stringp slot = CSP_CORE_EX(obj->core())->newString(slot_name.c_str());

			// search for the slot at the object
			avmplus::Binding slot_bind = obj->traits()->getTraitsBindings()->findBinding(slot);
			if(avmplus::AvmCore::isSlotBinding(slot_bind))
			{
				// found it, now get the slot to obtain the object
				return obj->getSlotObject(avmplus::AvmCore::bindingToSlotId(slot_bind));
			}
		}
		CATCH(avmplus::Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
			return NULL;
		}
		END_CATCH;
		END_TRY;

		return NULL;
	}
	//-----------------------------------------------------------------------
	void VmCore::initializeAllSlots(avmplus::ScriptObject* obj, const bool& recursive)
	{
		MMGC_GCENTER(obj->gc());

		VmCore* core = CSP_CORE_EX(obj->core());

		TRY(core, kCatchAction_ReportAsError)
		{
			size_t slot_count = obj->traits()->getTraitsBindings()->slotCount;

			for(size_t i=0; i<slot_count; ++i)
			{
				Traitsp slot_traits = obj->traits()->getTraitsBindings()->getSlotTraits(i);

				if(!(slot_traits->builtinType & BUILTIN_object) || slot_traits->isInterface() || slot_traits->isNumeric())
				{
					continue;
				}

				avmplus::ScriptObject* slot_obj = core->createObject(slot_traits->name(), slot_traits->ns()->getURI());

				if(recursive)
				{
					initializeAllSlots(slot_obj);
				}

				obj->coerceAndSetSlotAtom(i, slot_obj->atom());
			}
		}
		CATCH(avmplus::Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
		}
		END_CATCH;
		END_TRY;
	}
	//-----------------------------------------------------------------------
	bool VmCore::stickyObject(avmplus::ScriptObject* object)
	{
		MMGC_GCENTER(GetGC());

		StickyRefMap::iterator it = mStickyRefMap.find(object);
		if(it == mStickyRefMap.end())
		{
			uint index = mStickyRefArray->get_length();
			mStickyRefArray->setUintProperty(index, object->atom());
			mStickyRefMap.insert(std::make_pair(object, index));
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool VmCore::unstickyObject(avmplus::ScriptObject* object)
	{
		MMGC_GCENTER(GetGC());

		StickyRefMap::iterator it = mStickyRefMap.find(object);
		if(it != mStickyRefMap.end())
		{
			mStickyRefArray->delUintProperty(it->second);
			mStickyRefMap.erase(it);
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	const Atom& VmCore::scriptBoolean(const bool& value) const
	{
		if(value)
		{
			return AtomConstants::trueAtom;
		}

		return AtomConstants::falseAtom;
	}
	//-----------------------------------------------------------------------
	Atom VmCore::scriptInteger(const int& value)
	{
		MMGC_GCENTER(GetGC());
		return internInt(value)->atom();
	}
	//-----------------------------------------------------------------------
	Atom VmCore::scriptNumber(const double& value)
	{
		MMGC_GCENTER(GetGC());
		return doubleToAtom(value);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::scriptString(const char* value)
	{
		MMGC_GCENTER(GetGC());
		return newString(value)->atom();
	}
	//-----------------------------------------------------------------------
	String VmCore::stringFromAS3(avmplus::Stringp str)
	{
		MMGC_GCENTER(GetGC());

		String result;
		unsigned int len = str->length();
		for(unsigned int i=0; i<len; ++i)
		{
			result.append(1, (char)str->charAt(i));
		}

		return result;
	}
	//-----------------------------------------------------------------------
	avmplus::Stringp VmCore::stringToAS3(const String& str)
	{
		MMGC_GCENTER(GetGC());
		return newString(str.c_str());
	}
	//-----------------------------------------------------------------------
	WString VmCore::utfStringFromAS3(avmplus::Stringp str)
	{
		MMGC_GCENTER(GetGC());

		WString result;
		unsigned int len = str->length();
		for(unsigned int i=0; i<len; ++i)
		{
			result.append(1, str->charAt(i));
		}

		return result;
	}
	//-----------------------------------------------------------------------
	avmplus::Stringp VmCore::utfStringToAS3(const WString& str)
	{
		MMGC_GCENTER(GetGC());
		return newStringUTF8(str.c_str());
	}
	//-----------------------------------------------------------------------
	bool VmCore::executeFile(const String& filename)
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			avmshell::FileInputStream f(filename.c_str());

			bool isValid = f.valid() && ((uint64_t)f.length() < 0xFFFFFFFF); //currently we cannot read files > 4GB
			if(!isValid)
			{
				String message = "cannot open file: \"" + filename + "\"";
				throwException(message);
				return false;
			}

			// parse new bytecode
			ScriptBuffer code = newScriptBuffer((size_t)f.available());
			f.read(code.getBuffer(), (size_t)f.available());

			if(AbcParser::canParse(code) == 0)
			{
				uint32_t api = getAPI(NULL);
				handleActionBlock(code, 0, mToplevel, NULL, mCodeContext, api);
			}
			else
			{
				throwException("Unable to parse file, file is no ABC file: " + filename);
				return false;
			}
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			return false;
		}
		END_CATCH;
		END_TRY;

		return true;
	}
	//-----------------------------------------------------------------------
	bool VmCore::executeByteCode(const char* buf, size_t len)
	{
		MMGC_GCENTER(GetGC());

		MMgc::GC* gc = mToplevel->gc();

		ReadOnlyScriptBufferImpl* abcbuf = new (gc) ReadOnlyScriptBufferImpl((uint8_t*)buf, len);
		ScriptBuffer code(abcbuf);

		TRY(this, kCatchAction_ReportAsError)
		{
			if(AbcParser::canParse(code) == 0)
			{
				uint32_t api = getAPI(NULL);
				handleActionBlock(code, 0, mToplevel, NULL, mCodeContext, api);
				return true;
			}
			else
			{
				return false;
			}
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			return false;
		}
		END_CATCH;
		END_TRY;

		return true;
	}
	//-----------------------------------------------------------------------
	bool VmCore::executeString(const String& code)
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			Stringp code_string = internStringUTF8(code.c_str(), code.length());
			code_string = code_string->appendLatin1("\0", 1);

			ScriptBuffer buffer = compileProgram(this, mToplevel, code_string, NULL);

			uint32_t api = getAPI(NULL);
			handleActionSource(code_string, NULL, mToplevel, NULL, mCodeContext, api);
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			return false;
		}
		END_CATCH;
		END_TRY;

		return true;
	}
	//-----------------------------------------------------------------------
	avmplus::Toplevel* VmCore::getToplevel() const
	{
		return mToplevel;
	}
	//-----------------------------------------------------------------------
	avmplus::Domain* VmCore::getDomain() const
	{
		return mDomain;
	}
	//-----------------------------------------------------------------------
	avmplus::DomainEnv* VmCore::getDomainEnv() const
	{
		return mDomainEnv;
	}
	//-----------------------------------------------------------------------
	bool VmCore::addListener(OutputListener* listener)
	{
		return mOutputLogger->addListener(listener);
	}
	//-----------------------------------------------------------------------
	bool VmCore::removeListener(OutputListener* listener)
	{
		return mOutputLogger->removeListener(listener);
	}
	//-----------------------------------------------------------------------
	void VmCore::setUserData(void* data)
	{
		mUserData = data;
	}
	//-----------------------------------------------------------------------
	void* VmCore::getUserData() const
	{
		return mUserData;
	}
	//-----------------------------------------------------------------------
	void VmCore::printException(Exception* exception)
	{
		MMGC_GCENTER(GetGC());

#ifdef DEBUGGER
		if(!(exception->flags & Exception::SEEN_BY_DEBUGGER))
		{
			console << string(exception->atom) << "\n";
		}
		if(exception->getStackTrace())
		{
			console << exception->getStackTrace()->format(this) << '\n';
		}
#else
		console << string(exception->atom) << "\n";
#endif
	}
	//-----------------------------------------------------------------------
	void VmCore::throwException(const String& message)
	{
		MMGC_GCENTER(GetGC());
		AvmCore::throwException(new (GetGC()) Exception(this, scriptString(message.c_str())));
	}
	//-----------------------------------------------------------------------
	Stringp VmCore::newString(const char* str)
	{
		MMGC_GCENTER(GetGC());
		return internString(newStringLatin1(str));
	}
	//-----------------------------------------------------------------------
	avmplus::Stringp VmCore::newStringUTF8(const wchar_t* str)
	{
		MMGC_GCENTER(GetGC());
		return internString(newStringUTF16((wchar*)str));
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::constructObject(avmplus::ClassClosure* class_closure, ArgumentList& args)
	{
		MMGC_GCENTER(GetGC());

		avmplus::ScriptObject* object = NULL;

		TRY(this, kCatchAction_ReportAsError)
		{
			mArgumentArray[0] = nullObjectAtom;

			for(size_t i=0; i<args.size(); ++i)
			{
				mArgumentArray[i+1] = args.at(i);
			}

			Atom object_atom = class_closure->construct(args.size(), mArgumentArray);

			object = AvmCore::atomToScriptObject(object_atom);

			// otherwise it would be immediately collected by the GC
			object->IncrementRef();
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH;
		END_TRY;

		return object;
	}
	//-----------------------------------------------------------------------
	avmplus::String* VmCore::readFileForEval(avmplus::String* referencingFilename, avmplus::String* filename)
	{
		String msg = "Warning: unimplemented method called: csp::VmCore::readFileForEval";
		std::cout << msg << std::endl;
		return newString(("trace(\"" + msg + "\");").c_str());
	}
	//-----------------------------------------------------------------------
	void VmCore::interrupt(avmplus::Toplevel *env, avmplus::AvmCore::InterruptReason reason)
	{
		throwException("Error: The ActionScript 3 virtual machine reported an interrupt (csp::VmCore::interrupt)");
	}
	//-----------------------------------------------------------------------
	void VmCore::stackOverflow(avmplus::Toplevel *env)
	{
		throwException("Error: The ActionScript 3 virtual machine reported a stack overflow (csp::VmCore::stackOverflow)");
	}
	//-----------------------------------------------------------------------
#ifdef DEBUGGER
	//-----------------------------------------------------------------------
	avmplus::Debugger* VmCore::createDebugger(int tracelevel)
	{
		return new (GetGC()) avmshell::DebugCLI(this, (avmplus::Debugger::TraceLevel)tracelevel);
	}
	//-----------------------------------------------------------------------
	avmplus::Profiler* VmCore::createProfiler()
	{
		return new (GetGC()) avmshell::Profiler(this);
	}
	//-----------------------------------------------------------------------
#endif
}
