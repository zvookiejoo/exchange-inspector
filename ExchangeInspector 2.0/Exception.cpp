#include "Exception.h"



Exception::Exception()
{
}

Exception::Exception(const wchar_t * _message)
{
	message = _message;
}

Exception::~Exception()
{
}

const wstring & Exception::what()
{
	return message;
}
