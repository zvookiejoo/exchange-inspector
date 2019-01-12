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

const char * Application::unpackZip(const wstring & _fileName)
{
	FILE * zipFile = NULL;

	map<string *, const char *> files;

	if (_wfopen_s(&zipFile, _fileName.c_str(), L"rb") != 0)
		throw Exception(L"Не удалось открыть файл");

	ZLocalHeader localHeader = { 0 };
	char * name = NULL;
	char * data = NULL;
	bool found = false;

	string fname("Message_001_670.xml");

	fread(&localHeader, sizeof(localHeader), 1, zipFile);

	while (localHeader.header == ZHEADER)
	{
		name = new char[localHeader.fileNameLength + 1];
		data = new char[localHeader.compSize];

		memset(name, '\0', localHeader.fileNameLength + 1);
		memset(data, 0, localHeader.compSize);

		fread(name, localHeader.fileNameLength, 1, zipFile);
		fread(data, localHeader.compSize, 1, zipFile);

		if (fname == name)
		{
			found = true;
			break;
		}
		
		delete name;
		delete data;

		fread(&localHeader, sizeof(localHeader), 1, zipFile);
	}

	if (!found)
		throw Exception(L"В архиве не найден файл Message_001_670.xml");
	else
		return data;
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
		throw Exception(getSystemErrorMessage());

	fileName = _fileName;
	// TODO: uncomment this line after parser implemetation
	//haveFile = true;

	FILE * inputFile = NULL;

	errno_t result = _wfopen_s(&inputFile, fileName.c_str(), L"rb");

	if ((result != 0) || (inputFile == NULL))
		throw Exception(L"Не удаётся открыть файл");

	char buf[3] = { 0 };

	if (fread(buf, 1, 2, inputFile) != 2)
	{
		fclose(inputFile);
		throw Exception(L"Не удалось прочитать начало файла");
	}

	fclose(inputFile);

	const char t[3] = { (char)0xEF, (char)0xBB, (char)0 };
	string xml = t;
	string zip = "PK";

	ZLocalHeader localHeader = { 0 };

	if (zip == buf)
	{
		// TODO: unpack message from archive and then run the parser
		char * data = NULL;
		try
		{
			data = (char *)unpackZip(fileName);
		}
		catch (Exception e)
		{
			error(e.what());
		}
	}
	else if (xml == buf)
	{
		// TODO: run the parser
	}
	else
	{
		_fcloseall();
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
