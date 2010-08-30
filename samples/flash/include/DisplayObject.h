#pragma once

#define AVMSHELL_BUILD
#include "avmplus.h"

#ifndef DECLARE_SLOTS_DisplayObjectClass
#define DECLARE_SLOTS_DisplayObjectClass
#endif

#ifndef DECLARE_SLOTS_DisplayObject
#define DECLARE_SLOTS_DisplayObject
#endif

namespace vtx
{
	namespace swf
	{
		class DisplayObjectClass : public avmplus::ClassClosure
		{
		public:
			DisplayObjectClass(avmplus::VTable* cvtable);
			avmplus::ScriptObject* createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype);

			DECLARE_SLOTS_DisplayObjectClass;
		};

		class DisplayObject : public EventDispatcher
		{
		public:
			DisplayObject(avmplus::VTable* vtable, avmplus::ScriptObject* prototype);

			double mValue;

			double get_x();
			void set_x(double val);

			DECLARE_SLOTS_DisplayObject;
		};
	}
}
