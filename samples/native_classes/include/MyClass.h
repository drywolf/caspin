#pragma once

#include "cspPrerequisites.h"

#include "myclasses.h"

namespace avmplus
{
	class MyClass : public avmplus::ClassClosure
	{
	public:
		MyClass(avmplus::VTable* cvtable);
		~MyClass();

		avmplus::ScriptObject* createInstance(avmplus::VTable* ivtable, avmplus::ScriptObject* prototype);

		avmplus::Stringp getTestString();
	};

	class MyClassInstance : public avmplus::ScriptObject
	{
	public:
		MyClassInstance(avmplus::VTable* vtable, avmplus::ScriptObject* prototype);
		~MyClassInstance();

		int stringTest(avmplus::Stringp str);
	};
}
