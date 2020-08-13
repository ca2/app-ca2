#include "framework.h"

//string get_file_open(::user::frame_window * pframe, string strFolder);

namespace imagecopy2
{


   task_dialog::task_dialog(::aura::application * papp) :
      ::object(papp),
      simple_dialog(papp, "matter://parameters.html")
   {


   }


   task_dialog::~task_dialog()
   {


   }
   void task_dialog::show()
   {

      simple_dialog::show();

      m_pframe->set_window_text("Task");

      m_pframe->ResizeWindow(size(400, 400));

      m_pframe->set_need_layout();

      m_pframe->RedrawWindow();

      //m_pform->get_child_by_id("username")->SetFocus();

   }


   bool task_dialog::on_control_event(::user::form_window * pview, ::user::control_event * pevent)
   {
      
      if (pevent->m_eevent == ::user::event_form_initialize)
      {

         synch_lock sl(&m_plist->m_mutexParameters);

         if (m_pform->get_child_by_id("startdatetime") == NULL)
         {

            return false;

         }

         m_pform->get_child_by_id("startdatetime")->SetFocus();

         m_pform->get_child_by_id("startdatetime")->_001SetText(m_plist->m_strStartDateTime, ::source_sync);

         m_pform->get_child_by_id("enddatetime")->_001SetText(m_plist->m_strEndDateTime, ::source_sync);

         m_pform->get_child_by_id("interval")->_001SetText(m_plist->m_strInterval, ::source_sync);

         m_pform->get_child_by_id("folder")->_001SetText(m_plist->m_strFolder, ::source_sync);


      }

      else if (pevent->m_eevent == ::user::event_after_change_text)
      {
      
         if (pevent->m_puie->m_id == "startdatetime")
         {

            synch_lock sl(&m_plist->m_mutexParameters);

            m_pform->get_child_by_id("startdatetime")->_001GetText(m_plist->m_strStartDateTime);

            m_plist->save_parameters();


         }
         else if (pevent->m_puie->m_id == "enddatetime")
         {

            synch_lock sl(&m_plist->m_mutexParameters);

            m_pform->get_child_by_id("enddatetime")->_001GetText(m_plist->m_strEndDateTime);

            m_plist->save_parameters();


         }
         else if (pevent->m_puie->m_id == "interval")
         {

            synch_lock sl(&m_plist->m_mutexParameters);


            m_pform->get_child_by_id("interval")->_001GetText(m_plist->m_strInterval);

            m_plist->save_parameters();


         }
         else if (pevent->m_puie->m_id == "folder")
         {

            synch_lock sl(&m_plist->m_mutexParameters);


            m_pform->get_child_by_id("folder")->_001GetText(m_plist->m_strFolder);

            m_plist->save_parameters();


         }


      }
      else if (pevent->m_eevent == ::user::event_button_clicked
         || pevent->m_eevent == ::user::event_enter_key)
      {
         if (pevent->m_puie->m_id == "button_browse")
         {

            property_set set;

            m_pform->get_child_by_id("folder")->_001GetText(set["folder"]);

            if (Context.os().browse_file_open(m_plist->get_handle(), set))
            {

               m_pform->get_child_by_id("folder")->_001SetText(set["folder"], ::source_sync);

               m_plist->m_strFolder = set["folder"];

               m_plist->save_parameters();

            }


         }
         else if (pevent->m_puie->m_id == "button_close")
         {

            {

               synch_lock sl(&m_plist->m_mutexParameters);

               m_pform->get_child_by_id("startdatetime")->_001GetText(m_plist->m_strStartDateTime);

               m_pform->get_child_by_id("enddatetime")->_001GetText(m_plist->m_strEndDateTime);

               m_pform->get_child_by_id("interval")->_001GetText(m_plist->m_strInterval);

               m_pform->get_child_by_id("folder")->_001GetText(m_plist->m_strFolder);

               m_plist->save_parameters();


            }


         }

         if (pevent->m_puie->m_id == "button_close")
         {

            m_pframe->ShowWindow(SW_HIDE);

         }

         return true;

      }

      return false;

   }



} // namespace twitter_auto