#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <map>
#include "Exception.h"
#include "ITarget.h"
#include "ListView.h"

using namespace std;

typedef enum _WindowState {
	MISSING_DATA,
	DECOMPRESSING,
	HAVE_DATA
} WindowState;

class Window
{
	HWND handle = NULL;
	HINSTANCE hInstance;
	ITarget * pDropTarget;
	WindowState state;
	ListView * list = NULL;
	map<wstring, int> data;

	Window();
	~Window();
	Window(Window const &) = delete;
	Window & operator=(Window const &) = delete;
public:
	static Window & getInstance();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void create();
	HWND getHandle();
	void setState(WindowState state);
	void setData(map<wstring, int> const & data);
};

