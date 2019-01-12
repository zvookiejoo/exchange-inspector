#include "Window.h"



Window::Window()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	const wchar_t * className = L"ExchangeInspector2.0";

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = Window::WindowProc;
	wc.lpszClassName = className;
	wc.lpszMenuName = nullptr;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
		throw Exception(L"Ќе удалось зарегистрировать класс окна");

	handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		L"Exchange Inspector 2.0",
		WS_VISIBLE | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 500,
		NULL, NULL, hInstance, NULL);

	if (handle == NULL)
		throw Exception(L"Ќе удалось создать окно приложени€");
}


Window::~Window()
{
}

Window & Window::getInstance()
{
	static Window window;

	return window;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}