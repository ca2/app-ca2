#include "framework.h"


namespace grbl_merge_tool
{


   dialog1::dialog1(::aura::application * papp) :
      ::object(papp),
      ::win32::dialog(papp, MAKEINTRESOURCE(IDD_DIALOG1))
   {


   }


   dialog1::~dialog1()
   {

   }

   void dialog1::on_notify(int iCode, int iId)
   {

      if (iCode == EN_CHANGE && iId == IDC_EDIT_BROWSE)
      {

         m_pparent->m_pmaininterface->on_browse(m_editBrowse.get_window_text());

         //m_pathBrowse = m_editBrowse.get_window_text();

         //if (Context.file().exists(m_pathBrowse))
         //{

         //   Application.m_pathSend = m_pathBrowse;
         //   m_pparent->m_update.enable_window(true);

         //}
         //else
         //{
         //   Application.m_pathSend.Empty();
         //   m_pparent->m_update.enable_window(false);
         //}

      }

   }
   void dialog1::on_notify(NMHDR * p)
   {


   }
   bool dialog1::on_init_dialog()
   {

      child_dialog::on_init_dialog();

      //SetWindowLongPtr(h, GWL_USERDATA, lparam);

      set_dlg_item_font(IDC_TITLE1, m_pparent->m_fTitle1);

      set_dlg_item_font(IDC_DESCRIPTION, m_pparent->m_fText1);

      set_dlg_item_text(IDC_DESCRIPTION, load_resource_string(IDS_DESCRIPTION1));

      get_dlg_item(m_st3, IDC_STATIC3);

      m_st3.set_font(m_pparent->m_fText1);

      {

         set_dlg_item_font(IDC_RADIO_DOWNLOAD, m_pparent->m_fText2);

         get_dlg_item(m_radioDownload, IDC_RADIO_DOWNLOAD);

         get_dlg_item(m_checkVersion, IDC_CHECK_VERSION);

         m_checkVersion.set_font(m_pparent->m_fText2);

         get_dlg_item(m_versionStatus, IDC_VERSION_STATUS);

         m_versionStatus.set_font(m_pparent->m_fText2);

         get_dlg_item(m_download, IDC_DOWNLOAD);

         m_download.set_font(m_pparent->m_fText2);

         get_dlg_item(m_downloadProgress, IDC_DOWNLOAD_PROGRESS);

         m_downloadProgress.set_font(m_pparent->m_fText2);

         set_dlg_item_font(IDC_UPDATE, m_pparent->m_fText2);

         set_dlg_item_font(IDCANCEL, m_pparent->m_fText2);

         set_dlg_item_font(IDC_RADIO_LATEST, m_pparent->m_fText2);

         get_dlg_item(m_radioInstallVersion, IDC_RADIO_LATEST);

         get_dlg_item(m_radioBrowse, IDC_RADIO_BROWSE);

         set_dlg_item_font(IDC_RADIO_BROWSE, m_pparent->m_fText2);

         set_dlg_item_font(IDC_BROWSE, m_pparent->m_fText2);

         get_dlg_item(m_browse, IDC_BROWSE);

         m_browse.set_font(m_pparent->m_fText2);

         get_dlg_item(m_editBrowse, IDC_EDIT_BROWSE);

         m_editBrowse.set_font(m_pparent->m_fText2);

         ::win32::window radioDownload;

         get_dlg_item(radioDownload, IDC_RADIO_DOWNLOAD);

         radioDownload.send_message(BM_SETCHECK, BST_CHECKED);

         on_command(IDC_RADIO_DOWNLOAD);

      }

      Application.check_downloaded_bin();

      return true;

   }

   void dialog1::on_command(UINT nID)
   {

      if (nID == IDC_CHECK_VERSION)
      {

         m_pparent->m_update.enable_window(false);

         m_pparent->m_pmaininterface->check_version();

      }
      else if (nID == IDC_DOWNLOAD)
      {


         m_pparent->m_pmaininterface->download();

         //         memory m;
         //
         //         property_set set;
         //
         //         set["raw_http"] = true;
         //         set["disable_common_name_cert_check"] = true;
         //         set["app"] = get_context_application();
         //
         //         Context.http().get(m_strUrl, m, set);
         //
         //         m_pathDownload = Context.file().time_square();
         //
         //         Context.file().put_contents(m_pathDownload, m);
         //
         //         if (m.get_size() > 0 && Context.file().exists(m_pathDownload))
         //         {
         //
         //            m_pathSend = m_pathDownload;
         //
         //            on_download_response("100%", true);
         //
         //            //m_downloadProgress.set_window_text("100%");
         ////            m_pparent->m_update.enable_window(true);
         //
         //         }
         //         else
         //         {
         //
         //
         //            m_pathSend = m_pathDownload;
         //            m_pathDownload.Empty();
         //            m_pathSend.Empty();
         //
         //            on_download_response("Error.", false);
         //
         ////            m_pparent->m_update.enable_window(false);
         //
         //         }


      }
      else if (nID == IDC_BROWSE)
      {

         property_set set;

         set["file_filter_specs"].stra().add("*.bin");

         set["file_filter_names"].stra().add("Update File");

         set["folder"] = Context.dir().download();

         if (Context.os().browse_file_open(NULL, set))
         {

            m_editBrowse.set_window_text(set["file_name"]);

            m_pparent->m_pmaininterface->on_browse(set["file_name"].get_string());

         }


      }
      else if (nID == IDC_RADIO_DOWNLOAD)
      {

         m_editBrowse.enable_window(false);
         m_browse.enable_window(false);
         m_checkVersion.enable_window(true);
         m_versionStatus.enable_window(true);

         m_pparent->m_pmaininterface->radio_download();


      }
      else if (nID == IDC_RADIO_BROWSE)
      {

         m_editBrowse.enable_window(true);
         m_browse.enable_window(true);
         m_checkVersion.enable_window(false);
         m_versionStatus.enable_window(false);
         m_download.enable_window(false);
         m_downloadProgress.enable_window(false);

         m_pparent->m_pmaininterface->radio_browse();

      }
      else if (nID == IDC_RADIO_LATEST)
      {

         m_pparent->m_pmaininterface->radio_latest();

      }

   }


} // namespace grbl_merge_tool


