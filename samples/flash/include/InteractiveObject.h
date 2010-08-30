#pragma once

#define AVMSHELL_BUILD
#include "avmplus.h"

#ifndef DECLARE_SLOTS_InteractiveObjectClass
#define DECLARE_SLOTS_InteractiveObjectClass
#endif

#ifndef DECLARE_SLOTS_InteractiveObject
#define DECLARE_SLOTS_InteractiveObject
#endif

namespace vtx
{
	namespace swf
	{
		class InteractiveObjectClass : public avmplus::ClassClosure
		{
		public:
			InteractiveObjectClass(avmplus::VTable* cvtable);
			avmplus::ScriptObject* createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype);

			DECLARE_SLOTS_InteractiveObjectClass;
		};

		class InteractiveObject : public DisplayObject
		{
		public:
			InteractiveObject(avmplus::VTable* vtable, avmplus::ScriptObject* prototype);

			DECLARE_SLOTS_InteractiveObject;
		};
	}
}
