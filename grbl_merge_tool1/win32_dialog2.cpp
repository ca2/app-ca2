#include "framework.h"


namespace grbl_merge_tool
{


   dialog2::dialog2(::aura::application * papp) :
      ::object(papp),
      ::win32::dialog(papp, MAKEINTRESOURCE(IDD_DIALOG3))
   {

   }


   dialog2::~dialog2()
   {

   }


   bool dialog2::on_init_dialog()
   {

      child_dialog::on_init_dialog();

      get_dlg_item(m_firmwareVersion, IDC_FIRMWARE_VERSION);

      get_dlg_item(m_progressControl, IDC_PROGRESS_CONTROL);

      get_dlg_item(m_progressStatus, IDC_PROGRESS_STATUS);

      get_dlg_item(m_uploadProgress, IDC_UPLOAD_PROGRESS);

      get_dlg_item(m_combo, IDC_COMBO1);

      get_dlg_item(m_update2, IDC_UPDATE2);

      get_dlg_item(m_result, IDC_RESULT);

      set_dlg_item_font(IDC_TITLE1, m_pparent->m_fTitle1);

      set_dlg_item_font(IDC_DESCRIPTION, m_pparent->m_fText1);

      set_dlg_item_font(IDC_FIRMWARE_VERSION, m_pparent->m_fTitle2);

      get_dlg_item(m_result, IDC_RESULT);

      m_progressStatus.set_font(m_pparent->m_fText1);

      m_uploadProgress.set_font(m_pparent->m_fText1);

      m_result.set_font(m_pparent->m_fText1);

      m_combo.set_font(m_pparent->m_fText1);

      m_update2.set_font(m_pparent->m_fText1);

      {
         rect r;
         ::GetWindowRect(m_combo.m_hwnd, r);
         rect r2;
         ::GetWindowRect(m_update2.m_hwnd, r2);
         ::SetWindowPos(m_combo.m_hwnd, HWND_TOP, 0, 0, r.width(), r2.height(), SWP_NOMOVE);

      }

      get_dlg_item(m_result, IDC_RESULT);

      return true;

   }


   void dialog2::on_command(UINT nID)
   {

      if (nID == IDC_UPDATE2)
      {

         Application.m_portinfoOverride = Application.m_portinfoa[m_combo.get_combo_sel()];

         Application.m_strDetectedOrSelected = "Selected";

         m_pparent->m_pmaininterface->start_update();

      }

   }

   void dialog2::on_notify(NMHDR * p)
   {
      if (p->idFrom == IDC_SYSLINK1)
      {

         if (p->code == NM_CLICK)
         {

            m_pparent->m_pmaininterface->show_log();

         }

      }

   }
   INT_PTR dialog2::on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {

      if (uiMessage == WM_CTLCOLORSTATIC)
      {
         // Set the colour of the text for our URL
         if (m_pparent->m_pmaininterface->m_bError)
         {
            if ((HWND)lparam == m_result.m_hwnd
                  || (HWND)lparam == m_progressStatus.m_hwnd)
            {
               // we're about to draw the static
               // set the text colour in (HDC)lParam
               SetBkMode((HDC)wparam, TRANSPARENT);
               SetTextColor((HDC)wparam, RGB(255, 0, 0));
               // NOTE: per documentation as pointed out by selbie, GetSolidBrush would leak a GDI handle.
               return (BOOL)GetSysColorBrush(COLOR_MENU);
            }
         }
      }

      return child_dialog::on_dialog_message(uiMessage, wparam, lparam);

   }


} // namespace grbl_merge_tool


