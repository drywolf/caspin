#pragma once

#include "cspPrerequisites.h"

#include "myclasses.h"

namespace avmplus
{
	//-----------------------------------------------------------------------
	class MyClassInstance : public avmplus::ScriptObject
	{
	public:
		MyClassInstance(avmplus::VTable* vtable, avmplus::ScriptObject* prototype);
		~MyClassInstance();

		int native_function(avmplus::Stringp str);

		CSP_INST_SLOTS(MyClassInstance);
	};
	//-----------------------------------------------------------------------
	class MyClass : public avmplus::ClassClosure
	{
	public:
		MyClass(avmplus::VTable* cvtable);
		~MyClass();

		avmplus::Stringp static_native_function();

		CSP_CLASS_CREATE_INSTANCE(MyClass, MyClassInstance);
		CSP_CLASS_SLOTS(MyClass);
	};
	//-----------------------------------------------------------------------
}
