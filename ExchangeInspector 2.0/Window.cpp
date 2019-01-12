#include "Window.h"



Window::Window()
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

	INITCOMMONCONTROLSEX icex;

	icex.dwICC |= ICC_LISTVIEW_CLASSES;

	InitCommonControlsEx(&icex);

	RECT clientRect;

	if (!GetClientRect(handle, &clientRect))
		throw Exception(L"Не удалось получить размеры клиентской части окна");

	hList = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_LISTVIEW,
		NULL,
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		clientRect.left, clientRect.top,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,
		handle, NULL, hInstance, NULL);

	if (hList == NULL)
		throw Exception(L"Не удалось создать окно списка объектов");

	DWORD exStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, exStyle, exStyle);

	LVCOLUMN lvc;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT;
	lvc.iSubItem = 0;
	lvc.pszText = (wchar_t*)L"Тип объекта";
	lvc.cx = 250;

	if (SendMessage(hList, LVM_INSERTCOLUMN, lvc.iSubItem, (LPARAM)&lvc) == -1)
		throw Exception(L"Не удалось создать колонку \"Тип объекта\"");

	lvc.iSubItem = 1;
	lvc.pszText = (wchar_t*)L"Количество";
	lvc.cx = 100;

	if (SendMessage(hList, LVM_INSERTCOLUMN, lvc.iSubItem, (LPARAM)&lvc) == -1)
		throw Exception(L"Не удалось создать колонку \"Количество\"");
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