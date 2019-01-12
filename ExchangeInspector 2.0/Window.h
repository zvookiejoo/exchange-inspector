#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include "Exception.h"

class Window
{
	HWND handle, hList;
	HINSTANCE hInstance;

	Window();
	~Window();
	Window(Window const &) = delete;
	Window & operator=(Window const &) = delete;
public:
	static Window & getInstance();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

