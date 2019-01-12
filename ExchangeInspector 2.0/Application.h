#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "Window.h"
#include "Exception.h"

using namespace std;

class Application
{
	const Window & window = Window::getInstance();

	wstring fileName = L"";
	bool haveFile = false;
	map<wstring, int> objectList;

	Application();
	~Application();
	Application(Application const &) = delete;
	Application & operator=(Application const &) = delete;

	const wchar_t * getSystemErrorMessage();
public:
	static Application & getInstance();

	int run();
};

