#include <iostream>

#include "cspNativePackage.h"
#include "cspOutputListener.h"
#include "cspSystem.h"
#include "cspVmCore.h"

#include "flash_classes.h"

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
	NativePackage(core, flash);

	bool ok = true;

	if(core->initializePackages())
	{
		ok &= core->parseFile("../abc/vtx_button.swf");

		avmplus::ScriptObject* movie = core->createObject("MainTimeline", "vtx_button_fla");
		//csp::ScriptObject* button1 = core->createObject("SimpleButton", "flash.display");
		//movie->setSlotObject("button1", button1);

		avmplus::ScriptObject* button1 = core->createObject("SimpleButton", "flash.display");
		csp::VmCore::setSlotObject(movie, "button1", button1);

		csp::VmCore::callObjectFunction(movie, "frame1");

		csp::ArgumentList evt_args;
		evt_args.push_back(core->scriptString("click"));
		evt_args.push_back(core->scriptBoolean(false));
		evt_args.push_back(core->scriptBoolean(true));
		avmplus::ScriptObject* evt = core->createObject("MouseEvent", "flash.events", evt_args);

		csp::ArgumentList fn_args;
		fn_args.push_back(evt->atom());

		for(int i=0; i<10; ++i)
		{
			csp::VmCore::callObjectFunction(button1, "dispatchEvent", fn_args);
		}

		vtx::swf::SimpleButton* vtx_button = static_cast<vtx::swf::SimpleButton*>(button1);
		if(vtx_button)
		{
			std::cout << "DEBUG_X: " << vtx_button->get_x() << std::endl;
			vtx_button->set_x(vtx_button->get_x()+10);
			csp::VmCore::callObjectFunction(button1, "dispatchEvent", fn_args);
		}
		else
		{
			std::cout << "cast error" << std::endl;
		}
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
