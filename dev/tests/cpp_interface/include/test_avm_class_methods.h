#include "test_base.h"

class avm_class_test : public test_base
{
	//-----------------------------------------------------------------------
	const char* name() { return "avm_class_test"; }
	//-----------------------------------------------------------------------
	void test_global_functions(csp::VmCore* core)
	{
		avmplus::ClassClosure* function0 = core->getClassClosure("function0", "GlobalFunctions");
		core->callGlobalFunction(function0);

		avmplus::ClassClosure* function1 = core->getClassClosure("function1", "GlobalFunctions");
		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = core->callGlobalFunction(function1, args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
	void test_static_class_functions(csp::VmCore* core)
	{
		avmplus::ClassClosure* clazz = core->getClassClosure("Class", "ClassFunctions");
		csp::VmCore::callFunction(clazz, "static_function0");

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::MethodEnv* method = core->getMethodEnv(clazz, "static_function1");
		avmplus::Atom result = csp::VmCore::callFunction(clazz, method, args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* test_class_construction(csp::VmCore* core)
	{
		avmplus::ClassClosure* clazz = core->getClassClosure("Class", "ClassFunctions");

		avmplus::Atom args[] = 
		{
			0, 
			core->toScript("arg0"), 
			core->toScript("arg1")
		};

		return core->createObject(clazz, args);
	}
	//-----------------------------------------------------------------------
	void test_class_member_functions(csp::VmCore* core, avmplus::ScriptObject* obj)
	{
		avmplus::MethodEnv* member_function0 = csp::VmCore::getMethodEnv(obj, "member_function0");
		csp::VmCore::callFunction(obj, member_function0);

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::MethodEnv* member_function1 = csp::VmCore::getMethodEnv(obj, "member_function1");
		avmplus::Atom result = csp::VmCore::callFunction(
			obj, 
			member_function1, 
			args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
};
