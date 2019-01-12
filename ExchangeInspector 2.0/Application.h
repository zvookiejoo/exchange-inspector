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
	Window & window = Window::getInstance();

	wstring fileName = L"";
	bool haveFile = false;
	bool parseInProgress = false;
	map<wstring, int> objectList;

	Application();
	~Application();
	Application(Application const &) = delete;
	Application & operator=(Application const &) = delete;

	const wchar_t * getSystemErrorMessage();
	bool isFileExist(const wchar_t * _fileName);
	const char * unpackZip(const wstring & _fileName);
public:
	static Application & getInstance();

	int run();
	void setFile(const wchar_t * _fileName);
	void error(wstring const & message);
	void fatalError(wstring const & message);
	bool isDataPresent();
	void clearData();
};

