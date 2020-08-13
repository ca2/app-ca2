#include "framework.h"

//string get_file_open(::user::frame_window * pframe, string strFolder);

namespace imagecopy2
{


   main_view::main_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {


   }


   main_view::~main_view()
   {

   }


   void main_view::assert_valid() const
   {

      ::user::split_view::assert_valid();

   }


   void main_view::dump(dump_context & dumpcontext) const
   {

      ::user::split_view::dump(dumpcontext);

   }


   void main_view::update(::update * pupdate)
   {
      ::user::split_view::on_update(pSender, eupdate, pupdate);

   }


   

   void main_view::on_create_impacts()
   {

      if (get_pane_count() > 0)
         return;

      SetPaneCount(2);

      SetSplitOrientation(orientation_vertical);

      set_position(0,350);

      initialize_split_layout();


      m_pleft = create_pane_view < left_view >(0);

      m_pleft->m_pmainview = this;

      m_pleft->m_pinfo->m_pmainview = this;

      m_phtmldoc = m_pleft->m_phtmldoc;

      m_pform = m_pleft->m_pform;

      //pleftview->create_views();

      m_plist = create_pane_view < main_form_list >(1);

      if (m_plist == NULL)
      {

         System.simple_message_box(NULL, "Could not create file list ::user::impact");

      }

      m_plist->m_pmainview = this;

      m_phtmldoc->open_document_file("matter://parameters.html", true);

      if (m_pform == NULL)
      {

         System.simple_message_box(NULL, "Could not create form");

      }

      set_need_layout(true);

   }

   void main_view::update_form(bool bSave)
   {

      if (!bSave)
      {

         m_pform->get_child_by_id("startdatetime")->_001SetText(m_plist->m_strStartDateTime, ::source_sync);

         m_pform->get_child_by_id("enddatetime")->_001SetText(m_plist->m_strEndDateTime, ::source_sync);

         m_pform->get_child_by_id("interval")->_001SetText(m_plist->m_strInterval, ::source_sync);

         m_pform->get_child_by_id("folder")->_001SetText(m_plist->m_strFolder, ::source_sync);

         m_pform->get_child_by_id("check_thumbnail")->_001SetCheck(m_plist->m_bThumbnails ? ::check_checked : ::check_unchecked, ::source_sync);

      }


   }

   bool main_view::on_control_event(::user::form_window * pview, ::user::control_event * pevent)
   {
      

      if (pevent->m_eevent == ::user::event_form_initialize)
      {

         synch_lock sl(&m_plist->m_mutexParameters);

         if (pview->get_child_by_id("startdatetime") == NULL)
         {

            return false;

         }

         pview->get_child_by_id("startdatetime")->SetFocus();
        
         update_form(false);


      }
      else if (pevent->m_eevent == ::user::event_set_check)
      {

         if (pevent->m_context.is_user_source() && pevent->m_puie->m_id == "check_thumbnail")
         {

            m_plist->m_bThumbnails = pview->get_child_by_id("check_thumbnail")->_001GetCheck() == ::check_checked;

            m_plist->save_parameters(false, true);

         }
      }

      else if (pevent->m_eevent == ::user::event_after_change_text)
      {

         if (pevent->m_context.is_user_source())
         {
            if (pevent->m_puie->m_id == "startdatetime")
            {

               synch_lock sl(&m_plist->m_mutexParameters);

               pview->get_child_by_id("startdatetime")->_001GetText(m_plist->m_strStartDateTime);

               m_plist->save_parameters(true, false);


            }
            else if (pevent->m_puie->m_id == "enddatetime")
            {

               synch_lock sl(&m_plist->m_mutexParameters);

               pview->get_child_by_id("enddatetime")->_001GetText(m_plist->m_strEndDateTime);

               m_plist->save_parameters(true, false);


            }
            else if (pevent->m_puie->m_id == "interval")
            {

               synch_lock sl(&m_plist->m_mutexParameters);


               pview->get_child_by_id("interval")->_001GetText(m_plist->m_strInterval);

               m_plist->save_parameters(true, false);


            }
            else if (pevent->m_puie->m_id == "folder")
            {

               synch_lock sl(&m_plist->m_mutexParameters);


               pview->get_child_by_id("folder")->_001GetText(m_plist->m_strFolder);

               m_plist->save_parameters(false, false);


            }

         }

      }
      else if (pevent->m_eevent == ::user::event_button_clicked
         || pevent->m_eevent == ::user::event_enter_key)
      {
         if (pevent->m_puie->m_id == "button_open_folder")
         {

            m_plist->_001OnOpenFolder(NULL);

         }
         else if (pevent->m_puie->m_id == "button_copy")
         {

            m_plist->_001OnFileCopy(NULL);

         }
         else if (pevent->m_puie->m_id == "button_browse")
         {

            property_set set;

            pview->get_child_by_id("folder")->_001GetText(set["folder"]);

            if (Context.os().browse_folder(m_plist->get_handle(), set))
            {

               pview->get_child_by_id("folder")->_001SetText(set["folder"], ::source_sync);

               m_plist->m_strFolder = set["folder"];

               m_plist->save_parameters(false, false);

            }


         }
         else if (pevent->m_puie->m_id == "button_close")
         {

            {

               synch_lock sl(&m_plist->m_mutexParameters);

               pview->get_child_by_id("startdatetime")->_001GetText(m_plist->m_strStartDateTime);

               pview->get_child_by_id("enddatetime")->_001GetText(m_plist->m_strEndDateTime);

               pview->get_child_by_id("interval")->_001GetText(m_plist->m_strInterval);

               pview->get_child_by_id("folder")->_001GetText(m_plist->m_strFolder);

               m_plist->save_parameters();


            }


         }

         if (pevent->m_puie->m_id == "button_close")
         {

            //m_pframe->ShowWindow(SW_HIDE);

         }

         return true;

      }

      return false;

   }





} // namespace imagecopy2



