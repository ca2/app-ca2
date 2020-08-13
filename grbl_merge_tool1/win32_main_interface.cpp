#include "framework.h"




void main_interface::on_check_version_response(const char * pszStatus, bool bEnable)
{

   m_pmainwindow->m_dlg1.m_versionStatus.set_window_text(pszStatus);

   if (bEnable)
   {

      m_pmainwindow->m_dlg1.m_download.enable_window(true);

      m_pmainwindow->m_dlg1.m_downloadProgress.enable_window(true);

   }

   m_pmainwindow->m_dlg1.redraw_window();

}


void main_interface::on_radio_download_response(bool bDownloadEnable, bool bUpdateEnable)
{

   m_pmainwindow->m_dlg1.m_download.enable_window(bDownloadEnable);

   m_pmainwindow->m_dlg1.m_downloadProgress.enable_window(bDownloadEnable);

   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


void main_interface::on_radio_latest_response(bool bUpdateEnable)
{

   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}




void main_interface::on_radio_browse_response(bool bUpdateEnable)
{

   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


void main_interface::on_download_response(const char * pszStatus, bool bUpdateEnable)
{

   m_pmainwindow->m_dlg1.m_downloadProgress.set_window_text(pszStatus);

   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


void main_interface::on_app_installation_response(const char * pszStatus)
{

   m_pmainwindow->m_labelApp.set_window_text(pszStatus);

}


void main_interface::on_browse_response(bool bUpdateEnable)
{

   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


//void main_interface::set_progress_status(const char * psz)
//{
//
//   m_pmainwindow->m_dlg2.m_progressStatus.set_window_text(psz);
//
//}


void main_interface::set_result(const char * pszPort, const char * psz)
{

   m_pmainwindow->m_dlg2.m_result.set_window_text(psz);

}


void main_interface::set_progress(const char * pszPort, int iPercent, const char * pszStatus)
{

   string strStatus(pszStatus);

   m_pmainwindow->m_dlg2.m_progressControl.send_message(PBM_SETPOS, iPercent);

   if (iPercent == 0)
   {

      m_pmainwindow->m_dlg2.m_uploadProgress.set_window_text("");

   }
   else
   {

      m_pmainwindow->m_dlg2.m_uploadProgress.set_window_text(strStatus);

   }

   m_pmainwindow->m_dlg2.m_progressControl.redraw_window();

}



void main_interface::show_manual_com(char * const * pszList, int iCount, int iSel)
{

   m_pmainwindow->m_dlg2.m_combo.send_message(CB_RESETCONTENT);

   for (int i = 0; i < iCount; i++)
   {

      m_pmainwindow->m_dlg2.m_combo.send_message(CB_ADDSTRING, 0, (LPARAM)pszList[i]);

   }

   m_pmainwindow->m_dlg2.m_combo.send_message(CB_SETCURSEL, iSel);

   m_pmainwindow->m_dlg2.m_combo.show_window(SW_SHOW);

   m_pmainwindow->m_dlg2.m_update2.show_window(SW_SHOW);

}


void main_interface::hide_manual_com()
{

   m_pmainwindow->m_dlg2.m_combo.show_window(SW_HIDE);
   m_pmainwindow->m_dlg2.m_update2.show_window(SW_HIDE);
   m_pmainwindow->m_dlg2.m_combo.send_message(CB_RESETCONTENT);

}

void main_interface::cancel_enable(bool bEnable)
{

   m_pmainwindow->m_cancel.enable_window(bEnable);

}


void main_interface::update_enable(bool bEnable)
{

   m_pmainwindow->m_update.enable_window(bEnable);

}

void main_interface::redraw_window()
{

   m_pmainwindow->m_dlg2.m_progressControl.redraw_window();

}


void main_interface::to_close()
{

   m_pmainwindow->m_cancel.set_window_text("Close");

   m_pmainwindow->m_restart.enable_window(true);

}

void main_interface::to_cancel()
{

   m_pmainwindow->m_cancel.set_window_text("Cancel");

   m_pmainwindow->m_restart.enable_window(false);

}




void main_interface::on_app_check_version(const char * pszChar)
{

   m_pmainwindow->m_labelApp.set_window_text(pszChar);

   m_pmainwindow->m_labelApp.show_window(SW_SHOW);

   m_pmainwindow->m_updateApp.show_window(SW_SHOW);

}


void main_interface::quit_system()
{



}



void main_interface::on_disable_install_downloaded_bin()
{

   m_pmainwindow->m_dlg1.m_radioInstallVersion.enable_window(false);

}

void main_interface::on_enable_install_downloaded_bin(const char * strTitle)
{

   m_pmainwindow->m_dlg1.m_radioInstallVersion.set_window_text("Install Version " + string(strTitle));
   m_pmainwindow->m_dlg1.m_radioInstallVersion.enable_window();
   m_pmainwindow->m_dlg1.m_radioInstallVersion.send_message(BM_SETCHECK, BST_CHECKED);
   m_pmainwindow->m_dlg1.m_radioBrowse.send_message(BM_SETCHECK, BST_UNCHECKED);
   m_pmainwindow->m_dlg1.m_radioDownload.send_message(BM_SETCHECK, BST_UNCHECKED);
   m_pmainwindow->m_update.enable_window();


}



void main_interface::add_port(const char * pszPort)
{
}


void main_interface::remove_port(const char * pszPort)
{
}

