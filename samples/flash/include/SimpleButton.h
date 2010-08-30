#pragma once

#define AVMSHELL_BUILD
#include "avmplus.h"

#ifndef DECLARE_SLOTS_SimpleButtonClass
#define DECLARE_SLOTS_SimpleButtonClass
#endif

#ifndef DECLARE_SLOTS_SimpleButton
#define DECLARE_SLOTS_SimpleButton
#endif

namespace vtx
{
	namespace swf
	{
		class SimpleButtonClass : public avmplus::ClassClosure
		{
		public:
			SimpleButtonClass(avmplus::VTable* cvtable);
			avmplus::ScriptObject* createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype);

			DECLARE_SLOTS_SimpleButtonClass;
		};

		class SimpleButton : public InteractiveObject
		{
		public:
			SimpleButton(avmplus::VTable* vtable, avmplus::ScriptObject* prototype);

			DECLARE_SLOTS_SimpleButton;
		};
	}
}
