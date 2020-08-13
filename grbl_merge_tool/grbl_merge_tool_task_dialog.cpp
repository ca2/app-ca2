#include "framework.h"


namespace grbl_merge_tool
{


   task_dialog::task_dialog(::aura::application * papp) :
      ::object(papp),
      simple_dialog(papp, "matter://account_settings.html")
   {


   }


   task_dialog::~task_dialog()
   {


   }
   void task_dialog::show()
   {

      simple_dialog::show();

      m_pframe->set_window_text("Task");

      m_pframe->ResizeWindow(size(400, 850));

      m_pframe->set_need_layout();

      m_pframe->RedrawWindow();

      //m_pform->get_child_by_id("username")->SetFocus();

   }


   bool task_dialog::on_control_event(::user::form_window * pview, ::user::control_event * pevent)
   {

      UNREFERENCED_PARAMETER(pview);

      if (pevent->m_eevent == ::user::event_form_initialize)
      {

         on_update_image_path();

      }

      else if (pevent->m_eevent == ::user::event_button_clicked
               || pevent->m_eevent == ::user::event_enter_key)
      {
         if (pevent->m_puie->m_id == "button_ok" ||
               pevent->m_eevent == ::user::event_enter_key ||
               pevent->m_puie->m_id == "button_apply")
         {

         }

         if (pevent->m_puie->m_id == "button_ok" ||
               pevent->m_eevent == ::user::event_enter_key ||
               pevent->m_puie->m_id == "button_cancel")
         {

            m_pframe->ShowWindow(SW_HIDE);
         }
         if (pevent->m_puie->m_id == "button_browse_file")
         {
            property_set set;
            set["file_filter_specs"] = "*.jpg;*.png;*.gif";
            set["file_filter_names"] = "Image Files";
            if (Context.os().browse_file_open(pevent->m_puie->get_handle(), set))
            {
               string strFileName = set["file_name"];

               __pointer(::user::edit_text) ptext = m_pform->get_child_by_id("path");

               ptext->_001SetText(strFileName, ::source_sync);

               on_update_image_path();

            }
         }
         else if (pevent->m_puie->m_id == "button_browse_folder")
         {
            property_set set;
            if (Context.os().browse_folder(pevent->m_puie->get_handle(), set))
            {
               string strFileName = set["folder"];

               __pointer(::user::edit_text) ptext = m_pform->get_child_by_id("path");

               ptext->_001SetText(strFileName, ::source_sync);

               on_update_image_path();

            }
         }
         return true;
      }
      return false;
   }

   void task_dialog::on_update_image_path()
   {
      string strFileName;

      //debug_break();

      __pointer(::user::edit_text) ptext = m_pform->get_child_by_id("path");

      ptext->_001GetText(strFileName);

      index iImage = m_pform->get_html_data()->get_image_index("image_placeholder");

      ::draw2d::dib_pointer & dibTarget = m_pform->get_html_data()->m_imagea[iImage]->m_spdib;

      dibTarget.alloc(allocer());

      dibTarget->create(368 - 32, 207 - 18);

      if (Context.dir().is(strFileName))
      {

         dibTarget->Fill(ARGB(255, 255, 255, 255));

         ::file::listing ls(get_context_application());

         stringa straPattern;

         straPattern.add("*.png");
         straPattern.add("*.jpg");
         straPattern.add("*.gif");

         ls.ls_pattern_file(strFileName, straPattern);

         ::draw2d::font_pointer font(allocer());

         font->create_pixel_font(FONT_SANS, 10.0);

         dibTarget->get_graphics()->selectFont(font);

         if (ls.get_count() > 0)
         {
            dibTarget->get_graphics()->set_text_color(ARGB(255, 0, 128, 0));
            dibTarget->get_graphics()->text_out(10, 10, "Path is a folder with " + ::str::from(ls.get_count()) + " supported images");
         }
         else
         {
            dibTarget->get_graphics()->set_text_color(ARGB(255, 255, 0, 0));
            dibTarget->get_graphics()->text_out(10, 10, "Path is a folder with no supported images");
         }


      }
      else
      {

         ::visual::dib_pointer dib(allocer());

         dib.load_from_file(strFileName);

         double dx = (double)dib->m_size.cx / (double)dibTarget.m_size.cx;
         double dy = (double)dib->m_size.cy / (double)dibTarget.m_size.cy;

         double dr = MAX(dx, dy);

         dibTarget->Fill(ARGB(255, 255, 255, 255));

         dibTarget->get_graphics()->StretchBlt(0, 0, dib->m_size.cx / dr, dib->m_size.cy / dr, dib.get_graphics(), 0, 0, dib.m_size.cx, dib.m_size.cy);

      }

      m_pform->set_need_redraw();
      //m_pform->get_html_data()->m_imagea[iImage]->m_spdib = dib;

   }


} // namespace grbl_merge_tool