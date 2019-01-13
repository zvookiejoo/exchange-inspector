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
			try
			{
				processFile(argv[1]);
			}
			catch (Exception e)
			{
				error(e.what());
			}
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

FILE * Application::findMessage(const wstring & _fileName)
{
	FILE * zipFile = NULL;

	if (_wfopen_s(&zipFile, _fileName.c_str(), L"rb") != 0)
		throw Exception(L"Не удалось открыть файл");

	ZLocalHeader localHeader = { 0 };
	char * name = NULL;
	bool found = false;

	string fname("Message_001_670.xml");

	fread(&localHeader, sizeof(localHeader), 1, zipFile);

	while (localHeader.header == ZHEADER)
	{
		name = new char[localHeader.fileNameLength + 1];

		memset(name, '\0', localHeader.fileNameLength + 1);

		fread(name, localHeader.fileNameLength, 1, zipFile);

		if (localHeader.extraFieldLength > 0)
		{
			fseek(zipFile, localHeader.extraFieldLength, SEEK_CUR);
		}
		
		if (fname == name)
		{
			found = true;
			break;
		}
		
		delete name;

		fseek(zipFile, localHeader.compSize, SEEK_CUR);
		fread(&localHeader, sizeof(localHeader), 1, zipFile);
	}

	if (found)
	{
		long offset = (localHeader.extraFieldLength + localHeader.fileNameLength + sizeof(localHeader)) * (-1);

		return zipFile;
	}
	else
	{
		fclose(zipFile);
		return NULL;
	}
}

void Application::unpackMessage(FILE * message)
{
	FILE * in = findMessage(fileName);

	if (in == NULL)
		throw Exception(L"В архиве не найден файл Message_001_670.xml");

	FILE * out = NULL;
	_wfopen_s(&out, tempFileName.c_str(), L"wb");

	if (out == NULL)
	{
		_fcloseall();
		throw Exception(L"Не удалось открыть временный файл");
	}

	uint8_t src[CHUNK_SIZE] = { 0 }, dst[CHUNK_SIZE] = { 0 };

	z_stream s = { 0 };

	int result = inflateInit2(&s, -MAX_WBITS);

	if (result != Z_OK)
	{
		_fcloseall();
		inflateEnd(&s);
		throw Exception(L"Ошибка инициализации распаковщика");
	}

	do {
		s.avail_in = fread(src, 1, CHUNK_SIZE, in);

		if (ferror(in))
		{
			_fcloseall();
			inflateEnd(&s);
			throw Exception(L"Ошибка чтения входного файла");
		}

		if (s.avail_in == 0)
			break;

		s.next_in = src;

		do {
			s.avail_out = CHUNK_SIZE;
			s.next_out = dst;

			result = inflate(&s, Z_NO_FLUSH);

			if (result == Z_NEED_DICT || result == Z_DATA_ERROR || result == Z_MEM_ERROR)
			{
				_fcloseall();
				inflateEnd(&s);
				string t(s.msg);
				wstring text(t.begin(), t.end());
				throw Exception(text.c_str());
			}

			uint32_t have = CHUNK_SIZE - s.avail_out;

			if (fwrite(dst, 1, have, out) != have || ferror(out))
			{
				_fcloseall();
				inflateEnd(&s);
				throw Exception(L"Ошибка записи выходного файла");
			}
		} while (s.avail_out == 0);
	} while (result != Z_STREAM_END);

	_fcloseall();
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

	if (isFileExist(tempFileName.c_str()))
		DeleteFile(tempFileName.c_str());

	return 0;
}

void Application::processFile(const wchar_t * _fileName)
{
	if (_fileName == nullptr)
		throw Exception(L"Application::processFile() получила нулевой указатель");

	if (parseInProgress || haveFile)
		return;

	if (!isFileExist(_fileName))
		throw Exception(getSystemErrorMessage());

	fileName = _fileName;
	haveFile = true;

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
		FILE * file = findMessage(fileName);
		unpackMessage(file);
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
