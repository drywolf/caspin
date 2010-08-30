#include <iostream>

#include "cspNativePackage.h"
#include "cspOutputListener.h"
#include "cspSystem.h"
#include "cspVmCore.h"

#ifdef WIN32
#	define SAMPLE_PATH csp::String("../../samples/minimal")
#else
#	define SAMPLE_PATH csp::String("../samples/minimal")
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

	core->initializePackages();

	bool ok = core->parseFile(SAMPLE_PATH + "/as3/minimal.abc");

	csp::ArgumentList args;
	args.push_back(core->scriptInteger(2));
	args.push_back(core->scriptInteger(5));

	csp::ClassDef def = core->getDefinition("MinimalSample");
	if(def.scriptEnv)
	{
		avmplus::ScriptObject* obj = core->createObject(def, args);
		std::cout << core->integer(csp::VmCore::callObjectFunction(obj, "add")) << std::endl;

		args.clear();
		args.push_back(core->scriptInteger(125));
		args.push_back(core->scriptInteger(-120));
		std::cout << core->integer(csp::VmCore::callObjectFunction(obj, "add2", args)) << std::endl;
	}

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
