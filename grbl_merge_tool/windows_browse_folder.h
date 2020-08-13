#pragma once


namespace windows
{


   class browse_folder :
      virtual public ::object,
      virtual public IFolderFilter
   {
   public:

      string         m_strNewPath;
      string         m_strSelPath;

      // without prefixing dot
      string         m_strDefaultExtension;


      browse_folder(::aura::application * papp);
      virtual ~browse_folder();

      bool ValidateNewName(const char * psz, string & strCandidateNewPath);
      virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &, void **);
      virtual ULONG STDMETHODCALLTYPE AddRef();
      virtual ULONG STDMETHODCALLTYPE Release();
      virtual HRESULT STDMETHODCALLTYPE GetEnumFlags(IShellFolder * psf, PCIDLIST_ABSOLUTE pidlFolder, HWND * phwnd, DWORD * pgrfFlags);
      virtual HRESULT STDMETHODCALLTYPE ShouldShow(IShellFolder * psf, PCIDLIST_ABSOLUTE pidlFolder, PCUITEMID_CHILD pidlItem);

   };

   bool browse_for_folder(::user::interaction * puiRequired, string & strFolder,  string strInitialFolder, string strDefaultExtension = "", string strFilter = "*");


} // namespace windows