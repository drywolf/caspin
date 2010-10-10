#include "test_base.h"

class avm_string_test : public test_base
{
	//-----------------------------------------------------------------------
	const char* name() { return "avm_string_test"; }
	//-----------------------------------------------------------------------
	void test_global_functions(csp::VmCore* core)
	{
		core->callGlobalFunction(
			core->toScriptPtr("function0"), 
			core->toScriptPtr("GlobalFunctions"));

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = core->callGlobalFunction(
			core->toScriptPtr("function1"), 
			core->toScriptPtr("GlobalFunctions"), 
			args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
	void test_static_class_functions(csp::VmCore* core)
	{
		core->callStaticFunction(
			core->toScriptPtr("Class"), 
			core->toScriptPtr("static_function0"), 
			core->toScriptPtr("ClassFunctions"));

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = core->callStaticFunction(
			core->toScriptPtr("Class"), 
			core->toScriptPtr("static_function1"), 
			core->toScriptPtr("ClassFunctions"), 
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

		return core->createObject(
			core->toScriptPtr("Class"), 
			core->toScriptPtr("ClassFunctions"), 
			args);
	}
	//-----------------------------------------------------------------------
	void test_class_member_functions(csp::VmCore* core, avmplus::ScriptObject* obj)
	{
		csp::VmCore::callFunction(obj, core->toScriptPtr("member_function0"));

		avmplus::Atom args[] = { 0, core->toScript("arg0") };
		avmplus::Atom result = csp::VmCore::callFunction(
			obj, 
			core->toScriptPtr("member_function1"), 
			args);

		core->console << core->atomToString(result) << "\n";
	}
	//-----------------------------------------------------------------------
};
