#pragma once

#include "cspPrerequisites.h"

#include "myclasses.h"

namespace my { namespace pkg {
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
	class MyClassClass : public avmplus::ClassClosure
	{
	public:
		MyClassClass(avmplus::VTable* cvtable);
		~MyClassClass();

		avmplus::Stringp static_native_function();

		CSP_CLASS_CREATE_INSTANCE(MyClassClass, MyClassInstance);
		CSP_CLASS_SLOTS(MyClassClass);
	};
	//-----------------------------------------------------------------------
}}
