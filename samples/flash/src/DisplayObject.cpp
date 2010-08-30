#include "flash_classes.h"

namespace vtx
{
	namespace swf
	{
		//-----------------------------------------------------------------------
		DisplayObjectClass::DisplayObjectClass(avmplus::VTable* cvtable) 
			: ClassClosure(cvtable)
		{
			AvmAssert(traits()->getSizeOfInstance() == sizeof(DisplayObjectClass));
			createVanillaPrototype();
		}
		//-----------------------------------------------------------------------
		avmplus::ScriptObject* DisplayObjectClass::createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype)
		{
			return new (core()->GetGC(), ivtable->getExtraSize()) DisplayObject(ivtable, prototype);
		}
		//-----------------------------------------------------------------------
		DisplayObject::DisplayObject(avmplus::VTable* vtable, avmplus::ScriptObject* prototype) 
			: EventDispatcher(vtable, prototype)
		{
			mValue = 0; // <--- causes crash in Release-Build or disrupts proper ABC execution
		}
		//-----------------------------------------------------------------------
		double DisplayObject::get_x()
		{
			return mValue;
		}
		//-----------------------------------------------------------------------
		void DisplayObject::set_x(double val)
		{
			mValue = val; // <--- causes crash in Release-Build or disrupts proper ABC execution
		}
		//-----------------------------------------------------------------------
	}
}
