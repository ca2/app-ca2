#pragma once



namespace copy_url_context_menu
{


   class application :
      public IShellExtInit,
      public IContextMenu
   {
   public:

      HWND        m_hwnd;
      LPWSTR      m_pszMenuText;
      LPCSTR      m_pszVerb;
      LPCWSTR     m_pwszVerb;
      LPCSTR      m_pszVerbCanonicalName;
      LPCWSTR     m_pwszVerbCanonicalName;
      LPCSTR      m_pszVerbHelpText;
      LPCWSTR     m_pwszVerbHelpText;

      wchar_t     m_szSelectedFile[MAX_PATH * 8];

      long m_cRef;

      application();
      virtual ~application();


      void OnVerbCopyURL();


      /// IUnknown
      IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
      IFACEMETHODIMP_(ULONG) AddRef();
      IFACEMETHODIMP_(ULONG) Release();

      /// IShellExtInit
      IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

      /// IContextMenu
      IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
      IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
      IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);

   };


} // namespace copy_url_context_menu



