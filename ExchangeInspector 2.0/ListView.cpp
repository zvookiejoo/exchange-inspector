#include "ListView.h"
#include "Application.h"


ListView::ListView(const PListViewInit init)
{
	Application & app = Application::getInstance();

	if (init == NULL)
		app.fatalError(L"Конструктор ListView получил нулевой указатель");

	INITCOMMONCONTROLSEX icex;

	icex.dwICC |= ICC_LISTVIEW_CLASSES;

	InitCommonControlsEx(&icex);

	handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_LISTVIEW,
		NULL,
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		init->x, init->y, init->width, init->height,
		init->hParentWnd, NULL, init->hInstance, NULL);

	if (handle == NULL)
		app.fatalError(L"Не удалось создать окно списка объектов");

	DWORD extStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	SendMessage(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, extStyle, extStyle);

	LVCOLUMN lvc;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT;
	lvc.iSubItem = 0;
	lvc.pszText = (wchar_t*)L"Тип объекта";
	lvc.cx = 250;

	if (SendMessage(handle, LVM_INSERTCOLUMN, lvc.iSubItem, (LPARAM)&lvc) == -1)
		throw Exception(L"Не удалось создать колонку \"Тип объекта\"");

	lvc.iSubItem = 1;
	lvc.pszText = (wchar_t*)L"Количество";
	lvc.cx = 100;

	if (SendMessage(handle, LVM_INSERTCOLUMN, lvc.iSubItem, (LPARAM)&lvc) == -1)
		throw Exception(L"Не удалось создать колонку \"Количество\"");
}


ListView::~ListView()
{
}

void ListView::clear()
{
	SendMessage(handle, LVM_DELETEALLITEMS, 0, 0);
}

void ListView::update(map<wstring, int> const & data)
{
	clear();

	unsigned int index = 0;

	for (auto it = data.begin(); it != data.end(); it++)
	{
		LVITEM item;

		item.iItem = index;
		item.mask = LVIF_TEXT;
		item.iSubItem = 0;
		item.pszText = (wchar_t *)it->first.c_str();

		SendMessage(handle, LVM_INSERTITEM, 0, (LPARAM)&item);

		item.iSubItem = 1;
		item.pszText = (wchar_t *)to_wstring(it->second).c_str();

		SendMessage(handle, LVM_SETITEMTEXT, index, (LPARAM)&item);

		index += 1;
	}
}
