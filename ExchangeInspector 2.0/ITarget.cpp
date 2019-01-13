#include "ITarget.h"
#include "Application.h"


ITarget::ITarget()
	: nRefCount(0)
{
}


ITarget::~ITarget()
{
}

HRESULT __stdcall ITarget::QueryInterface(REFIID riid, void ** ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IUnknown*>(this);
	}
	else if (riid == IID_IDropTarget)
	{
		*ppvObject = static_cast<IDropTarget*>(this);
	}
	else
	{
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	AddRef();

	return S_OK;
}

ULONG __stdcall ITarget::AddRef(void)
{
	return InterlockedIncrement(&nRefCount);
}

ULONG __stdcall ITarget::Release(void)
{
	long refCount = InterlockedDecrement(&nRefCount);

	if (refCount == 0) delete this;

	return refCount;
}

HRESULT __stdcall ITarget::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	*pdwEffect = DROPEFFECT_COPY;

	return S_OK;
}

HRESULT __stdcall ITarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	return S_OK;
}

HRESULT __stdcall ITarget::DragLeave()
{
	return S_OK;
}

HRESULT __stdcall ITarget::Drop(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	IEnumFORMATETC * pFormatEnum = NULL;

	HRESULT r = pDataObject->EnumFormatEtc(DATADIR_GET, &pFormatEnum);

	bool fileFound = false;

	if (r == S_OK)
	{
		FORMATETC format;
		STGMEDIUM storage;

		while ((pFormatEnum->Next(1, &format, NULL)) == S_OK)
		{
			if ((format.cfFormat == CF_HDROP) && (!fileFound))
			{
				Application & app = Application::getInstance();

				r = pDataObject->GetData(&format, &storage);

				if (r != S_OK)
				{
					app.error(L"Не удалось получить данные объекта");
					return E_ABORT;
				}

				HDROP hDrop = (HDROP)storage.hGlobal;

				if (DragQueryFile(hDrop, -1, NULL, 0) != 1)
				{
					app.error(L"Приложение принимает по одному файлу");
					return E_ABORT;
				}

				size_t nameLength = DragQueryFile(hDrop, 0, NULL, 0);

				if (nameLength == 0)
				{
					app.error(L"Нулевая длина имени файла");
					return E_ABORT;
				}

				nameLength += 1;

				wchar_t * name = new wchar_t[nameLength];

				DragQueryFile(hDrop, 0, name, nameLength);

				fileFound = true;

				try
				{
					app.processFile(name);
				}
				catch (Exception e)
				{
					fileFound = false;
					app.clearData();
					app.error(e.what());
				}
			}
		}
	}

	return S_OK;
}
