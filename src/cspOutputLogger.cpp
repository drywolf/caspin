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

#include "cspOutputLogger.h"
#include "cspOutputListener.h"

#include "Platform.h"

namespace csp
{
	//-----------------------------------------------------------------------
	bool OutputLogger::addListener(OutputListener* listener)
	{
		ListenerMap::iterator it = mListeners.find(listener);
		if(it == mListeners.end())
		{
			mListeners.insert(std::make_pair(listener, listener));
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool OutputLogger::removeListener(OutputListener* listener)
	{
		ListenerMap::iterator it = mListeners.find(listener);
		if(it != mListeners.end())
		{
			mListeners.erase(it);
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	//int OutputLogger::write(const void *buffer, int count)
	//{
	//	const char* buf = (const char*)buffer;

	//	if(buf[count-1] == '\n')
	//	{
	//		if(mMessage.length())
	//		{
	//			printMessage(mMessage);
	//		}
	//		mMessage.clear();
	//	}
	//	else
	//	{
	//		mMessage.append(buf, count);
	//	}

	//	return count;
	//}
	//-----------------------------------------------------------------------
	void OutputLogger::write(const char* utf8)
	{
		const char* buf = (const char*)utf8;

		mMessage.append(buf);
		printMessage(mMessage);
		mMessage.clear();
	}
	//-----------------------------------------------------------------------
	void OutputLogger::writeN(const char* utf8, size_t count)
	{
		const char* buf = (const char*)utf8;

		if(buf[count-1] == '\n')
		{
			if(mMessage.length())
				printMessage(mMessage);

			mMessage.clear();
		}
		else
			mMessage.append(buf, count);
	}
	//-----------------------------------------------------------------------
	void OutputLogger::printMessage(const String& message)
	{
		ListenerMap::iterator it = mListeners.begin();
		ListenerMap::iterator end = mListeners.end();
		while(it != end)
		{
			it->second->output(message);
			++it;
		}
	}
	//-----------------------------------------------------------------------
}
