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

#ifndef __cspVmCore_H__
#define __cspVmCore_H__

#include "cspPrerequisites.h"

namespace csp
{
	//-----------------------------------------------------------------------
	/** The class that wraps the ActionScript 3 Virtual Machine */
	class VmCore : public avmplus::AvmCore
	{
	private:
		typedef std::vector<NativePackageBase*> NativePackageList;
		typedef std::map<avmplus::Atom, uint> StickyRefMap;

		//-----------------------------------------------------------------------
		/** A small class to get access to a protected avmplus::Toplevel member function */
		class VmToplevel : public avmplus::Toplevel
		{
		public:
			VmToplevel(avmplus::AbcEnv* abc) : avmplus::Toplevel(abc) {}

			/** Get a ClassClosure that is contained in the given PoolObject */
			inline avmplus::ClassClosure* findClassInPool(int class_id, avmplus::PoolObject* pool)
			{ return avmplus::Toplevel::findClassInPool(class_id, pool); }
		};
		//-----------------------------------------------------------------------

	public:
		/** Usually you don't need to call this constructor manually, use the CSP_CREATE_VMCORE macro instead */
		VmCore(MMgc::GC* gc);

		/** Usually you don't need to call this destructor manually, use the CSP_DESTROY_VMCORE macro instead */
		virtual ~VmCore();

		///-----------------------------------------------------------------------
		/// Program / VmCore initialization
		///-----------------------------------------------------------------------

		/** Initialize the garbage collector heap */
		static void createGcHeap();

		/** Free the garbage collector heap */
		static void destroyGcHeap();

		/** Add an class package to this VmCore */
		int addPackage(NativePackageBase* package);

		/** Initialize all added class packages */
		bool initializePackages();

		///-----------------------------------------------------------------------
		/// Get ActionScript 3 script definitions (classes, functions, etc.)
		///-----------------------------------------------------------------------

		/** Get a multiname for the given identifier and package  */
		avmplus::Multiname getMultiname(avmplus::Stringp identifier, avmplus::Stringp package = NULL);

		/** Get a class closure from the given identifier and package  */
		avmplus::ClassClosure* getClassClosure(avmplus::Stringp identifier, avmplus::Stringp package = NULL);

		/** Get a class closure from the given identifier and package  */
		avmplus::ClassClosure* getClassClosure(const String& identifier, const String& package = "");

		/** Get a method definition via its name and the containing script object */
		static avmplus::MethodEnv* getMethodEnv(avmplus::ScriptObject* object, avmplus::Stringp method_name);

		/** Get a method definition via its name and the containing script object */
		static avmplus::MethodEnv* getMethodEnv(avmplus::ScriptObject* object, const String& method_name);

		///-----------------------------------------------------------------------
		/// Generic calling of ActionScript 3 functions (static class functions, object methods, etc.)
		///-----------------------------------------------------------------------

		/** Call a method of the given object via a previously obtained method definition
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		static avmplus::Atom callFunction(avmplus::ScriptObject* obj, avmplus::MethodEnv* method_env, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(static avmplus::Atom, callFunction, avmplus::ScriptObject*, avmplus::MethodEnv*);

		/** Call a method of the given object via its name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		static avmplus::Atom callFunction(avmplus::ScriptObject* obj, avmplus::Stringp function_name, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(static avmplus::Atom, callFunction, avmplus::ScriptObject*, avmplus::Stringp);

		/** Call a method of the given object via its name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		static avmplus::Atom callFunction(avmplus::ScriptObject* obj, const String& function_name, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(static avmplus::Atom, callFunction, avmplus::ScriptObject*, const String&);

		///-----------------------------------------------------------------------
		/// Calling global ActionScript 3 script functions
		///-----------------------------------------------------------------------

		/** Call a global function via a script definition
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::Atom callGlobalFunction(avmplus::ClassClosure* class_closure, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_1(avmplus::Atom, callGlobalFunction, avmplus::ClassClosure*);

		/** Call a global function via its name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::Atom callGlobalFunction(avmplus::Stringp function_name, avmplus::Stringp package = NULL, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(avmplus::Atom, callGlobalFunction, avmplus::Stringp, avmplus::Stringp);

		/** Call a global function via its name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::Atom callGlobalFunction(const String& function_name, const String& package = "", int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(avmplus::Atom, callGlobalFunction, const String&, const String&);

		///-----------------------------------------------------------------------
		/// Calling static ActionScript 3 class functions
		///-----------------------------------------------------------------------

		/** Call a static function of the given class via its name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::Atom callStaticFunction(avmplus::Stringp class_name, avmplus::Stringp function_name, avmplus::Stringp package = NULL, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_3(avmplus::Atom, callStaticFunction, avmplus::Stringp, avmplus::Stringp, avmplus::Stringp);

		/** Call a static function of the given class via its name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::Atom callStaticFunction(const String& class_name, const String& function_name, const String& package = "", int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_3(avmplus::Atom, callStaticFunction, const String&, const String&, const String&);

		///-----------------------------------------------------------------------
		/// Creating ActionScript 3 objects
		///-----------------------------------------------------------------------

		/** Create an object via a previously obtained class definition
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::ScriptObject* createObject(avmplus::ClassClosure* class_closure, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_1(avmplus::ScriptObject*, createObject, avmplus::ClassClosure*);

		/** Create an object via its class name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::ScriptObject* createObject(avmplus::Stringp class_name, avmplus::Stringp package = NULL, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(avmplus::ScriptObject*, createObject, avmplus::Stringp, avmplus::Stringp);

		/** Create an object via its class name
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::ScriptObject* createObject(const String& class_name, const String& package = "", int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(avmplus::ScriptObject*, createObject, const String&, const String&);

		/** Create an object via its class identifier and package identifier
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::ScriptObject* createNativeObject(const uint& native_class_id, const uint& package_id, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_2(avmplus::ScriptObject*, createNativeObject, const uint&, const uint&);

		/** Create a builtin ActionScript 3 object via its class identifier
		@warning If you pass an argument list to this function, keep the first entry of the array empty (i.e. "0") for internal usage */
		avmplus::ScriptObject* createBuiltinObject(const uint& builtin_class_id, int num_args = 0, avmplus::Atom* args = NULL);
		CSP_TEMPLATE_ARG_ARRAY_IMPL_1(avmplus::ScriptObject*, createBuiltinObject, const uint&);

		///-----------------------------------------------------------------------
		/// Managing ActionScript 3 object slots
		///-----------------------------------------------------------------------

		/** Initialize a named attribute of the given object with another object */
		static bool setSlotObject(avmplus::ScriptObject* obj, const String& slot_name, avmplus::ScriptObject* slot_obj);

		/** Get an object that is assigned to another object's child slot by its name */
		static avmplus::ScriptObject* getSlotObject(avmplus::ScriptObject* obj, const String& slot_name);

		/** Try to automatically initialize all child slots of the given object */
		static void initializeAllSlots(avmplus::ScriptObject* obj, const bool& recursive = false);

		/** Add a sticky reference to the given script object to keep it from being garbage collected */
		bool stickyObject(avmplus::Atom object);

		/** Remove the sticky reference to the given script object so it can be garbage collected again */
		bool unstickyObject(avmplus::Atom object);

		///-----------------------------------------------------------------------
		/// Convert C++ types to ActionScript 3 types
		///-----------------------------------------------------------------------

		/// base types

		/** Convert bool to an AS3 Boolean */
		inline const avmplus::Atom& toScript(const bool& value)
		{ CSP_ENTER_GC(); if(value) return avmplus::AtomConstants::trueAtom; else return avmplus::AtomConstants::falseAtom; }

		/** Convert int to an AS3 Integer */
		inline avmplus::Stringp toScriptPtr(const int& value)
		{ CSP_ENTER_GC(); return internInt(value); }
		CSP_CONVERT_TO_ATOM_IMPL(const int&);

		/** Convert double to an AS3 Number */
		inline avmplus::Stringp toScriptPtr(const double& value)
		{ CSP_ENTER_GC(); return internDouble(value); }
		CSP_CONVERT_TO_ATOM_IMPL(const double&);

		/// strings

		/** Convert a char* string to an AS3 String */
		inline avmplus::Stringp toScriptPtr(const char* value)
		{ CSP_ENTER_GC(); return internString(newStringLatin1(value)); }
		CSP_CONVERT_TO_ATOM_IMPL(const char*);

		/** Convert a std::string to an AS3 String */
		inline avmplus::Stringp toScriptPtr(const String& value)
		{ CSP_ENTER_GC(); return internString(newStringLatin1(value.c_str())); }
		CSP_CONVERT_TO_ATOM_IMPL(const String&);

		/// UTF strings

		/** Convert a wchar_t* string to an AS3 String */
		inline avmplus::Stringp toScriptPtr(const wchar_t* value)
		{ CSP_ENTER_GC(); return internStringUTF8((const char*)value); }
		CSP_CONVERT_TO_ATOM_IMPL(const wchar_t*);

		/** Convert a std::wstring to an AS3 UTF-String */
		inline avmplus::Stringp toScriptPtr(const WString& value)
		{ CSP_ENTER_GC(); return internStringUTF8((const char*)value.c_str()); }
		CSP_CONVERT_TO_ATOM_IMPL(const WString&);

		///-----------------------------------------------------------------------
		/// Convert ActionScript 3 types to C++ types
		///-----------------------------------------------------------------------

		/// base types

		/** Convert an AS3 Boolean to bool */
		inline bool atomToBool(const avmplus::Atom& atom)
		{ return (boolean(atom) != 0); }

		/// strings

		/** Convert an AS3 String to a std::string */
		String toString(avmplus::Stringp str);

		/// UTF strings

		/** Convert an AS3 UTF-String to a std::wstring */
		WString toUTFString(avmplus::Stringp str);

		///-----------------------------------------------------------------------
		/// ActionScript 3 code execution
		///-----------------------------------------------------------------------

		/** Execute the given ABC file */
		bool executeFile(const String& filename);

		/** Execute some ActionScript 3 byte code from the given memory buffer */
		bool executeByteCode(const char* buf, size_t len);

		/** Compile and execute the given ActionScript 3 code string (!!!EXPERIMENTAL!!!) */
		bool executeString(const String& code);

		///-----------------------------------------------------------------------
		/// avmplus Accessors
		///-----------------------------------------------------------------------

		/** Get the avmplus Toplevel that is associated with this VmCore */
		VmToplevel* getToplevel() const;

		/** Get the avmplus Domain that is associated with this VmCore */
		avmplus::Domain* getDomain() const;

		/** Get the avmplus DomainEnv that is associated with this VmCore */
		avmplus::DomainEnv* getDomainEnv() const;

		///-----------------------------------------------------------------------
		/// Add / remove output listeners
		///-----------------------------------------------------------------------

		/** Add an OutputListener to this virtual machine */
		bool addListener(OutputListener* listener);

		/** Remove an OutputListener from this virtual machine */
		bool removeListener(OutputListener* listener);

		///-----------------------------------------------------------------------
		/// Set / get custom data
		///-----------------------------------------------------------------------

		/** Assign a user specified object to this VmCore */
		void setUserData(void* data);

		/** Get the user specified object that has been assigned to this VmCore */
		void* getUserData() const;

		///-----------------------------------------------------------------------
		/// Print / throw ActionScript 3 exceptions
		///-----------------------------------------------------------------------

		/** Internal method for printing ActionScript 3 exceptions */
		void printException(avmplus::Exception* exception);

		/** Internal method for throwing ActionScript 3 exceptions */
		void throwException(const String& message);

	private:
		void* mUserData;
		OutputLogger* mOutputLogger;

		static MMgc::EnterFrame* mEF;

		VmToplevel* mToplevel;
		avmplus::Domain* mDomain;
		avmplus::DomainEnv* mDomainEnv;

		avmplus::ArrayObject* mStickyRefArray;
		StickyRefMap mStickyRefMap;

		int mNextPackageID;
		NativePackageList mPackages;

		// overridden
		avmplus::String* readFileForEval(avmplus::String* referencingFilename, avmplus::String* filename);
		void interrupt(avmplus::Toplevel *env, avmplus::AvmCore::InterruptReason reason);
		void stackOverflow(avmplus::Toplevel *env);

		inline int32_t getDefaultAPI() { return 0; }

#ifdef DEBUGGER
		/** Create a debugger object */
		virtual avmplus::Debugger* createDebugger(int tracelevel);

		/** Create a profiler object */
		virtual avmplus::Profiler* createProfiler();
#endif
	};
	//-----------------------------------------------------------------------
}

#endif
