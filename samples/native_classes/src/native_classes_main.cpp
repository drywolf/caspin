#include <iostream>

#include "cspNativePackage.h"
#include "cspOutputListener.h"
#include "cspSystem.h"
#include "cspVmCore.h"

// your own native classes
#include "MyClass.h"

#ifdef WIN32
#	define SAMPLE_PATH csp::String("../../samples/native_classes")
#else
#	define SAMPLE_PATH csp::String("../samples/native_classes")
#endif

class coutLog : public csp::OutputListener
{
public:
	void output(const std::string& message)
	{
		std::cout << message << std::endl;
	}
};

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

	bool ok = core->parseFile(SAMPLE_PATH + "/as3/playpen.abc");

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
