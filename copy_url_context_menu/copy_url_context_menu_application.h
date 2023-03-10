#pragma once



namespace copy_url_context_menu
{


   class application :
      virtual public ::aura::system,
      public IShellExtInit,
      public IContextMenu
   {
   public:
      HWND     m_hwnd;


      PWSTR m_pszMenuText;
      HANDLE m_hMenuBmp;
      PCSTR m_pszVerb;
      PCWSTR m_pwszVerb;
      PCSTR m_pszVerbCanonicalName;
      PCWSTR m_pwszVerbCanonicalName;
      PCSTR m_pszVerbHelpText;
      PCWSTR m_pwszVerbHelpText;

      // The name of the selected file.
      wchar_t m_szSelectedFile[MAX_PATH * 8];

      static application * g_papp;
      long m_cRef;

      application();
      virtual ~application();




      // The method that handles the "display" verb.
      void OnVerbDisplayFileName(HWND hWnd);




      // IUnknown
      IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
      IFACEMETHODIMP_(ULONG) AddRef();
      IFACEMETHODIMP_(ULONG) Release();

      // IShellExtInit
      IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

      // IContextMenu
      IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
      IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
      IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);

   };


} // namespace copy_url_context_menu



