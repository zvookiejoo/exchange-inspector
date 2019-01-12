#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	int resultCode = -1;

	try
	{
		resultCode = Application::getInstance().run();
	}
	catch (Exception e)
	{
		MessageBox(NULL, e.what().c_str(), L"Exchange Inspector v2.0", MB_ICONERROR | MB_OK);
	}

	return resultCode;
}