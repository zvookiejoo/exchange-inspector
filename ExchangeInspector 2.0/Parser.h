#pragma once
#include <Windows.h>
#include <string>
#include "XMLContentHandler.h"
#include "XMLErrorHandler.h"
#include "Exception.h"

class Parser
{
	XMLContentHandler contentHandler;
	XMLErrorHandler errorHandler;

	ISAXXMLReader * reader = NULL;

	wstring * url = NULL;

	DWORD threadId = NULL;

	static DWORD __stdcall threadProc(void * parameter);
public:
	Parser();
	~Parser();
	
	void run(wstring const & fileName);
};

