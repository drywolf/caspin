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
		: avmplus::AvmCore(gc), 
		mUserData(NULL)
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
		initBuiltinPool(avmplus::Debugger::TRACE_OFF);
#else
		initBuiltinPool();
#endif
	}
	//-----------------------------------------------------------------------
	VmCore::~VmCore()
	{
		MMGC_GCENTER(GetGC());
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
	void VmCore::addFunctions(NativePackageBase* package)
	{
		mPackages.push_back(package);
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

			NativePackageBaseList::iterator it = mPackages.begin();
			for( ; it != mPackages.end(); ++it)
			{
				handleActionPool((*it)->getPool(), mToplevel, mCodeContext);
			}

			// Create a new Domain for the user code
			mDomain = Domain::newDomain(this, mToplevel->domainEnv()->domain());

			// Return a new DomainEnv for the user code
			mDomainEnv = DomainEnv::newDomainEnv(this, mDomain, mToplevel->domainEnv());

			return true;
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			return false;
		}
		END_CATCH
		END_TRY

		return false;
	}
	//-----------------------------------------------------------------------
	ClassDef VmCore::getDefinition(avmplus::Stringp class_name, avmplus::Stringp package)
	{
		ClassDef result(this, class_name, package);

		TRY(this, kCatchAction_ReportAsError)
		{
			Namespacep ns = mToplevel->getDefaultNamespace();

			if(package)
			{
				if(package->length())
				{
					ns = internNamespace(newNamespace(package));
				}
			}

			Multiname mn(ns, class_name);

			ScriptEnv* script = domainMgr()->findScriptEnvInDomainEnvByMultiname(mDomainEnv, mn);

			if(script == (ScriptEnv*)BIND_AMBIGUOUS)
			{
				mToplevel->throwReferenceError(kAmbiguousBindingError, mn);
			}

			if(script == (ScriptEnv*)BIND_NONE)
			{
				mToplevel->throwReferenceError(kUndefinedVarError, mn);
			}

			if(script == NULL)
			{
				std::stringstream str;
				str << "Unable to find class \"" << class_name << "\"";
				throwException(str.str());
				return result;
			}

			if(script->global == NULL)
			{
				script->initGlobal();
				script->coerceEnter(script->global->atom());
			}

			result.scriptEnv = script;
		}
		CATCH(Exception* exception)
		{
			printException(exception);
		}
		END_CATCH
		END_TRY

		return result;
	}
	//-----------------------------------------------------------------------
	ClassDef VmCore::getDefinition(const String& class_name, const String& package)
	{
		MMGC_GCENTER(GetGC());

		Stringp cls_nm = newString(class_name.c_str());
		Stringp pckg = newString(package.c_str());

		return getDefinition(cls_nm, pckg);
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(const ClassDef& definition, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		avmplus::Atom* atom_args = new avmplus::Atom[args.size()+1];

		TRY(this, kCatchAction_ReportAsError)
		{
			ScriptEnv* script = definition.scriptEnv;
			Atom global_atom = mToplevel->getproperty(script->global->atom(), &definition.multiName, script->global->vtable);
			ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(global_atom);

			atom_args[0] = nullObjectAtom;

			for(size_t i=0; i<args.size(); ++i)
			{
				atom_args[i+1] = args.at(i);
			}

			Atom new_atom = class_closure->construct(args.size(), atom_args);

			delete[] atom_args;

			avmplus::ScriptObject* mObject = AvmCore::atomToScriptObject(new_atom);

			// otherwise it would be immediately collected by the GC
			mObject->IncrementRef();

			return mObject;
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			delete[] atom_args;
			return NULL;
		}
		END_CATCH
		END_TRY

		return NULL;
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(Stringp class_name, Stringp package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		avmplus::Atom* atom_args = new avmplus::Atom[args.size()+1];

		TRY(this, kCatchAction_ReportAsError)
		{
			ClassDef def = getDefinition(class_name, package);

			ScriptEnv* script = def.scriptEnv;
			Atom global_atom = mToplevel->getproperty(script->global->atom(), &def.multiName, script->global->vtable);
			ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(global_atom);

			atom_args[0] = nullObjectAtom;

			for(size_t i=0; i<args.size(); ++i)
			{
				atom_args[i+1] = args.at(i);
			}

			Atom new_atom = class_closure->construct(args.size(), atom_args);

			delete[] atom_args;

			avmplus::ScriptObject* mObject = AvmCore::atomToScriptObject(new_atom);

			// otherwise it would be immediately collected by the GC
			mObject->IncrementRef();

			return mObject;
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			delete[] atom_args;
			return NULL;
		}
		END_CATCH
		END_TRY

		return NULL;
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* VmCore::createObject(const String& class_name, const String& package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		Stringp cls_nm = newString(class_name.c_str());
		Stringp pckg = newString(package.c_str());

		return createObject(cls_nm, pckg, args);
	}
	//-----------------------------------------------------------------------
	avmplus::Atom VmCore::callObjectFunction(avmplus::ScriptObject* obj, const String& function_name, ArgumentList args)
	{
		MMGC_GCENTER(obj->gc());

		avmplus::Atom* atom_args = new avmplus::Atom[args.size()+1];

		TRY(obj->core(), kCatchAction_ReportAsError)
		{
			avmplus::Stringp func = CSP_CORE_EX(obj->core())->newString(function_name.c_str());

			// search for the function binding at the object
			avmplus::Binding func_bind = obj->traits()->getTraitsBindings()->findBinding(func);
			if(avmplus::AvmCore::isMethodBinding(func_bind))
			{
				// found it, get the MethodEnv for the function and call it
				avmplus::MethodEnv* method = obj->vtable->methods[avmplus::AvmCore::bindingToMethodId(func_bind)];

				atom_args[0] = obj->atom();

				for(size_t i=0; i<args.size(); ++i)
				{
					atom_args[i+1] = args.at(i);
				}

				avmplus::Atom result = method->coerceEnter(args.size(), atom_args);
				delete[] atom_args;
				return result;
			}
		}
		CATCH(avmplus::Exception* exception)
		{
			CSP_CORE_EX(obj->core())->printException(exception);
			delete[] atom_args;
		}
		END_CATCH
		END_TRY

		return nullObjectAtom;
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
		END_CATCH
		END_TRY

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
		END_CATCH
		END_TRY

		return NULL;
	}
	//-----------------------------------------------------------------------
	void VmCore::initializeAllSlots(avmplus::ScriptObject* obj, const bool& recursive)
	{
		MMGC_GCENTER(obj->gc());

		VmCore* core = CSP_CORE_EX(obj->core());

		TRY(core, kCatchAction_ReportAsError)
		{
			std::cout << "init slots for: " << core->stringFromAS3(obj->traits()->name()) << std::endl;
			size_t slot_count = obj->traits()->getTraitsBindings()->slotCount;

			for(size_t i=0; i<slot_count; ++i)
			{
				Traitsp slot_traits = obj->traits()->getTraitsBindings()->getSlotTraits(i);

				if(!(slot_traits->builtinType & BUILTIN_object) || slot_traits->isInterface() || slot_traits->isNumeric())
				{
					continue;
				}

				std::cout << (slot_traits->builtinType) << std::endl;
				std::cout << "DEBUG: " << core->stringFromAS3(slot_traits->name()) << " -> " << core->stringFromAS3(slot_traits->ns()->getURI()) << std::endl;
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
		END_CATCH
		END_TRY
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
	bool VmCore::parseFile(const String& filename)
	{
		MMGC_GCENTER(GetGC());

		TRY(this, kCatchAction_ReportAsError)
		{
			// TODO: only temporary placeholder, use own filestream instead ???
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
		END_CATCH
		END_TRY

		return true;
	}
	//-----------------------------------------------------------------------
	bool VmCore::executeByteCode(const char* buf, size_t len)
	{
		MMGC_GCENTER(GetGC());

		MMgc::GC* gc = mToplevel->gc();

		ReadOnlyScriptBufferImpl* abcbuf = new (gc) ReadOnlyScriptBufferImpl((avmplus::byte*)buf, len);
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
		END_CATCH
		END_TRY

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
		END_CATCH
		END_TRY

		return true;
	}
	//-----------------------------------------------------------------------
	bool VmCore::callGlobalFunction(const String& function_name, const String& package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		Atom* atom_args = new Atom[args.size()+1];

		TRY(this, kCatchAction_ReportAsError)
		{
			Stringp function_str = newString(function_name.c_str());
			Namespacep ns = mToplevel->getDefaultNamespace();

			if(package != "")
			{
				Stringp ns_str = newString(package.c_str());
				ns = internNamespace(newNamespace(ns_str));
			}

			Multiname mn(ns, function_str);
			ScriptEnv* script = domainMgr()->findScriptEnvInDomainEnvByMultiname(mDomainEnv, mn);

			if (script == NULL)
			{
				String message = "Unable to find function \"" + function_name + "\"";
				throwException(message);
			}
			else
			{
				if(script->global == NULL)
				{
					script->initGlobal();
					Atom argv[1] = { script->global->atom() };
					script->coerceEnter(0, argv);
				}

				Atom atom = mToplevel->getproperty(script->global->atom(), &mn, script->global->vtable);
				ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(atom);

				atom_args[0] = class_closure->atom();

				for(size_t i=0; i<args.size(); ++i)
				{
					atom_args[i+1] = args.at(i);
				}

				class_closure->call(args.size(), atom_args);
				delete[] atom_args;
			}
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			delete[] atom_args;
			return false;
		}
		END_CATCH
		END_TRY

		return true;
	}
	//-----------------------------------------------------------------------
	bool VmCore::callStaticFunction(const String& class_name, const String& function_name, const String& package, ArgumentList args)
	{
		MMGC_GCENTER(GetGC());

		Atom* atom_args = new Atom[args.size()+1];

		TRY(this, kCatchAction_ReportAsError)
		{
			Stringp clss_str = newString(class_name.c_str());
			Namespacep ns = mToplevel->getDefaultNamespace();

			if(package != "")
			{
				Stringp ns_str = newString(package.c_str());
				ns = internNamespace(newNamespace(ns_str));
			}

			Multiname mn(ns, clss_str);
			ScriptEnv* script = domainMgr()->findScriptEnvInDomainEnvByMultiname(mDomainEnv, mn);

			if (script == NULL)
			{
				String message = "Unable to find class \"" + class_name + "\"";
				throwException(message);
			}
			else
			{
				if(script->global == NULL)
				{
					script->initGlobal();
					Atom argv[1] = { script->global->atom() };
					script->coerceEnter(0, argv);
				}

				Atom atom = mToplevel->getproperty(script->global->atom(), &mn, script->global->vtable);
				ClassClosure* class_closure = (ClassClosure*)AvmCore::atomToScriptObject(atom);

				Stringp fnName = newString(function_name.c_str());
				Binding binding = class_closure->traits()->getTraitsBindings()->findBinding(fnName);

				if(AvmCore::isMethodBinding(binding))
				{
					MethodEnv* method = class_closure->vtable->methods[AvmCore::bindingToMethodId(binding)];

					atom_args[0] = class_closure->atom();

					for(size_t i=0; i<args.size(); ++i)
					{
						atom_args[i+1] = args.at(i);
					}

					method->coerceEnter(args.size(), atom_args);
					delete[] atom_args;
				}
				else
				{
					String message = 
						"Unable to find static function \"" + 
						function_name + 
						"\" inside class \"" + 
						class_name + "\"";

					throwException(message);
				}
			}
		}
		CATCH(Exception* exception)
		{
			printException(exception);
			delete[] atom_args;
			return false;
		}
		END_CATCH
		END_TRY

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
