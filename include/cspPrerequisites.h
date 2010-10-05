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

#ifndef __cspPrerequisites_H__
#define __cspPrerequisites_H__

#if !defined DEBUGGER && defined CSP_USE_RUNTIME_DEBUGGER
#	define DEBUGGER
#endif

#ifndef AVMSHELL_BUILD
#	define AVMSHELL_BUILD
#endif

#include "avmplus.h"

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// version
#define CASPIN_VERSION_MAJOR 0
#define CASPIN_VERSION_MINOR 0
#define CASPIN_VERSION_PATCH 1
#define CASPIN_VERSION_NAME ""

#define CASPIN_VERSION ((CASPIN_VERSION_MAJOR << 16) | (CASPIN_VERSION_MINOR << 8) | CASPIN_VERSION_PATCH)

#define CSP_CREATE_VMCORE(name) \
	{ \
	MMgc::GC* name_gc = mmfx_new(MMgc::GC(MMgc::GCHeap::GetGCHeap(), MMgc::GC::kIncrementalGC)); \
	MMGC_GCENTER(name_gc); \
	name = new csp::VmCore(name_gc); \
	}

#define CSP_DESTROY_VMCORE(name) \
	{ \
	MMgc::GC* name_gc = name->GetGC(); \
	MMGC_GCENTER(name_gc); \
	delete name; \
	mmfx_delete(name_gc); \
	}

#define CSP_CORE static_cast<csp::VmCore*>(core())
#define CSP_CORE_EX(name) static_cast<csp::VmCore*>(name)

#define CSP_CLASS_SLOTS(name) DECLARE_SLOTS_##name
#define CSP_INST_SLOTS(name) DECLARE_SLOTS_##name

/** A macro for adding the required class methods to an ActionScript 3 native class definition */
#define CSP_CLASS_CREATE_INSTANCE(clazz, inst) \
avmplus::ScriptObject* createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype) \
{ \
	return new (core()->GetGC(), ivtable->getExtraSize()) inst(ivtable, prototype); \
}

/** A macro for creating a native ActionScript 3 class definition */
#define CSP_DEFINE_CLASS(clazz, inst) \
class clazz : public avmplus::ClassClosure \
{ \
public: \
	clazz(avmplus::VTable* cvtable) : avmplus::ClassClosure(cvtable) \
	{ \
		AvmAssert(traits()->getSizeOfInstance() == sizeof(inst)); \
		createVanillaPrototype(); \
	} \
	\
	CSP_CLASS_CREATE_INSTANCE(clazz, inst); \
	\
	CSP_CLASS_SLOTS(clazz); \
}

namespace MMgc
{
	class GC;
	class GCAutoEnter;
	class EnterFrame;
}

namespace avmplus
{
	class Domain;
	class DomainEnv;
	class PoolObject;
	class Toplevel;
}

/** The root namespace of the caspin library */
namespace csp
{
	class NativePackageBase;
	class OutputListener;
	class OutputLogger;
	class ScriptDefinition;
	class VmCore;

	typedef unsigned int uint;
	typedef std::string String;
	typedef std::wstring WString;
	typedef std::vector<String> StringList;
	typedef std::vector<avmplus::Atom> ArgumentList;
}

#endif
