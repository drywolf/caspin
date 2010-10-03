#include "MyClass.cpp"

#include "cspNativePackage.h"
#include "cspOutputListener.h"
#include "cspSystem.h"
#include "cspVmCore.h"

#ifdef WIN32
#	define SAMPLE_PATH csp::String("../../samples/auto_bind")
#else
#	define SAMPLE_PATH csp::String("../samples/auto_bind")
#endif

class coutLog : public csp::OutputListener
{
public:
	void output(const csp::String& message)
	{
		std::cout << message << std::endl;
	}
};

int main()
{
	csp::VmCore::createGcHeap();

	csp::VmCore* core;
	CSP_CREATE_VMCORE(core);

	coutLog logger;
	core->addListener(&logger);

	NativePackage(core, caspin_base);
	NativePackage(core, auto_classes);

	core->initializePackages();

	bool ok = core->parseFile(SAMPLE_PATH + "/as3/playpen.abc");

	CSP_DESTROY_VMCORE(core);

	if(ok)
	{
		csp::VmCore::destroyGcHeap();
	}

	std::cin.get();

	return 0;
}