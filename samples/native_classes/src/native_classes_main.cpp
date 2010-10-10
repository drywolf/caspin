#include <iostream>

#include "cspNativePackage.h"
#include "cspOutputListener.h"
#include "cspSystem.h"
#include "cspVmCore.h"

// your own native classes
#include "MyClass.h"

using namespace avmplus::NativeID;
//-----------------------------------------------------------------------
#ifdef WIN32
#	define SAMPLE_PATH csp::String("../../samples/native_classes")
#else
#	define SAMPLE_PATH csp::String("../samples/native_classes")
#endif
//-----------------------------------------------------------------------
class coutLog : public csp::OutputListener
{
public:
	void output(const csp::String& message)
	{
		std::cout << message << std::endl;
	}
};
//-----------------------------------------------------------------------
int main(int argc, char* argv[])
{
	std::cout << "----------Start-----------" << std::endl << std::endl;

	csp::VmCore::createGcHeap();

	csp::VmCore* core;
	CSP_CREATE_VMCORE(core);

	coutLog logger;
	core->addListener(&logger);

	NativePackage(core, caspin_base);
	NativePackage(core, myclasses);

	core->initializePackages();

	//avmplus::ClassClosure* clazz = core->getClassClosure("MyClass", "avmplus");
	//if(clazz)
	//{
	//	avmplus::MyClassInstance* myclass = (avmplus::MyClassInstance*)core->createObject(clazz);
	//	core->stickyObject(myclass->atom());
	//	csp::VmCore::callFunction(myclass, "as3_function");
	//	core->unstickyObject(myclass->atom());
	//}

	//core->callGlobalFunction("hihi", "test");

	//csp::ArgumentList args;
	//args.push_back(core->scriptInteger(3));
	//args.push_back(core->scriptInteger(-234));
	//core->console << core->integer(core->callStaticFunction("MyClass", "static_as3_function", "avmplus", args)) << "\n";

	//avmplus::ScriptObject* obj = core->createBuiltinObject(abcclass_Array);
	//int myclasses_id = core_myclasses.getID();
 //	avmplus::MyClassInstance* myclass = (avmplus::MyClassInstance*)core->createNativeObject(abcclass_avmplus_MyClass, myclasses_id);
 //	//csp::VmCore::callFunction(myclass, "as3_function");
	//avmplus::MethodEnv* as3_function = csp::VmCore::getMethodEnv(myclass, "as3_function");
	//avmplus::MethodEnv* native_function = csp::VmCore::getMethodEnv(myclass, "native_function");

	//long time = timeGetTime();

	//csp::uint c = -1;
	//c = c / 4096;
	//avmplus::Atom arg[1] = { myclass->atom() };
	////for(csp::uint i=0; i<c; ++i)
	//	//csp::VmCore::callFunction(myclass, "as3_function", arg);
	//	//csp::VmCore::callFunction(myclass, as3_function, arg);

	//std::cout << timeGetTime() - time << std::endl;

	//avmplus::Atom args[2] = { myclass->atom(), core->toScript("54321") };
	//avmplus::Atom result = csp::VmCore::callFunction(myclass, "native_function", args);

	//core->console << core->integer(result) << "\n";
	bool ok = core->executeFile(SAMPLE_PATH + "/as3/playpen.abc");

	CSP_DESTROY_VMCORE(core);

	if(ok)
	{
		csp::VmCore::destroyGcHeap();
	}

	std::cout << std::endl << "-----------End------------" << std::endl;
	std::cout << std::endl << "Press ENTER to continue..." << std::endl;
	std::cin.get();

	return 0;
}
//-----------------------------------------------------------------------
