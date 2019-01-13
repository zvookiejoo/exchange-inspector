#include "Window.h"



void Window::create()
{
	hInstance = GetModuleHandle(NULL);

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
		throw Exception(L"Не удалось зарегистрировать класс окна");

	handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		L"Exchange Inspector 2.0",
		WS_VISIBLE | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 500,
		NULL, NULL, hInstance, NULL);

	if (handle == NULL)
		throw Exception(L"Не удалось создать окно приложения");

	SetWindowLong(handle, GWL_USERDATA, (LONG)this);

	ListViewInit lvInit = { 0 };
	RECT rect;

	GetClientRect(handle, &rect);

	lvInit.hParentWnd = handle;
	lvInit.hInstance = hInstance;
	lvInit.x = rect.left;
	lvInit.y = rect.top;
	lvInit.width = rect.right - rect.left;
	lvInit.height = rect.bottom - rect.top;

	list = new ListView(&lvInit);

	OleInitialize(NULL);

	pDropTarget = new ITarget();

	if (RegisterDragDrop(handle, pDropTarget) != S_OK)
		throw Exception(L"Не удалось зарегистрировать приёмник Drag&Drop");
}


Window::Window()
{
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

HWND Window::getHandle()
{
	return handle;
}

void Window::setState(WindowState state)
{
	switch (this->state) {
	case (MISSING_DATA):
		this->state = state;
		break;
	case (DECOMPRESSING):
		this->state = state;
		break;
	}
}

void Window::setData(map<wstring, int> const & data)
{
	this->data = data;

	list->update(data);
}
