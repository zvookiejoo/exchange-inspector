#pragma once
#include <exception>
#include <string>

using namespace std;

class Exception :
	public exception
{
	wstring message;

public:
	Exception();
	Exception(const wchar_t * message);
	~Exception();

	const wstring & what();
};

