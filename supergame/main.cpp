
#include "TheApp.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nShowCmd)
{
	auto app = std::make_unique<TheApp>();
	if (!app->Create(1280, 720, "SUPERGAME"))
	{
		IApplication::Debug("APP START FAILED!");
		return 0;
	}

	app->Run();
	return 0;
}


