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
		: AvmCore(gc), 
		mUserData(NULL), 
		mNextPackageID(0)
	{
		mOutputLogger = new (gc) OutputLogger;
		setConsoleStream(mOutputLogger);

		const char* uris[] = {"",};
		const int32_t api_compat [] = {0x1,};
		setAPIInfo(0, 1, 0, uris, api_compat);

		setStackLimit(avmshell::Platform::GetInstance()->getMainThreadStackLimit());

		AvmCore::CacheSizes sizes;
		setCacheSizes(sizes);

#ifdef DEBUGGER
		initBuiltinPool(Debugger::TRACE_OFF);
#else
		initBuiltinPool();
#endif
	}
	//-----------------------------------------------------------------------
	VmCore::~VmCore()
	{
		CSP_ENTER_GC();
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
		CSP_ENTER_GC();

		TRY(this, kCatchAction_ReportAsError)
		{
			mToplevel = (VmToplevel*)initTopLevel();

			NativePackageList::iterator it = mPackages.begin();
			for( ; it != mPackages.end(); ++it)
			{
				handleActionPool((*it)->getPool(), mToplevel->domainEnv(), (Toplevel*&)mToplevel, NULL);
			}

			// Create a new Domain for the user code
			mDomain = new (GetGC()) Domain(this, builtinDomain);

			// Return a new DomainEnv for the user code
			mDomainEnv = new (GetGC()) DomainEnv(this, mDomain, mToplevel->domainEnv());

			// an internal array to store stickied AS3 objects
			mStickyRefArray = static_cast<ArrayObject*>(createBuiltinObject(NativeID::abcclass_Array));

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
	Multiname VmCore::getMultiname(Stringp identifier, Stringp package)
	{
		CSP_ENTER_GC();

		Namespacep ns = mToplevel->getDefaultNamespace();

		if(package && package->length())
		{
			ns = internNamespace(newNamespace(package));
		}

		return Multiname(ns, identifier);
	}
	//-----------------------------------------------------------------------
	ClassClosure* VmCore::getClassClosure(Stringp identifier, Stringp package)
	{
		CSP_ENTER_GC();

		TRY(this, kCatchAction_ReportAsError)
		{
			Multiname mn = getMultiname(identifier, package);
			ScriptEnv* script = (ScriptEnv*)mDomainEnv->getScriptInit(mn);

			if(script == (ScriptEnv*)BIND_AMBIGUOUS)
			{
				mToplevel->throwReferenceError(kAmbiguousBindingError, mn);
			}

			if(script == NULL)
			{
				throwException("Unable to find script definition \"" + toString(identifier) + "\" [" + toString(package) + "]");
			}

			if(script->global == NULL)
			{
				script->initGlobal();
				script->coerceEnter(script->global->atom());
			}

			Atom atom = mToplevel->getproperty(script->global->atom(), &mn, script->global->vtable);
			return (ClassClosure*)AvmCore::atomToScriptObject(atom);
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
	ClassClosure* VmCore::getClassClosure(const String& identifier, const String& package)
	{
		CSP_ENTER_GC();

		Stringp id = toScriptPtr(identifier);
		Stringp pckg = toScriptPtr(package);

		return getClassClosure(id, pckg);
	}
	//-----------------------------------------------------------------------
	MethodEnv* VmCore::getMethodEnv(ScriptObject* object, Stringp method_name)
	{
		VmCore* core = CSP_CORE_EX(object->core());
		MMgc::GC* gc = core->GetGC();

		MMGC_GCENTER(gc);

		TRY(core, kCatchAction_ReportAsError)
		{
			// search for the method binding at the object
			Binding method_bind = object->traits()->getTraitsBindings()->findBinding(method_name);

			if(AvmCore::isMethodBinding(method_bind))
			{
				// found it, return the MethodEnv for the method
				return object->vtable->methods[AvmCore::bindingToMethodId(method_bind)];
			}
			else
			{
				core->throwException("Invalid method binding \"" + core->toString(method_name) + "\" at VmCore::getMethodEnv(...)");
			}
		}
		CATCH(Exception* exception)
		{
			core->printException(exception);
		}
		END_CATCH;
		END_TRY;

		return NULL;
	}
	//-----------------------------------------------------------------------
	MethodEnv* VmCore::getMethodEnv(ScriptObject* object, const String& method_name)
	{
		MMGC_GCENTER(object->gc());

		VmCore* core = CSP_CORE_EX(object->core());
		Stringp mthd_nm = core->toScriptPtr(method_name);

		return getMethodEnv(object, mthd_nm);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callFunction(ScriptObject* obj, MethodEnv* method_env, int num_args, Atom* args)
	{
		VmCore* core = static_cast<VmCore*>(method_env->core());

		MMGC_GCENTER(core->GetGC());

		Atom result = nullObjectAtom;

		if(method_env == NULL)
			core->throwException("Invalid method definition at VmCore::callFunction(...)");

		if(num_args == 0 || args == NULL)
		{
			Atom this_atom = obj->atom();
			result = method_env->coerceEnter(0, &this_atom);
		}
		else
		{
			args[0] = obj->atom();
			result = method_env->coerceEnter(num_args, args);
		}

		return result;
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callFunction(ScriptObject* obj, Stringp function_name, int num_args, Atom* args)
	{
		MMGC_GCENTER(obj->gc());

		MethodEnv* method_env = getMethodEnv(obj, function_name);

		if(method_env == NULL)
			return nullObjectAtom;

		return callFunction(obj, method_env, num_args, args);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callFunction(ScriptObject* obj, const String& function_name, int num_args, Atom* args)
	{
		MMGC_GCENTER(obj->gc());

		MethodEnv* method_env = getMethodEnv(obj, function_name);

		if(method_env == NULL)
			return nullObjectAtom;

		return callFunction(obj, method_env, num_args, args);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callGlobalFunction(ClassClosure* class_closure, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		Atom result = nullObjectAtom;

		if(class_closure == NULL)
			throwException("Invalid or empty class closure for global function call");

		if(num_args == 0 || args == NULL)
		{
			Atom clazz = class_closure->atom();
			result = class_closure->call(0, &clazz);
		}
		else
		{
			args[0] = class_closure->atom();
			result = class_closure->call(num_args, args);
		}

		return result;
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callGlobalFunction(Stringp function_name, Stringp package, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		ClassClosure* class_closure = getClassClosure(function_name, package);
		return callGlobalFunction(class_closure, num_args, args);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callGlobalFunction(const String& function_name, const String& package, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		ClassClosure* class_closure = getClassClosure(function_name, package);
		return callGlobalFunction(class_closure, num_args, args);
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callStaticFunction(Stringp class_name, Stringp function_name, Stringp package, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		Atom result = nullObjectAtom;

		ClassClosure* class_closure = getClassClosure(class_name, package);

		if(class_closure == NULL)
		{
			throwException("Unable to find class \"" + toString(class_name) + "\" [" + toString(package) + "]");
		}

		result = callFunction(class_closure, function_name, num_args, args);

		return result;
	}
	//-----------------------------------------------------------------------
	Atom VmCore::callStaticFunction(const String& class_name, const String& function_name, const String& package, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		Stringp cls_nm = toScriptPtr(class_name);
		Stringp fn_nm = toScriptPtr(function_name);
		Stringp pckg = toScriptPtr(package);

		return callStaticFunction(cls_nm, fn_nm, pckg, num_args, args);
	}
	//-----------------------------------------------------------------------
	ScriptObject* VmCore::createObject(ClassClosure* class_closure, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		if(!class_closure)
			throwException("Error at 'VmCore::createObject(...)': NULL ClassClosure given");

		Atom object_atom;

		if(num_args == 0 || args == NULL)
		{
			Atom null_atom = nullObjectAtom;
			object_atom = class_closure->construct(0, &null_atom);
		}
		else
		{
			args[0] = nullObjectAtom;
			object_atom = class_closure->construct(num_args, args);
		}

		ScriptObject* object = AvmCore::atomToScriptObject(object_atom);

		// otherwise it would be immediately collected by the GC
		object->IncrementRef();

		return object;
	}
	//-----------------------------------------------------------------------
	ScriptObject* VmCore::createObject(Stringp class_name, Stringp package, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		ClassClosure* class_closure = getClassClosure(class_name, package);
		return createObject(class_closure, num_args, args);
	}
	//-----------------------------------------------------------------------
	ScriptObject* VmCore::createObject(const String& class_name, const String& package, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		ClassClosure* class_closure = getClassClosure(class_name, package);
		return createObject(class_closure, num_args, args);
	}
	//-----------------------------------------------------------------------
	ScriptObject* VmCore::createNativeObject(const uint& native_class_id, const uint& package_id, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		if(package_id > mPackages.size()-1)
			throwException("Error at 'VmCore::createNativeObject(...)': package_id out of bounds");

		NativePackageBase* package = mPackages[package_id];

		if(native_class_id > package->getNumClasses()-1)
			throwException("Error at 'VmCore::createNativeObject(...)': native_class_id out of bounds");

		ClassClosure* class_closure = mToplevel->findClassInPool(native_class_id, package->getPool());

		return createObject(class_closure, num_args, args);
	}
	//-----------------------------------------------------------------------
	ScriptObject* VmCore::createBuiltinObject(const uint& builtin_class_id, int num_args, Atom* args)
	{
		CSP_ENTER_GC();

		if(builtin_class_id > NativeID::builtin_abc_class_count-1)
			throwException("Error at 'VmCore::createBuiltinObject(...)': builtin_class_id out of bounds");

		ClassClosure* class_closure = mToplevel->getBuiltinClass(builtin_class_id);

		return createObject(class_closure, num_args, args);
	}
	//-----------------------------------------------------------------------
	bool VmCore::setSlotObject(ScriptObject* obj, const String& slot_name, ScriptObject* slot_obj)
	{
		MMGC_GCENTER(obj->gc());

		TRY(obj->core(), kCatchAction_ReportAsError)
		{
			Stringp slot = CSP_CORE_EX(obj->core())->toScriptPtr(slot_name);

			// search for the slot at the object
			Binding slot_bind = obj->traits()->getTraitsBindings()->findBinding(slot);
			if(AvmCore::isSlotBinding(slot_bind))
			{
				// found it, now set the slot to contain our given object
				obj->coerceAndSetSlotAtom(AvmCore::bindingToSlotId(slot_bind), slot_obj->atom());
			}
		}
		CATCH(Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
			return false;
		}
		END_CATCH;
		END_TRY;

		return true;
	}
	//-----------------------------------------------------------------------
	ScriptObject* VmCore::getSlotObject(ScriptObject* obj, const String& slot_name)
	{
		MMGC_GCENTER(obj->gc());

		TRY(obj->core(), kCatchAction_ReportAsError)
		{
			Stringp slot = CSP_CORE_EX(obj->core())->toScriptPtr(slot_name);

			// search for the slot at the object
			Binding slot_bind = obj->traits()->getTraitsBindings()->findBinding(slot);
			if(AvmCore::isSlotBinding(slot_bind))
			{
				// found it, now get the slot to obtain the object
				return obj->getSlotObject(AvmCore::bindingToSlotId(slot_bind));
			}
		}
		CATCH(Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
			return NULL;
		}
		END_CATCH;
		END_TRY;

		return NULL;
	}
	//-----------------------------------------------------------------------
	void VmCore::initializeAllSlots(ScriptObject* obj, const bool& recursive)
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

				ScriptObject* slot_obj = core->createObject(slot_traits->name(), slot_traits->ns()->getURI());

				if(recursive)
				{
					initializeAllSlots(slot_obj);
				}

				obj->coerceAndSetSlotAtom(i, slot_obj->atom());
			}
		}
		CATCH(Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
		}
		END_CATCH;
		END_TRY;
	}
	//-----------------------------------------------------------------------
	bool VmCore::stickyObject(Atom object)
	{
		CSP_ENTER_GC();

		StickyRefMap::iterator it = mStickyRefMap.find(object);
		if(it == mStickyRefMap.end())
		{
			uint index = mStickyRefArray->get_length();
			mStickyRefArray->setUintProperty(index, object);
			mStickyRefMap.insert(std::make_pair(object, index));
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool VmCore::unstickyObject(Atom object)
	{
		CSP_ENTER_GC();

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
	String VmCore::toString(Stringp str)
	{
		CSP_ENTER_GC();

		String result;
		unsigned int len = str->length();
		for(unsigned int i=0; i<len; ++i)
		{
			result.append(1, (char)str->charAt(i));
		}

		return result;
	}
	//-----------------------------------------------------------------------
	WString VmCore::toUTFString(Stringp str)
	{
		CSP_ENTER_GC();

		WString result;
		unsigned int len = str->length();
		for(unsigned int i=0; i<len; ++i)
		{
			result.append(1, str->charAt(i));
		}

		return result;
	}
	//-----------------------------------------------------------------------
	bool VmCore::executeFile(const String& filename)
	{
		CSP_ENTER_GC();

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
				handleActionBlock(code, 0, mDomainEnv, (avmplus::Toplevel*&)mToplevel, NULL, codeContext(), api);
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
		CSP_ENTER_GC();

		MMgc::GC* gc = mToplevel->gc();

		ReadOnlyScriptBufferImpl* abcbuf = new (gc) ReadOnlyScriptBufferImpl((uint8_t*)buf, len);
		ScriptBuffer code(abcbuf);

		TRY(this, kCatchAction_ReportAsError)
		{
			if(AbcParser::canParse(code) == 0)
			{
				uint32_t api = getAPI(NULL);
				handleActionBlock(code, 0, mDomainEnv, (avmplus::Toplevel*&)mToplevel, NULL, codeContext(), api);
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
		CSP_ENTER_GC();

		TRY(this, kCatchAction_ReportAsError)
		{
			Stringp code_string = toScriptPtr(code);
			code_string = code_string->appendLatin1("\0", 1);

			ScriptBuffer buffer = compileProgram(this, mToplevel, code_string, NULL);

			uint32_t api = getAPI(NULL);
			handleActionSource(code_string, NULL, mDomainEnv, (avmplus::Toplevel*&)mToplevel, NULL, codeContext(), api);
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
	VmCore::VmToplevel* VmCore::getToplevel() const
	{
		return mToplevel;
	}
	//-----------------------------------------------------------------------
	Domain* VmCore::getDomain() const
	{
		return mDomain;
	}
	//-----------------------------------------------------------------------
	DomainEnv* VmCore::getDomainEnv() const
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
		CSP_ENTER_GC();

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
		CSP_ENTER_GC();
		AvmCore::throwException(new (GetGC()) Exception(this, toScript("csp-Error: " + message)));
	}
	//-----------------------------------------------------------------------
	Stringp VmCore::readFileForEval(Stringp referencingFilename, Stringp filename)
	{
		String msg = "Warning: unimplemented method called: csp::VmCore::readFileForEval";
		std::cout << msg << std::endl;
		return toScriptPtr(("trace(\"" + msg + "\");"));
	}
	//-----------------------------------------------------------------------
	void VmCore::interrupt(avmplus::Toplevel *env, AvmCore::InterruptReason reason)
	{
		throwException("The ActionScript 3 virtual machine reported an interrupt (csp::VmCore::interrupt)");
	}
	//-----------------------------------------------------------------------
	void VmCore::stackOverflow(avmplus::Toplevel *env)
	{
		throwException("The ActionScript 3 virtual machine reported a stack overflow (csp::VmCore::stackOverflow)");
	}
	//-----------------------------------------------------------------------
#ifdef DEBUGGER
	//-----------------------------------------------------------------------
	Debugger* VmCore::createDebugger(int tracelevel)
	{
		return new (GetGC()) avmshell::DebugCLI(this, (Debugger::TraceLevel)tracelevel);
	}
	//-----------------------------------------------------------------------
	Profiler* VmCore::createProfiler()
	{
		return new (GetGC()) avmshell::Profiler(this);
	}
	//-----------------------------------------------------------------------
#endif
}
