#pragma once


namespace quasar_science_update_utility
{


   class dialog1 :
      virtual public child_dialog
   {
   public:

      ::win32::window      m_checkVersion;
      ::win32::window      m_versionStatus;
      ::win32::window      m_download;
      ::win32::window      m_downloadProgress;
      ::win32::window      m_browse;
      ::win32::window      m_editBrowse;
      ::win32::window      m_radioDownload;
      ::win32::window      m_radioInstallVersion;
      ::win32::window      m_radioBrowse;
      ::win32::window      m_st3;

      ::win32::window      m_comboLang;



      //::file::path m_pathBrowse;

      //string m_strDownload;
      //string m_strVersion;
      //string m_strUrl;
      //::file::path m_pathDownload;

      dialog1(::aura::application * papp);
      virtual ~dialog1();

      void on_notify(NMHDR * p);
      virtual bool on_init_dialog();

      void on_command(UINT nID);
      void on_notify(int iCode, int iId);
   };


} // namespace quasar_science_update_utility



