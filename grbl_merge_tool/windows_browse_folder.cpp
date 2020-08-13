#include "framework.h"


namespace windows
{

   int CALLBACK LibraryFrameBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

   bool browse_for_folder(::user::interaction * puiRequired, string & strFolder, string strInitialFolder, string strDefaultExtension, string strFilter)
   {

      browse_folder browsefolder(puiRequired->get_context_application());

      browsefolder.m_strDefaultExtension = strDefaultExtension;

      BROWSEINFO bi = { 0 };

      itemidlist pidlInitialFolder(strInitialFolder);

      if (strInitialFolder.has_char())
      {

         //bi.pidlRoot = pidlInitialFolder.detach();

      }

      bi.hwndOwner = puiRequired->get_safe_handle();

      bi.lParam = (LPARAM)&browsefolder;

      bi.ulFlags = BIF_USENEWUI | BIF_NONEWFOLDERBUTTON | BIF_VALIDATE;

      bi.lpfn = &LibraryFrameBrowseCallbackProc;

      itemidlist pidl(SHBrowseForFolder(&bi));

      strFolder = pidl.path();

      return true;

   }


   browse_folder::browse_folder(::aura::application * papp) :
      ::object(papp)
   {


   }
   browse_folder::~browse_folder()
   {


   }

   bool browse_folder::ValidateNewName(const char * pszNewName, string & strCandidateNewPath)
   {

      string & strNewPath = strCandidateNewPath;

      strNewPath = m_strSelPath;

      strNewPath.trim_right("/");

      strNewPath.trim_right("\\");

      strNewPath += "\\" + string(pszNewName);

      if (m_strDefaultExtension.has_char())
      {

         string strExt = "." + m_strDefaultExtension;

         if (!::str::ends_eat_ci(strNewPath, strExt))
         {

            strNewPath += strExt;

         }

      }

      if (Context.dir().is(strNewPath))
      {

         return false;

      }

      if (Context.file().exists(strNewPath))
      {

         return false;

      }

      m_strNewPath = strNewPath;

      return true;

   }


   int CALLBACK LibraryFrameBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
   {

      browse_folder * pframe = (browse_folder *)lpData;

      if (uMsg == BFFM_IUNKNOWN)
      {

         IUnknown * pUk = (IUnknown *)lParam;

         if (pUk != NULL)
         {

            comptr<IFolderFilterSite> pfilter;

            pUk->QueryInterface(&pfilter);

            if (pfilter.is_set())
            {

               IUnknown * pThis = (IUnknown *)pframe;

               pfilter->SetFilter(pThis);

            }

         }

      }
      else if (uMsg == BFFM_SELCHANGED)
      {

         LPITEMIDLIST pidl = (LPITEMIDLIST)lParam;

         pframe->m_strSelPath = itemidlist::_path(pidl);

      }
      else if (uMsg == BFFM_VALIDATEFAILED)
      {

         LPCTSTR lpcwszNewName = (LPCTSTR)lParam;

         string str;

         str = lpcwszNewName;

         string strCandidateNewPath;

         if (!pframe->ValidateNewName(str, strCandidateNewPath))
         {

            return 1;

         }

      }

      return 0;

   }


   HRESULT STDMETHODCALLTYPE browse_folder::QueryInterface(const IID & iid, void ** p)
   {

      if (iid == IID_IUnknown)
      {

         *p = (IUnknown *)this;
         return S_OK;
      }
      else if (iid == IID_IFolderFilter)
      {

         *p = (IFolderFilter *)this;
         return S_OK;
      }
      return E_FAIL;
   }


   ULONG STDMETHODCALLTYPE browse_folder::AddRef()
   {

      return add_ref();

   }


   ULONG STDMETHODCALLTYPE browse_folder::Release()
   {

      return release();

   }


   HRESULT browse_folder::GetEnumFlags(IShellFolder * psf, PCIDLIST_ABSOLUTE pidlFolder, HWND * phwnd, DWORD * pgrfFlags)
   {
      *pgrfFlags = SHCONTF_FOLDERS;

      return S_OK;
   }


   HRESULT browse_folder::ShouldShow(IShellFolder * psf, PCIDLIST_ABSOLUTE pidlFolder, PCUITEMID_CHILD pidlItem)
   {

      comptr < IShellItem > psi;

      HRESULT hr = SHCreateItemWithParent(NULL, psf, pidlItem, IID_PPV_ARGS(&psi));

      if (SUCCEEDED(hr))
      {
         // Determine here whether the item should be shown. This determination
         // is application-dependent.
         cotaskptr < LPWSTR > lpwstr;

         psi->GetDisplayName(SIGDN_FILESYSPATH, &lpwstr);

         string str(lpwstr);

      }

      return S_OK;
   }


} // namespace windows


