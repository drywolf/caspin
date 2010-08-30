#include "flash_classes.h"

namespace vtx
{
	namespace swf
	{
		//-----------------------------------------------------------------------
		SimpleButtonClass::SimpleButtonClass(avmplus::VTable* cvtable) 
			: ClassClosure(cvtable)
		{
			AvmAssert(traits()->getSizeOfInstance() == sizeof(SimpleButtonClass));
			createVanillaPrototype();
		}
		//-----------------------------------------------------------------------
		avmplus::ScriptObject* SimpleButtonClass::createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype)
		{
			return new (core()->GetGC(), ivtable->getExtraSize()) SimpleButton(ivtable, prototype);
		}
		//-----------------------------------------------------------------------
		SimpleButton::SimpleButton(avmplus::VTable* vtable, avmplus::ScriptObject* prototype) 
			: InteractiveObject(vtable, prototype)
		{

		}
		//-----------------------------------------------------------------------
	}
}
