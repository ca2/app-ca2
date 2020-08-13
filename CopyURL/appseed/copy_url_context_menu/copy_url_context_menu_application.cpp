#include "framework.h"
#pragma comment(lib, "shlwapi.lib")

extern HINSTANCE g_hInst;

extern long g_cDllRef;

#define IDM_COPY_URL             0

namespace copy_url_context_menu
{



   application::application():
      m_pszMenuText(L"&Copy URL"),
      m_pszVerb("application"),
      m_pwszVerb(L"application"),
      m_pszVerbCanonicalName("application"),
      m_pwszVerbCanonicalName(L"application"),
      m_pszVerbHelpText("Copy URL Path"),
      m_pwszVerbHelpText(L"Copy URL Path")
   {

      m_hwnd = NULL;

      InterlockedIncrement(&g_cDllRef);

   }


   application::~application()
   {

      if (m_hwnd)
      {
         DestroyWindow(m_hwnd);
      }
      InterlockedDecrement(&g_cDllRef);

   }


   void application::OnVerbCopyURL()
   {

      WCHAR wstr1[8192];

      WCHAR wstr2[8192];

      UNIVERSAL_NAME_INFOW * pinfo;

      pinfo = (UNIVERSAL_NAME_INFOW *)&wstr1;

      DWORD dw = sizeof(wstr1);

      int err = WNetGetUniversalNameW(m_szSelectedFile, UNIVERSAL_NAME_INFO_LEVEL, pinfo, &dw);

      if (err == NO_ERROR)
      {

         pinfo->lpUniversalName[dw / sizeof(WCHAR)] = L'\0';

         dw = 8192;

         UrlCreateFromPathW(pinfo->lpUniversalName, wstr2, &dw, NULL);

      }
      else
      {

         dw = 8192;

         UrlCreateFromPathW(m_szSelectedFile, wstr2, &dw, NULL);

      }

      wstr2[dw] = L'\0';

      if (!::OpenClipboard(m_hwnd))
      {

         return;

      }

      EmptyClipboard();

      int iCount = lstrlenW(wstr2) + 1;
      HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, iCount * sizeof(WCHAR));
      wchar_t * lpwstrCopy = (wchar_t *) ::GlobalLock(hglbCopy);
      wcsncpy(lpwstrCopy, wstr2, iCount);
      ::GlobalUnlock(hglbCopy);

      iCount = WideCharToMultiByte(CP_UTF8, 0, wstr2, lstrlenW(wstr2), NULL, 0, NULL, NULL);
      HGLOBAL hglbCopy2 = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(CHAR) * (iCount + 1));
      char * lpstrCopy = (char *) ::GlobalLock(hglbCopy2);
      WideCharToMultiByte(CP_UTF8, 0, wstr2, lstrlenW(wstr2), lpstrCopy, sizeof(CHAR) * (iCount + 1), NULL, NULL);
      lpstrCopy[iCount] = '\0';
      ::GlobalUnlock(hglbCopy2);

      SetClipboardData(CF_UNICODETEXT, hglbCopy);

      SetClipboardData(CF_TEXT, hglbCopy2);

      ::CloseClipboard();


   }


// Query to the interface the component supported.
   IFACEMETHODIMP application::QueryInterface(REFIID riid, void **ppv)
   {
      static const QITAB qit[] =
      {
         QITABENT(application, IContextMenu),
         QITABENT(application, IShellExtInit),
         { 0 },
      };
      return QISearch(this, qit, riid, ppv);
   }

// Increase the context_object count for an interface on an object.
   IFACEMETHODIMP_(ULONG) application::AddRef()
   {
      return InterlockedIncrement(&m_cRef);
   }

// Decrease the context_object count for an interface on an object.
   IFACEMETHODIMP_(ULONG) application::Release()
   {
      return InterlockedDecrement(&m_cRef);
   }


// Initialize the context menu handler.
   IFACEMETHODIMP application::Initialize(
   LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
   {
      if (NULL == pDataObj)
      {

         return E_INVALIDARG;

      }

      if (m_hwnd == NULL)
      {

         WNDCLASS wndcls = {};

         char strClass[] = "copy_url_context_menu_window";

         if (!GetClassInfo(g_hInst, strClass, &wndcls))
         {

            wndcls.style = 0;
            wndcls.lpfnWndProc = DefWindowProc;
            wndcls.cbClsExtra = 0;
            wndcls.cbWndExtra = 0;
            wndcls.hInstance = g_hInst;
            wndcls.hIcon = NULL;
            wndcls.hCursor = NULL;
            wndcls.hbrBackground = NULL;
            wndcls.lpszMenuName = NULL;
            wndcls.lpszClassName = strClass;
            if (!::RegisterClass(&wndcls))
            {
               return false;
            }

         }

         m_hwnd = ::CreateWindowEx(0, strClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, 0, NULL);

      }

      HRESULT hr = E_FAIL;

      if (m_hwnd == NULL)
      {

         return hr;

      }

      FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
      STGMEDIUM stm;

      // The pDataObj pointer contains the objects being acted upon. In this
      // example, we get an HDROP handle for enumerating the selected files and
      // folders.
      if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
      {
         // Get an HDROP handle.
         HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
         if (hDrop != NULL)
         {
            // Determine how many files are involved in this operation. This
            // code sample displays the custom context menu item when only
            // one file is selected.
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
            if (nFiles == 1)
            {
               // Get the path of the file.
               if (0 != DragQueryFileW(hDrop, 0, m_szSelectedFile,
                                       ARRAYSIZE(m_szSelectedFile)))
               {
                  hr = S_OK;
               }
            }

            GlobalUnlock(stm.hGlobal);
         }

         ReleaseStgMedium(&stm);
      }

      // If any value other than S_OK is returned from the method, the context
      // menu item is not displayed.
      return hr;
   }


   IFACEMETHODIMP application::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
   {


      if (CMF_DEFAULTONLY & uFlags)
      {

         return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));

      }

      MENUITEMINFOW mii = { sizeof(mii) };
      mii.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_STATE;
      mii.wID = idCmdFirst + IDM_COPY_URL;
      mii.fType = MFT_STRING;
      mii.dwTypeData = m_pszMenuText;
      mii.fState = MFS_ENABLED;
      if (!InsertMenuItemW(hMenu, indexMenu, TRUE, &mii))
      {
         return HRESULT_FROM_WIN32(GetLastError());
      }

      MENUITEMINFO sep = { sizeof(sep) };
      sep.fMask = MIIM_TYPE;
      sep.fType = MFT_SEPARATOR;
      if (!InsertMenuItem(hMenu, indexMenu + 1, TRUE, &sep))
      {
         return HRESULT_FROM_WIN32(GetLastError());
      }

      return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_COPY_URL + 1));

   }


   IFACEMETHODIMP application::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
   {

      BOOL fUnicode = FALSE;

      if (pici->cbSize == sizeof(CMINVOKECOMMANDINFOEX))
      {

         if (pici->fMask & CMIC_MASK_UNICODE)
         {

            fUnicode = TRUE;

         }

      }

      if (!fUnicode && HIWORD(pici->lpVerb))
      {

         if (StrCmpIA(pici->lpVerb, m_pszVerb) == 0)
         {

            OnVerbCopyURL();

         }
         else
         {

            return E_FAIL;

         }

      }
      else if (fUnicode && HIWORD(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW))
      {

         if (StrCmpIW(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW, m_pwszVerb) == 0)
         {

            OnVerbCopyURL();

         }
         else
         {

            return E_FAIL;

         }

      }
      else
      {

         if (LOWORD(pici->lpVerb) == IDM_COPY_URL)
         {

            OnVerbCopyURL();

         }
         else
         {

            return E_FAIL;

         }

      }

      return S_OK;

   }


   IFACEMETHODIMP application::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
   {

      HRESULT hr = E_INVALIDARG;

      if (idCommand == IDM_COPY_URL)
      {

         switch (uFlags)
         {
         case GCS_HELPTEXTW:
         {

            hr = StringCchCopyW(reinterpret_cast<PWSTR>(pszName), cchMax, m_pwszVerbHelpText);

         }
         break;

         case GCS_VERBW:
         {

            hr = StringCchCopyW(reinterpret_cast<PWSTR>(pszName), cchMax, m_pwszVerbCanonicalName);

         }
         break;

         case GCS_HELPTEXTA:
         {

            hr = StringCchCopyA(pszName, cchMax, m_pszVerbHelpText);

         }
         break;

         case GCS_VERBA:
         {

            hr = StringCchCopyA(pszName, cchMax, m_pszVerbCanonicalName);

         }
         break;
         default:
         {

         }

         break;

         }

      }

      return hr;

   }


} // namespace copy_url_context_menu





