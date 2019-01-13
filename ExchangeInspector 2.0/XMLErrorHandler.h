#pragma once
#include <MsXml6.h>

class XMLErrorHandler :
	public ISAXErrorHandler
{
	unsigned long referenceCounter = 0;
public:
	XMLErrorHandler();
	~XMLErrorHandler();

	// Унаследовано через ISAXErrorHandler
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject) override;
	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall error(ISAXLocator * pLocator, const wchar_t * pwchErrorMessage, HRESULT hrErrorCode) override;
	virtual HRESULT __stdcall fatalError(ISAXLocator * pLocator, const wchar_t * pwchErrorMessage, HRESULT hrErrorCode) override;
	virtual HRESULT __stdcall ignorableWarning(ISAXLocator * pLocator, const wchar_t * pwchErrorMessage, HRESULT hrErrorCode) override;
};

