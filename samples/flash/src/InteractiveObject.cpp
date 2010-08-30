#include "flash_classes.h"

namespace vtx
{
	namespace swf
	{
		//-----------------------------------------------------------------------
		InteractiveObjectClass::InteractiveObjectClass(avmplus::VTable* cvtable) 
			: ClassClosure(cvtable)
		{
			AvmAssert(traits()->getSizeOfInstance() == sizeof(InteractiveObjectClass));
			createVanillaPrototype();
		}
		//-----------------------------------------------------------------------
		avmplus::ScriptObject* InteractiveObjectClass::createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype)
		{
			return new (core()->GetGC(), ivtable->getExtraSize()) InteractiveObject(ivtable, prototype);
		}
		//-----------------------------------------------------------------------
		InteractiveObject::InteractiveObject(avmplus::VTable* vtable, avmplus::ScriptObject* prototype) 
			: DisplayObject(vtable, prototype)
		{

		}
		//-----------------------------------------------------------------------
	}
}
