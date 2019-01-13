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
	PARSING,
	HAVE_DATA
} WindowState;

class Window
{
	const wstring title = L"Exchange Inspector v2.0";
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
	void setTitle(wstring const & title);
public:
	static Window & getInstance();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void create();
	HWND getHandle();
	void setState(WindowState state);
	void setData(map<wstring, int> const & data);
	const wstring & getTitle();
	void updateList(map<wstring, int> const & data);
};
