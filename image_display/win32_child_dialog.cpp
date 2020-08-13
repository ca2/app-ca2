#include "framework.h"


namespace quasar_science_update_utility
{


   bool child_dialog::on_init_dialog()
   {

      get_dlg_item(m_logo, IDC_LOGO);

      return true;

   }


   INT_PTR child_dialog::on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {

      if (uiMessage == WM_DRAWITEM)
      {
         DRAWITEMSTRUCT* pdraw = (DRAWITEMSTRUCT*)lparam;
         // Manually draw the progress bar.
         if (pdraw->hwndItem == m_logo.m_hwnd)
         {

            ::draw2d::graphics_pointer g(allocer());

            g->Attach(pdraw->hDC);

            rect r;

            ::GetClientRect(m_logo.m_hwnd, r);



            g->fill_solid_rect(r, ::GetSysColor(COLOR_3DFACE) | (255 << 24));

            g->draw(null_point(), m_pparent->m_dib->m_size,
                    m_pparent->m_dib->get_graphics());

            return TRUE;

         }

      }

      return ::win32::dialog::on_dialog_message(uiMessage, wparam, lparam);

   }



} // namespace quasar_science_update_utility