#pragma once
#include <MsXml6.h>
#include <string>
#include <map>

using namespace std;

class XMLContentHandler :
	public ISAXContentHandler
{
	unsigned long referenceCounter = 0;
	map<wstring, int> * docTypes = NULL;
public:
	XMLContentHandler();
	~XMLContentHandler();

	// Унаследовано через ISAXContentHandler
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject) override;
	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall putDocumentLocator(ISAXLocator * pLocator) override;
	virtual HRESULT __stdcall startDocument(void) override;
	virtual HRESULT __stdcall endDocument(void) override;
	virtual HRESULT __stdcall startPrefixMapping(const wchar_t * pwchPrefix, int cchPrefix, const wchar_t * pwchUri, int cchUri) override;
	virtual HRESULT __stdcall endPrefixMapping(const wchar_t * pwchPrefix, int cchPrefix) override;
	virtual HRESULT __stdcall startElement(const wchar_t * pwchNamespaceUri, int cchNamespaceUri, const wchar_t * pwchLocalName, int cchLocalName, const wchar_t * pwchQName, int cchQName, ISAXAttributes * pAttributes) override;
	virtual HRESULT __stdcall endElement(const wchar_t * pwchNamespaceUri, int cchNamespaceUri, const wchar_t * pwchLocalName, int cchLocalName, const wchar_t * pwchQName, int cchQName) override;
	virtual HRESULT __stdcall characters(const wchar_t * pwchChars, int cchChars) override;
	virtual HRESULT __stdcall ignorableWhitespace(const wchar_t * pwchChars, int cchChars) override;
	virtual HRESULT __stdcall processingInstruction(const wchar_t * pwchTarget, int cchTarget, const wchar_t * pwchData, int cchData) override;
	virtual HRESULT __stdcall skippedEntity(const wchar_t * pwchName, int cchName) override;
};

