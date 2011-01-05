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

#ifndef __cspNativePackage_H__
#define __cspNativePackage_H__

#include "cspPrerequisites.h"
#include "cspVmCore.h"

namespace csp
{
	/** An abstract helper class for adding ActionScript 3 class packages to a VmCore */
	class NativePackageBase
	{
	public:
		NativePackageBase(VmCore* core, uint num_classes) 
			: mID(-1), 
			mCore(core), 
			mNumClasses(num_classes) { }

		virtual ~NativePackageBase() {}

		/** Get the PoolObject that contains the ActionScript 3 definitions */
		avmplus::PoolObject* getPool()
		{
			return mPool;
		}

		/** Abstract method to get the package name */
		const int& getID() const
		{
			return mID;
		}

		const uint& getNumClasses() const
		{
			return mNumClasses;
		}

	protected:
		int mID;
		VmCore* mCore;
		uint mNumClasses;
		avmplus::PoolObject* mPool;

		/** The abstract method that needs to do the binding work */
		virtual void initPool() = 0;
	};

	/** The macro that automatically implements the abstract NativePackageBase class */
#define NativePackage(core, native_ns, package) \
	class core##_Impl_##package : public csp::NativePackageBase \
	{ \
	public: \
		core##_Impl_##package(csp::VmCore* core) \
			: csp::NativePackageBase(core, native_ns::package##_abc_class_count) { initPool(); } \
	protected: \
		void initPool() \
		{ \
			MMGC_GCENTER(mCore->GetGC()); \
			mPool = native_ns::initBuiltinABC_##package(mCore, mCore->builtinDomain); \
			mID = mCore->addPackage(this); \
		} \
	} PKG_##core##_##package(core);
}

#endif
