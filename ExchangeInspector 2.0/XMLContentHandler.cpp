#include "XMLContentHandler.h"
#include "Application.h"



XMLContentHandler::XMLContentHandler()
{
	docTypes = Application::getInstance().getList();
}


XMLContentHandler::~XMLContentHandler()
{
}

HRESULT __stdcall XMLContentHandler::QueryInterface(REFIID riid, void ** ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IUnknown*>(this);
	}
	else if (riid == __uuidof(ISAXContentHandler))
	{
		*ppvObject = static_cast<ISAXContentHandler*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG __stdcall XMLContentHandler::AddRef(void)
{
	return InterlockedIncrement(&referenceCounter);
}

ULONG __stdcall XMLContentHandler::Release(void)
{
	long counter = InterlockedDecrement(&referenceCounter);

	if (counter == 0) delete this;

	return counter;
}

HRESULT __stdcall XMLContentHandler::putDocumentLocator(ISAXLocator * pLocator)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::startDocument(void)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::endDocument(void)
{
	Application::getInstance().parsingDone();
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::startPrefixMapping(const wchar_t * pwchPrefix, int cchPrefix, const wchar_t * pwchUri, int cchUri)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::endPrefixMapping(const wchar_t * pwchPrefix, int cchPrefix)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::startElement(const wchar_t * pwchNamespaceUri, int cchNamespaceUri, const wchar_t * pwchLocalName, int cchLocalName, const wchar_t * pwchQName, int cchQName, ISAXAttributes * pAttributes)
{
	HRESULT r = S_OK;
	wstring localName(pwchLocalName, cchLocalName);
	Application & app = Application::getInstance();
	static unsigned long itemsGot = 0;

	if (localName == L"Объект")
	{
		wchar_t * docType = NULL;
		int cchDocType = 0;

		r = pAttributes->getValueFromName(pwchNamespaceUri, cchNamespaceUri, L"ИмяПравила", 10, (const wchar_t**)&docType, &cchDocType);

		if (FAILED(r))
		{
			if (r != E_INVALIDARG)
				return r;
			else
				return S_OK;
		}

		wstring typeName(docType, cchDocType);

		auto it = docTypes->find(typeName);

		if (it == docTypes->end())
		{
			pair<wstring, int> p;
			p.first = typeName;
			p.second = 0;

			docTypes->insert(p);
			itemsGot++;
		}
		else
		{
			docTypes->at(typeName)++;
			itemsGot++;
		}

		if (itemsGot == 500)
		{
			app.refreshList();
			itemsGot = 0;
		}
	}

	return r;
}

HRESULT __stdcall XMLContentHandler::endElement(const wchar_t * pwchNamespaceUri, int cchNamespaceUri, const wchar_t * pwchLocalName, int cchLocalName, const wchar_t * pwchQName, int cchQName)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::characters(const wchar_t * pwchChars, int cchChars)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::ignorableWhitespace(const wchar_t * pwchChars, int cchChars)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::processingInstruction(const wchar_t * pwchTarget, int cchTarget, const wchar_t * pwchData, int cchData)
{
	return S_OK;
}

HRESULT __stdcall XMLContentHandler::skippedEntity(const wchar_t * pwchName, int cchName)
{
	return S_OK;
}
