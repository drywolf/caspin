#include "flash_classes.h"

namespace vtx
{
	namespace swf
	{
		//-----------------------------------------------------------------------
		EventDispatcherClass::EventDispatcherClass(avmplus::VTable* cvtable) 
			: ClassClosure(cvtable)
		{
			AvmAssert(traits()->getSizeOfInstance() == sizeof(EventDispatcherClass));
			createVanillaPrototype();
		}
		//-----------------------------------------------------------------------
		avmplus::ScriptObject* EventDispatcherClass::createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype)
		{
			return new (core()->GetGC(), ivtable->getExtraSize()) EventDispatcher(ivtable, prototype);
		}
		//-----------------------------------------------------------------------
		EventDispatcher::EventDispatcher(avmplus::VTable* vtable, avmplus::ScriptObject* prototype) 
			: ScriptObject(vtable, prototype)
		{

		}
		//-----------------------------------------------------------------------
		//bool EventDispatcher::dispatchEvent(ScriptObject* event)
		//{
		//	ScriptObject* mListeners = AvmCore::atomToScriptObject(get_private_mListeners());
		//	if(mListeners)
		//	{
		//		Atom args[2] = { get_private_mListeners(), event->atom() };
		//		mListeners->call(1, args);
		//	}

		//	return false;
		//}
		//-----------------------------------------------------------------------
	}
}
