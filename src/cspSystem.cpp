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

#include "cspSystem.h"

#include "caspin_base.cpp"

namespace csp
{
	//-----------------------------------------------------------------------
	SystemClass::SystemClass(avmplus::VTable *cvtable) 
		: avmplus::ClassClosure(cvtable)
	{
		createVanillaPrototype();
	}
	//-----------------------------------------------------------------------
	SystemClass::~SystemClass()
	{

	}
	//-----------------------------------------------------------------------
	void SystemClass::trace(avmplus::ArrayObject* a)
	{
		if(!a)
		{
			toplevel()->throwArgumentError(kNullArgumentError, "array");
		}

		avmplus::AvmCore* core = this->core();
		avmplus::PrintWriter& console = core->console;

		for(int i=0, n = a->getLength(); i < n; ++i)
		{
			if(i > 0)
			{
				console << ' ';
			}

			avmplus::StringIndexer s(core->string(a->getUintProperty(i)));
			for(int j = 0; j < s->length(); ++j)
			{
				wchar c = s[j];
				// '\r' gets converted into '\n'
				// '\n' is left alone
				// '\r\n' is left alone
				if(c == '\r')
				{
					if(((j+1) < s->length()) && s[j+1] == '\n')
					{
						console << '\r';
						++j;
					}

					console << '\n';
				}
				else
				{
					console << c;
				}
			}
		}

		console << '\n';
	}
	//-----------------------------------------------------------------------
}
