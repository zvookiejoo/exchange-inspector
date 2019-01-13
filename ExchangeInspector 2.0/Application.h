#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "Window.h"
#include "Exception.h"
#include "zlib\zlib.h"
#include "zip.h"

using namespace std;

class Application
{
	Window * window = NULL;

	wstring fileName = L"";
	const wstring tempFileName = L"message.xml";

	bool haveFile = false;
	bool parseInProgress = false;
	map<wstring, int> objectList;

	Application();
	~Application();
	Application(Application const &) = delete;
	Application & operator=(Application const &) = delete;

	const wchar_t * getSystemErrorMessage();
	bool isFileExist(const wchar_t * _fileName);
	FILE * findMessage(const wstring & _fileName);
	void unpackMessage(FILE * message);
public:
	static Application & getInstance();

	int run();
	void processFile(const wchar_t * _fileName);
	void error(wstring const & message);
	void fatalError(wstring const & message);
	bool isDataPresent();
	void clearData();
};

