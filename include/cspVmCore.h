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
		typedef std::map<avmplus::ScriptObject*, uint> StickyRefMap;

		//-----------------------------------------------------------------------
		class CodeContext : public avmplus::CodeContext
		{
		public:
			inline CodeContext(avmplus::DomainEnv* env, const avmplus::BugCompatibility* bugCompatibility) 
				: avmplus::CodeContext(env, bugCompatibility) {}
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

		/** Get a script definition via its identifier and package name */
		ScriptDefinition getDefinition(avmplus::Stringp identifier, avmplus::Stringp package = NULL);

		/** Get a script definition via its identifier and package name */
		ScriptDefinition getDefinition(const String& identifier, const String& package = "");

		/** Get a method definition via its name and the containing script object */
		static ScriptDefinition getMethodDefinition(avmplus::ScriptObject* object, avmplus::Stringp method_name);

		/** Get a method definition via its name and the containing script object */
		static ScriptDefinition getMethodDefinition(avmplus::ScriptObject* object, const String& method_name);

		///-----------------------------------------------------------------------
		/// Calling global ActionScript 3 script functions
		///-----------------------------------------------------------------------

		/** Call a global function via a script definition */
		avmplus::Atom callGlobalFunction(const ScriptDefinition& definition, ArgumentList args);

		/** Call a global function via its name */
		avmplus::Atom callGlobalFunction(avmplus::Stringp function_name, avmplus::Stringp package = NULL, ArgumentList args = ArgumentList());

		/** Call a global function via its name */
		avmplus::Atom callGlobalFunction(const String& function_name, const String& package = "", ArgumentList args = ArgumentList());

		///-----------------------------------------------------------------------
		/// Calling static ActionScript 3 class functions
		///-----------------------------------------------------------------------

		/** Call a static function of the given class via its name */
		avmplus::Atom callStaticFunction(avmplus::Stringp class_name, avmplus::Stringp function_name, avmplus::Stringp package = NULL, ArgumentList args = ArgumentList());

		/** Call a static function of the given class via its name */
		avmplus::Atom callStaticFunction(const String& class_name, const String& function_name, const String& package = "", ArgumentList args = ArgumentList());

		///-----------------------------------------------------------------------
		/// Creating ActionScript 3 objects
		///-----------------------------------------------------------------------

		/** Create an object via a previously obtained class definition */
		avmplus::ScriptObject* createObject(const ScriptDefinition& definition, ArgumentList args = ArgumentList());

		/** Create an object via its class name */
		avmplus::ScriptObject* createObject(avmplus::Stringp class_name, avmplus::Stringp package = NULL, ArgumentList args = ArgumentList());

		/** Create an object via its class name */
		avmplus::ScriptObject* createObject(const String& class_name, const String& package = "", ArgumentList args = ArgumentList());

		/** Create an object via its class identifier and package identifier */
		avmplus::ScriptObject* createObject(const uint& native_class_id, const uint& package_id, ArgumentList args = ArgumentList());

		/** Create an ActionScript 3 toplevel object via its class identifier
		(see: http://livedocs.adobe.com/flash/9.0/ActionScriptLangRefV3/package-detail.html for a list of AS3 toplevel classes) */
		avmplus::ScriptObject* createObject(const uint& toplevel_class_id, ArgumentList args = ArgumentList());

		///-----------------------------------------------------------------------
		/// Calling ActionScript 3 object methods
		///-----------------------------------------------------------------------

		/** Call a method of the given object via a previously obtained method definition */
		static avmplus::Atom callObjectFunction2(avmplus::ScriptObject* obj, const ScriptDefinition& method_definition, avmplus::Atom* args, uint num_args = 0);

		/** Call a method of the given object via its name */
		static avmplus::Atom callObjectFunction2(avmplus::ScriptObject* obj, avmplus::Stringp function_name, avmplus::Atom* args, uint num_args = 0);

		/** Call a method of the given object via its name */
		static avmplus::Atom callObjectFunction2(avmplus::ScriptObject* obj, const String& function_name, avmplus::Atom* args, uint num_args = 0);

		/** Call a method of the given object via a previously obtained method definition */
		static avmplus::Atom callObjectFunction(avmplus::ScriptObject* obj, const ScriptDefinition& method_definition, ArgumentList args = ArgumentList());

		/** Call a method of the given object via its name */
		static avmplus::Atom callObjectFunction(avmplus::ScriptObject* obj, avmplus::Stringp function_name, ArgumentList args = ArgumentList());

		/** Call a method of the given object via its name */
		static avmplus::Atom callObjectFunction(avmplus::ScriptObject* obj, const String& function_name, ArgumentList args = ArgumentList());

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
		bool stickyObject(avmplus::ScriptObject* object);

		/** Remove the sticky reference to the given script object so it can be garbage collected again */
		bool unstickyObject(avmplus::ScriptObject* object);

		///-----------------------------------------------------------------------
		/// C++ / ActionScript 3 type conversions
		///-----------------------------------------------------------------------

		/** Create an ActionScript 3 boolean value */
		const avmplus::Atom& scriptBoolean(const bool& value) const;

		/** Create an ActionScript 3 integer value */
		avmplus::Atom scriptInteger(const int& value);

		/** Create an ActionScript 3 number value */
		avmplus::Atom scriptNumber(const double& value);

		/** Create an ActionScript 3 string object */
		avmplus::Atom scriptString(const char* value);

		/** Convert an ActionScript 3 string object to a C++ STL string */
		String stringFromAS3(avmplus::Stringp str);

		/** Convert a C++ STL string to an ActionScript 3 string object */
		avmplus::Stringp stringToAS3(const String& str);

		/** Convert an ActionScript 3 UTF string object to a C++ STL string */
		WString utfStringFromAS3(avmplus::Stringp str);

		/** Convert a C++ STL wide-string to an ActionScript 3 string object */
		avmplus::Stringp utfStringToAS3(const WString& str);

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
		avmplus::Toplevel* getToplevel() const;

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

		avmplus::Toplevel* mToplevel;
		avmplus::Domain* mDomain;
		avmplus::DomainEnv* mDomainEnv;
		CodeContext* mCodeContext;

		avmplus::Atom* mArgumentArray;

		avmplus::ArrayObject* mStickyRefArray;
		StickyRefMap mStickyRefMap;

		int mNextPackageID;
		NativePackageList mPackages;

		/** Internal helper method for creating string objects */
		avmplus::Stringp newString(const char* str);

		/** Internal helper method for creating string objects */
		avmplus::Stringp newStringUTF8(const wchar_t* str);

		/** Internal helper method for creating script objects */
		avmplus::ScriptObject* constructObject(avmplus::ClassClosure* class_closure, ArgumentList& args);

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
