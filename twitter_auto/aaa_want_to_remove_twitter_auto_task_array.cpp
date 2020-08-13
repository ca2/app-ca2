#include "framework.h"


namespace twitter_auto
{

   task_array::task_array(main_form_list * pformlist) :
      object(pformlist->get_context_application()),
      ::thread(pformlist->get_context_application()),
      m_pformlist(pformlist)
   {
      m_bAddEmpty = false;
      m_strPath = Context.dir().home() / "Twitter Automator" / "Twitter Automator.csv";
      m_iActive = 0;
      set_header();
   }


   task_array::~task_array()
   {


   }


   void task_array::simple_csv_on_load()
   {

      stop_all_tasks();

      remove_all();

      set_header();

      for (index i = 0; i < m_stra.get_count(); i++)
      {

         task * ptask = new task(this);

         ptask->simple_csv_set_line(m_stra[i]);

         ptask->m_iIndex = i;

         add(ptask);

      }

   }

   void task_array::simple_csv_on_save()
   {


   }

   void task_array::stop_all_tasks()
   {
      // todo
   }

   void task_array::update_task(int iTask)
   {
      
      synch_lock sl(mutex());

      __pointer(task) ptask = element_at(iTask);
      
      sl.unlock();
         
      ptask->simple_csv_set_line(m_stra[iTask]);

      sl.lock();

      m_bPendingSave = true;

   }

   void task_array::delete_task(int iTask)
   {

      if (iTask < 0)
      {
         return;
      }
      if (iTask >= get_count())
      {

         return;

      }

      backup();

      {

         synch_lock sl(mutex());

         __pointer(task) ptask = element_at(iTask);

         remove_at(iTask);

         m_stra.remove_at(iTask);

         sl.unlock();

         ptask->set_finish();

      }

      save();
      
   }


   void task_array::on_notify_change_status(task * ptask)
   {

      index iFind = find_first(ptask);

      if (iFind >= 0)
      {

         m_stra[iFind][task::data_status] = ptask->get(task::data_status);

      }

   }

   i32 task_array::run()
   {

      {

         synch_lock sl(mutex());

         simple_csv_load_file(m_strPath);

      }

      while (thread_get_run())
      {

         try
         {

            synch_lock sl(mutex());

            int iActive = 0;

            for (index i = 0; i < get_count(); i++)
            {

               element_at(i)->m_iIndex = i;

               if (element_at(i)->m_estatusRequest == task::status_on)
               {

                  if (element_at(i)->m_hthread == NULL)
                  {

                     element_at(i)->thread::begin();

                  }

               }
               else if(element_at(i)->m_estatusRequest == task::status_off)
               {

                  if (element_at(i)->m_hthread != NULL)
                  {

                     element_at(i)->set_finish();

                  }

               }

               element_at(i)->m_estatusRequest = task::status_none;

               if (element_at(i)->get_run_thread())
               {

                  iActive++;

                  if (m_stra[i][task::data_status] != "on")
                  {

                     m_stra[i][task::data_status] = "on";

                     m_bPendingSave = true;

                  }

               }
               else
               {

                  if (m_stra[i][task::data_status] != "off")
                  {

                     m_stra[i][task::data_status] = "off";

                     m_bPendingSave = true;

                  }

               }

            }

            m_iActive = iActive;

            if (m_bPendingSave)
            {

               save();

            }

         }
         catch (...)
         {

         }

         Sleep(1000);

      }

      return 0;

   }


   void task_array::save()
   {

      synch_lock sl(mutex());

      set_header();

      simple_csv_save_file(m_strPath);

   }


   void task_array::backup()
   {
   
      synch_lock sl(mutex());
      
      string strPath = Context.dir().home() / "Twitter Automator" / "Twitter Automator - Backup " + 
         System.datetime().international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE) +".csv";

      if (!file_exists(strPath))
      {
      
         simple_csv_save_file(strPath);

      }

   }


   void task_array::assert_valid() const
   {

   }


   void task_array::dump(dump_context & dumpcontext) const
   {

   }

   void task_array::set_header()
   {

      m_straHeader.set_at_grow(task::data_source, "Source");
      m_straHeader.set_at_grow(task::data_source_api_consumer_key, "Source Consumer Key");
      m_straHeader.set_at_grow(task::data_source_api_consumer_sec, "Source Consumer Secret");
      m_straHeader.set_at_grow(task::data_source_proxy, "Source Proxy");
      m_straHeader.set_at_grow(task::data_hint_target, "Target hint");
      m_straHeader.set_at_grow(task::data_target_api_consumer_key, "Target Consumer Key");
      m_straHeader.set_at_grow(task::data_target_api_consumer_sec, "Target Consumer Secret");
      m_straHeader.set_at_grow(task::data_target_api_token_key, "Target Token Key");
      m_straHeader.set_at_grow(task::data_target_api_token_sec, "Target Token Secret");
      m_straHeader.set_at_grow(task::data_target_proxy, "Target Proxy");
      m_straHeader.set_at_grow(task::data_option, "Copy Mode");
      m_straHeader.set_at_grow(task::data_same_consumer, "Same Consumer");
      m_straHeader.set_at_grow(task::data_same_proxy, "Same Proxy");

   }

} // namespace twitter_auto


