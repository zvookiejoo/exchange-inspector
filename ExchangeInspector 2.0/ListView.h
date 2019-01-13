#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <map>
#include <string>

using namespace std;

typedef struct _ListViewInit
{
	HWND hParentWnd;
	HINSTANCE hInstance;
	int x, y, width, height;
} ListViewInit, * PListViewInit;

class ListView
{
	HWND handle;
	int x, y, width, height;
public:
	ListView(const PListViewInit init);
	~ListView();

	void clear();
	void update(map<wstring, int> const & data);
	void resize(const RECT * rect);
};

