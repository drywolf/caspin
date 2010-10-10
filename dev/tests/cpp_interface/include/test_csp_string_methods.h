#include "test_base.h"

class csp_string_test : public test_base
{
	//-----------------------------------------------------------------------
	const char* name() { return "csp_string_test"; }
	//-----------------------------------------------------------------------
	void test_global_functions(csp::VmCore* core)
	{
		core->callGlobalFunction(
			"function0", 
			"GlobalFunctions");

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = core->callGlobalFunction(
			"function1", 
			"GlobalFunctions", 
			args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
	void test_static_class_functions(csp::VmCore* core)
	{
		core->callStaticFunction(
			"Class", 
			"static_function0", 
			"ClassFunctions");

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = core->callStaticFunction(
			"Class", 
			"static_function1", 
			"ClassFunctions", 
			args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
	avmplus::ScriptObject* test_class_construction(csp::VmCore* core)
	{
		avmplus::Atom args[] = 
		{
			0, 
			core->toScript("arg0"), 
			core->toScript("arg1")
		};

		return core->createObject("Class", "ClassFunctions", args);
	}
	//-----------------------------------------------------------------------
	void test_class_member_functions(csp::VmCore* core, avmplus::ScriptObject* obj)
	{
		csp::VmCore::callFunction(obj, "member_function0");

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = csp::VmCore::callFunction(obj, "member_function1", args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
};
