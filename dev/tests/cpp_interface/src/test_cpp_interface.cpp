#include <iostream>

#include "cspNativePackage.h"
#include "cspOutputListener.h"
#include "cspSystem.h"
#include "cspVmCore.h"

#include "test_avm_class_methods.h"
#include "test_avm_string_methods.h"
#include "test_csp_string_methods.h"

#ifdef WIN32
#	define TEST_PATH csp::String("")
#else
#	define TEST_PATH csp::String("")
#endif
//-----------------------------------------------------------------------
class testLog : public csp::OutputListener
{
public:
	virtual ~testLog(){}

	std::stringstream str;

	void output(const csp::String& message)
	{
		std::cout << message << std::endl;
		str << message << std::endl;
	}
};
//-----------------------------------------------------------------------
int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "No target interface provided, aborting test..." << std::endl;
		std::cout << "Valid targets are: { avm_class , avm_string , csp_string }" << std::endl;
		return 1;
	}

	csp::String test_target(argv[1]);

	csp::VmCore::createGcHeap();

	csp::VmCore* core;
	CSP_CREATE_VMCORE(core);

	MMGC_GCENTER(core->GetGC());

	testLog logger;
	core->addListener(&logger);

	NativePackage(core, caspin_base);

	core->initializePackages();

	bool ok = core->executeFile(TEST_PATH + "as3/test_cpp_interface.abc");

	if(ok)
	{
		test_base* test = NULL;

		if(test_target == "avm_class")
			test = new avm_class_test;
		else if(test_target == "avm_string")
			test = new avm_string_test;
		else if(test_target == "csp_string")
			test = new csp_string_test;

		std::cout << 
			"-----------------------------------------------------" << std::endl << 
			" RUNNING CPP INTERFACE TEST(" << test->name() << ")..." << std::endl << 
			"-----------------------------------------------------" << std::endl;

		test->test_global_functions(core);
		test->test_static_class_functions(core);
		avmplus::ScriptObject* created_obj = test->test_class_construction(core);

		if(created_obj)
			test->test_class_member_functions(core, created_obj);
		else
			core->console << "Error during test_class_construction(...)\n";
	}

	csp::String correct_log = csp::String() + 
		"GlobalFunctions.function0()\n" + 
		"GlobalFunctions.function1(arg0)\n" + 
		"arg0\n" + 
		"ClassFunctions.Class.static_function0()\n" + 
		"ClassFunctions.Class.static_function1(arg0)\n" + 
		"arg0\n" + 
		"ClassFunctions.Class.Constructor(arg0, arg1)\n" + 
		"ClassFunctions.Class.member_function0()\n" + 
		"ClassFunctions.Class.member_function1(arg0)\n" + 
		"arg0\n";

	if(logger.str.str() == correct_log)
		std::cout << 
		"-----------------------------------------------------" << std::endl << 
		" TEST SUCCESSFUL" << std::endl << 
		"-----------------------------------------------------" << std::endl;
	else
		std::cout << "TEST FAILED" << std::endl << std::endl;

	CSP_DESTROY_VMCORE(core);

	if(ok)
	{
		csp::VmCore::destroyGcHeap();
	}

	return 0;
}
//-----------------------------------------------------------------------
