#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include "Exception.h"
#include "ITarget.h"

class Window
{
	HWND handle, hList;
	HINSTANCE hInstance;
	ITarget * pDropTarget;

	Window();
	~Window();
	Window(Window const &) = delete;
	Window & operator=(Window const &) = delete;
public:
	static Window & getInstance();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND getHandle();
};

