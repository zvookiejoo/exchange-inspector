#include "Application.h"



Application::Application()
{
	int argc = 0;

	TCHAR * lpCmdLine = GetCommandLine();

	TCHAR ** argv = CommandLineToArgvW(lpCmdLine, &argc);

	if (argc == 2)
	{
		if (isFileExist(argv[1]))
		{
			fileName = argv[1];
			haveFile = true;
		}
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

bool Application::isFileExist(const wchar_t * _fileName)
{
	if (_fileName == NULL)
		throw Exception(L"isFileExist() получила нулевой указатель");

	bool result = false;

	HANDLE hFile = CreateFile(_fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		
		result = true;
	}

	return result;
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

void Application::setFile(const wchar_t * _fileName)
{
	if (_fileName == nullptr)
		throw Exception(L"Application::setFile() получила нулевой указатель");

	if (parseInProgress || haveFile)
		return;

	if (!isFileExist(_fileName))
	{
		throw Exception(getSystemErrorMessage());
	}

	fileName = _fileName;
	haveFile = true;

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		throw Exception(getSystemErrorMessage());
	}

	char buf[3];
	unsigned long bytesRead = 0;

	if (!ReadFile(hFile, buf, sizeof(char) * 2, &bytesRead, NULL))
	{
		if (bytesRead != (sizeof(wchar_t)) * 2)
		{
			CloseHandle(hFile);
			throw Exception(getSystemErrorMessage());
		}
	}

	string zip = "PK";
	string xml = "<?";
	buf[2] = 0;

	if (zip == buf)
	{
		// TODO: unzip the file to temp folder

		fileName = L"%TEMP%\\message.xml";
	}
	else if (xml == buf)
	{
		// TODO: run the parser
	}
	else
	{
		throw Exception(L"Неизвестный формат файла");
	}
}

void Application::error(wstring const & message)
{
	MessageBox(window.getHandle(), message.c_str(), L"Exchange Inspector v2.0", MB_ICONERROR | MB_OK);
}

void Application::fatalError(wstring const & message)
{
	error(message);
	exit(-1);
}

bool Application::isDataPresent()
{
	return haveFile;
}

void Application::clearData()
{
	objectList.clear();
	fileName = L"";
	haveFile = false;
}
