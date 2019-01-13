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
		throw Exception(L"�� ������� ���������������� ����� ����");

	handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		title.c_str(),
		WS_VISIBLE | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 500,
		NULL, NULL, hInstance, NULL);

	if (handle == NULL)
		throw Exception(L"�� ������� ������� ���� ����������");

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
		throw Exception(L"�� ������� ���������������� ������� Drag&Drop");

	setState(MISSING_DATA);
}


Window::Window()
{
}

Window::~Window()
{
}

void Window::setTitle(wstring const & title)
{
	SetWindowText(handle, title.c_str());
}

Window & Window::getInstance()
{
	static Window window;

	return window;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window * window = (Window*)GetWindowLong(hWnd, GWL_USERDATA);

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		RECT rect;
		GetClientRect(hWnd, &rect);
		if (window != NULL)
		{
			window->list->resize(&rect);
		}
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
	switch (state) {
	case (MISSING_DATA):
		this->state = state;
		setTitle(title + L" - ��� ������");
		break;
	case (DECOMPRESSING):
		this->state = state;
		setTitle(title + L" - ����������");
		break;
	case (PARSING):
		this->state = state;
		setTitle(title + L" - ������ ������");
		break;
	}
}

void Window::setData(map<wstring, int> const & data)
{
	this->data = data;

	list->update(data);

	state = HAVE_DATA;

	setTitle(title + L" - ���������");
}

const wstring & Window::getTitle()
{
	return this->title;
}

void Window::updateList(map<wstring, int> const & data)
{
	this->data = data;

	list->update(this->data);
}
