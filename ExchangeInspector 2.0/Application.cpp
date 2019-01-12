#include "Application.h"



Application::Application()
{
	int argc = 0;

	TCHAR * lpCmdLine = GetCommandLine();

	TCHAR ** argv = CommandLineToArgvW(lpCmdLine, &argc);

	if (argc == 2)
	{
		fileName = argv[1];
		
		HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			const wchar_t * errorText = getSystemErrorMessage();

			throw Exception(errorText);
		}

		CloseHandle(hFile);

		haveFile = true;
	}
}

Application::~Application()
{
}

const wchar_t * Application::getSystemErrorMessage()
{
	int code = GetLastError();
	wchar_t * message = NULL;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
		code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&message, 0, NULL);

	return message;
}

Application & Application::getInstance()
{
	static Application app;

	return app;
}

int Application::run()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
