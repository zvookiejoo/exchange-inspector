#include "XMLErrorHandler.h"
#include "Application.h"


XMLErrorHandler::XMLErrorHandler()
{
}


XMLErrorHandler::~XMLErrorHandler()
{
}

HRESULT __stdcall XMLErrorHandler::QueryInterface(REFIID riid, void ** ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IUnknown*>(this);
	}
	else if (riid == __uuidof(ISAXErrorHandler))
	{
		*ppvObject = static_cast<ISAXErrorHandler*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG __stdcall XMLErrorHandler::AddRef(void)
{
	return InterlockedIncrement(&referenceCounter);
}

ULONG __stdcall XMLErrorHandler::Release(void)
{
	long counter = InterlockedDecrement(&referenceCounter);

	if (counter == 0) delete this;

	return counter;
}

HRESULT __stdcall XMLErrorHandler::error(ISAXLocator * pLocator, const wchar_t * pwchErrorMessage, HRESULT hrErrorCode)
{
	Application::getInstance().error(pwchErrorMessage);
	return S_OK;
}

HRESULT __stdcall XMLErrorHandler::fatalError(ISAXLocator * pLocator, const wchar_t * pwchErrorMessage, HRESULT hrErrorCode)
{
	Application::getInstance().fatalError(pwchErrorMessage);
	return S_OK;
}

HRESULT __stdcall XMLErrorHandler::ignorableWarning(ISAXLocator * pLocator, const wchar_t * pwchErrorMessage, HRESULT hrErrorCode)
{
	Application::getInstance().warning(pwchErrorMessage);
	return S_OK;
}
