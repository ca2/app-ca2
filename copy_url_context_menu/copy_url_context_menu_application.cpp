#include "framework.h"
#pragma comment(lib, "shlwapi.lib")

extern HINSTANCE g_hInst;
extern long g_cDllRef;

#define IDM_DISPLAY             0  // The command's identifier offset



namespace copy_url_context_menu
{


   application * application::g_papp = NULL;


   application::application():
      ::aura::system(NULL, NULL),
      m_pszMenuText(L"&Copy URL"),
      m_pszVerb("application"),
      m_pwszVerb(L"application"),
      m_pszVerbCanonicalName("application"),
      m_pwszVerbCanonicalName(L"application"),
      m_pszVerbHelpText("Copy URL Path"),
      m_pwszVerbHelpText(L"Copy URL Path")
   {

      InterlockedIncrement(&g_cDllRef);

      g_papp = this;


   }


   application::~application()
   {

      if (m_hMenuBmp)
      {
         DeleteObject(m_hMenuBmp);
         m_hMenuBmp = NULL;
      }
      if (m_hwnd)
      {
         DestroyWindow(m_hwnd);
      }
      InterlockedDecrement(&g_cDllRef);

   }








   void application::OnVerbDisplayFileName(HWND hWnd)
   {

      string strUrl;

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

      strUrl = wstr2;

      wchar_t szMessage[300];

#ifdef _DEBUG

      if (SUCCEEDED(StringCchPrintfW(szMessage, ARRAYSIZE(szMessage), L"Generated URL is:\r\n\r\n%s", wstr2)))
      {

         MessageBoxW(hWnd, szMessage, L"application", MB_OK);

      }

#endif

      ASSERT(::IsWindow(m_hwnd));

      string str(strUrl);

      ASSERT(::IsWindow(m_hwnd));

      if (!::OpenClipboard(m_hwnd))
      {

         return;

      }

      EmptyClipboard();

      ::count iCount = ::str::international::utf8_to_unicode_count(str) + 1;
      HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, iCount * sizeof(WCHAR));
      unichar * lpwstrCopy = (unichar *) ::GlobalLock(hglbCopy);
      ::str::international::utf8_to_unicode(lpwstrCopy, iCount, str);
      ::GlobalUnlock(hglbCopy);

      HGLOBAL hglbCopy2 = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(CHAR) * (str.length() + 1));
      char * lpstrCopy = (char *) ::GlobalLock(hglbCopy2);
      strcpy(lpstrCopy, str);
      ::GlobalUnlock(hglbCopy2);


      SetClipboardData(CF_UNICODETEXT, hglbCopy);
      SetClipboardData(CF_TEXT, hglbCopy2);
      VERIFY(::CloseClipboard());


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
      return add_ref();
   }

// Decrease the context_object count for an interface on an object.
   IFACEMETHODIMP_(ULONG) application::Release()
   {
      return release();
   }


// Initialize the context menu handler.
   IFACEMETHODIMP application::Initialize(
   LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
   {
      if (NULL == pDataObj)
      {
         return E_INVALIDARG;
      }

      if (m_hMenuBmp == NULL)
      {

         memory_file file(this);

         read_resource_as_memory_dup(*file.get_primitive_memory(), g_hInst, IDB_OK, "PNG");

         ::visual::dib_pointer dib(allocer());

         dib.load_from_file(&file, false);

         m_hMenuBmp = dib->get_bitmap()->_GetHBITMAP();


      }

      if (m_hwnd == NULL)
      {

         WNDCLASS wndcls = {};

         string strClass = "copy_url_context_menu_window";

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


//
//   FUNCTION: application::QueryContextMenu
//
//   PURPOSE: The Shell calls IContextMenu::QueryContextMenu to allow the
//            context menu handler to add its menu items to the menu. It
//            passes in the HMENU handle in the hmenu parameter. The
//            indexMenu parameter is set to the index to be used for the
//            first menu item that is to be added.
//
   IFACEMETHODIMP application::QueryContextMenu(
   HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
   {
      // If uFlags include CMF_DEFAULTONLY then we should not do anything.
      if (CMF_DEFAULTONLY & uFlags)
      {
         return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
      }

      // Use either InsertMenu or InsertMenuItem to add menu items.
      // Learn how to add sub-menu from:
      // http://www.codeproject.com/KB/shell/ctxextsubmenu.aspx

      MENUITEMINFOW mii = { sizeof(mii) };
      mii.fMask = MIIM_BITMAP | MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_STATE;
      mii.wID = idCmdFirst + IDM_DISPLAY;
      mii.fType = MFT_STRING;
      mii.dwTypeData = m_pszMenuText;
      mii.fState = MFS_ENABLED;
      mii.hbmpItem = static_cast<HBITMAP>(m_hMenuBmp);
      if (!InsertMenuItemW(hMenu, indexMenu, TRUE, &mii))
      {
         return HRESULT_FROM_WIN32(GetLastError());
      }

      // Add a separator.
      MENUITEMINFO sep = { sizeof(sep) };
      sep.fMask = MIIM_TYPE;
      sep.fType = MFT_SEPARATOR;
      if (!InsertMenuItem(hMenu, indexMenu + 1, TRUE, &sep))
      {
         return HRESULT_FROM_WIN32(GetLastError());
      }

      // Return an HRESULT value with the severity set to SEVERITY_SUCCESS.
      // Set the code value to the offset of the largest command identifier
      // that was assigned, plus one (1).
      return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_DISPLAY + 1));
   }


//
//   FUNCTION: application::InvokeCommand
//
//   PURPOSE: This method is called when a user clicks a menu item to tell
//            the handler to run the associated command. The lpcmi parameter
//            points to a structure that contains the needed information.
//
   IFACEMETHODIMP application::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
   {

      BOOL fUnicode = FALSE;

      // Determine which structure is being passed in, CMINVOKECOMMANDINFO or
      // CMINVOKECOMMANDINFOEX based on the cbSize member of lpcmi. Although
      // the lpcmi parameter is declared in Shlobj.h as a CMINVOKECOMMANDINFO
      // structure, in practice it often points to a CMINVOKECOMMANDINFOEX
      // structure. This struct is an extended version of CMINVOKECOMMANDINFO
      // and has additional members that allow Unicode strings to be passed.
      if (pici->cbSize == sizeof(CMINVOKECOMMANDINFOEX))
      {

         if (pici->fMask & CMIC_MASK_UNICODE)
         {

            fUnicode = TRUE;

         }

      }

      // Determines whether the command is identified by its offset or verb.
      // There are two ways to identify commands:
      //
      //   1) The command's verb string
      //   2) The command's identifier offset
      //
      // If the high-order word of lpcmi->lpVerb (for the ANSI case) or
      // lpcmi->lpVerbW (for the Unicode case) is nonzero, lpVerb or lpVerbW
      // holds a verb string. If the high-order word is zero, the command
      // offset is in the low-order word of lpcmi->lpVerb.

      // For the ANSI case, if the high-order word is not zero, the command's
      // verb string is in lpcmi->lpVerb.
      if (!fUnicode && HIWORD(pici->lpVerb))
      {

         // Is the verb supported by this context menu extension?
         if (StrCmpIA(pici->lpVerb, m_pszVerb) == 0)
         {

            OnVerbDisplayFileName(pici->hwnd);

         }
         else
         {

            // If the verb is not recognized by the context menu handler, it
            // must return E_FAIL to allow it to be passed on to the other
            // context menu handlers that might implement that verb.
            return E_FAIL;

         }

      }
      // For the Unicode case, if the high-order word is not zero, the
      // command's verb string is in lpcmi->lpVerbW.
      else if (fUnicode && HIWORD(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW))
      {

         // Is the verb supported by this context menu extension?
         if (StrCmpIW(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW, m_pwszVerb) == 0)
         {

            OnVerbDisplayFileName(pici->hwnd);

         }
         else
         {

            // If the verb is not recognized by the context menu handler, it
            // must return E_FAIL to allow it to be passed on to the other
            // context menu handlers that might implement that verb.
            return E_FAIL;

         }

      }
      // If the command cannot be identified through the verb string, then
      // check the identifier offset.
      else
      {

         // Is the command identifier offset supported by this context menu
         // extension?
         if (LOWORD(pici->lpVerb) == IDM_DISPLAY)
         {

            OnVerbDisplayFileName(pici->hwnd);

         }
         else
         {

            // If the verb is not recognized by the context menu handler, it
            // must return E_FAIL to allow it to be passed on to the other
            // context menu handlers that might implement that verb.

            return E_FAIL;

         }

      }

      return S_OK;

   }


//
//   FUNCTION: application::GetCommandString
//
//   PURPOSE: If a user highlights one of the items added by a context menu
//            handler, the handler's IContextMenu::GetCommandString method is
//            called to request a Help text string that will be displayed on
//            the Windows Explorer status bar. This method can also be called
//            to request the verb string that is assigned to a command.
//            Either ANSI or Unicode verb strings can be requested. This
//            example only implements support for the Unicode values of
//            uFlags, because only those have been used in Windows Explorer
//            since Windows 2000.
//
   IFACEMETHODIMP application::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
   {

      HRESULT hr = E_INVALIDARG;

      if (idCommand == IDM_DISPLAY)
      {

         switch (uFlags)
         {
         case GCS_HELPTEXTW:
         {

            // Only useful for pre-Vista versions of Windows that have a
            // Status bar.

            hr = StringCchCopyW(reinterpret_cast<PWSTR>(pszName), cchMax, m_pwszVerbHelpText);

         }
         break;

         case GCS_VERBW:
         {
            // GCS_VERBW is an optional feature that enables a caller to
            // discover the canonical name for the verb passed in through
            // idCommand.

            hr = StringCchCopyW(reinterpret_cast<PWSTR>(pszName), cchMax, m_pwszVerbCanonicalName);

         }
         break;

         case GCS_HELPTEXTA:
         {
            // Only useful for pre-Vista versions of Windows that have a
            // Status bar.

            hr = StringCchCopyA(pszName, cchMax, m_pszVerbHelpText);

         }
         break;

         case GCS_VERBA:
            // GCS_VERBW is an optional feature that enables a caller to
            // discover the canonical name for the verb passed in through
            // idCommand.
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

      // If the command (idCommand) is not supported by this context menu
      // extension handler, return E_INVALIDARG.

      return hr;

   }




} // namespace copy_url_context_menu




extern "C"
::aura::library * app_ca2_grbl_merge_tool_get_new_library(::aura::application * papp)
{

   return canew(::aura::single_application_library < ::copy_url_context_menu::application >(papp, "app-ca2/copy_url_context_menu"));

}


::aura::application * get_acid_app(::aura::application * papp)
{

   return canew(::copy_url_context_menu::application());

}

