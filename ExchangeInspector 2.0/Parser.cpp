#include "Parser.h"
#include "Application.h"


Parser::Parser()
{
	HRESULT hr;

	CoInitialize(NULL);
	
	hr = CoCreateInstance(__uuidof(SAXXMLReader60), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (LPVOID*)&reader);

	if (hr != S_OK)
		throw Exception(L"�� ������� ������� ��������� ISAXXMLReader");

	hr = reader->putContentHandler(&contentHandler);

	if (hr != S_OK)
		throw Exception(L"�� ������� ������ ���������� ����������� XML");

	hr = reader->putErrorHandler(&errorHandler);

	if (hr != S_OK)
		throw Exception(L"�� ������� ������ ���������� ������ ������� XML");
}


Parser::~Parser()
{
	CoUninitialize();
}

DWORD __stdcall Parser::threadProc(void * parameter)
{
	Application & app = Application::getInstance();

	if (parameter != NULL)
	{
		app.setParsing(true);
		Parser * parser = (Parser*)parameter;
		parser->reader->parseURL(parser->url->c_str());
		app.setParsing(false);
	}

	return 0;
}

void Parser::run(wstring const & fileName)
{
	url = (std::wstring*)&fileName;
	
	CreateThread(NULL, NULL, threadProc, this, 0, &threadId);
}
