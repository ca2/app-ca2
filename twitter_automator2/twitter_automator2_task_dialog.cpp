#include "framework.h"


namespace twitter_automator2
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

      m_pframe->ResizeWindow(size(400, 700));

      m_pframe->set_need_layout();

      m_pframe->RedrawWindow();

      //m_pform->get_child_by_id("username")->SetFocus();

   }


   void task_dialog::on_control_event(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_form_initialize)
      {

         synch_lock sl(m_ptaska->mutex());

         if (m_pform->get_child_by_id("username") == NULL)
         {

            return;

         }

         m_pform->get_child_by_id("username")->SetFocus();

         string str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_hint_target];

         m_pform->get_child_by_id("username")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source];

         m_pform->get_child_by_id("source")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source_api_consumer_key];

         m_pform->get_child_by_id("consumer_key")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source_api_consumer_sec];

         m_pform->get_child_by_id("consumer_secret")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source_proxy];

         m_pform->get_child_by_id("proxy")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_api_token_key];

         m_pform->get_child_by_id("token_key")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_api_token_sec];

         m_pform->get_child_by_id("token_secret")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_delay];

         m_pform->get_child_by_id("delay")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_hashtag];

         m_pform->get_child_by_id("hash_tag")->_001SetText(str, ::source_sync);

         str = m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_option];

         if (str.compare_ci("Copy Photos Text and Videos") == 0)
         {

            str = "Copy Photos, Text and Videos";

         }

         __pointer(::user::combo_box) pcombo = m_pform->get_child_by_id("copy_mode");

         index iFind = pcombo->_001FindListText(str);

         pcombo->set_current_item(iFind, ::source_sync);

      }

      else if (pevent->m_eevent == ::user::event_button_clicked
               || pevent->m_eevent == ::user::event_enter_key)
      {
         if (pevent->m_puie->m_id == "button_ok" ||
               pevent->m_eevent == ::user::event_enter_key ||
               pevent->m_puie->m_id == "button_apply")
         {

            {
               synch_lock sl(m_ptaska->mutex());
               string str;

               m_pform->get_child_by_id("username")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_hint_target] = str;


               m_pform->get_child_by_id("source")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source] = str;

               m_pform->get_child_by_id("consumer_key")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source_api_consumer_key] = str;
               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_api_consumer_key] = str;

               m_pform->get_child_by_id("consumer_secret")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source_api_consumer_sec] = str;
               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_api_consumer_sec] = str;

               m_pform->get_child_by_id("proxy")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_source_proxy] = str;
               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_proxy] = str;

               m_pform->get_child_by_id("token_key")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_api_token_key] = str;

               m_pform->get_child_by_id("token_secret")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_target_api_token_sec] = str;


               m_pform->get_child_by_id("delay")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_delay] = str;

               m_pform->get_child_by_id("hash_tag")->_001GetText(str);

               m_ptaska->prepare_field(str);

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_hashtag] = str;

               __pointer(::user::combo_box) pcombo = m_pform->get_child_by_id("copy_mode");

               pcombo->_001GetListText(pcombo->current_item(), str);

               m_ptaska->prepare_field(str);

               if (str.compare_ci("Copy Photos, Text and Videos") == 0)
               {

                  str = "Copy Photos Text and Videos";

               }

               m_ptaska->m_stra[m_iTask][::twitter_automator::task::data_option] = str;

            }

            m_ptaska->update_task(m_iTask);
         }

         if(pevent->m_puie->m_id == "button_ok" ||
               pevent->m_eevent == ::user::event_enter_key ||
               pevent->m_puie->m_id == "button_cancel")
         {

            m_pframe->ShowWindow(SW_HIDE);
         }

         pevent->m_bRet = true;

         return;

      }

   }


} // namespace twitter_auto