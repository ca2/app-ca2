#include "framework.h"


namespace twitter_auto
{

   user_array::user_array(container * pcontainer) :
      object(pcontainer->get_context_application()),
      ::thread(pcontainer->get_context_application()),
      m_pcontainer(pcontainer),
      m_mutexError(pcontainer->get_context_application()),
      m_mutexMediaId(pcontainer->get_context_application())
   {

      m_bAddEmpty = false;

      m_strPath = Context.dir().home() / "Twitter Auto" / "Twitter Auto.csv";

      m_iActive = 0;

      set_header();

   }


   user_array::~user_array()
   {


   }


   void user_array::simple_csv_on_load()
   {

      stop_all_tasks();

      remove_all();

      set_header();

      synch_lock sl(mutex());

      for (index i = 0; i < m_stra.get_count(); i++)
      {

         user * ptask = canew(user(this));

         ptask->m_iIndex = i;

         add(ptask);

         ptask->simple_csv_set_line(m_stra[i]);

      }

   }

   void user_array::simple_csv_on_save()
   {


   }

   void user_array::update_task(index iTask)
   {

      synch_lock sl(mutex());

      __pointer(user) ptask = element_at(iTask);

      sl.unlock();

      ptask->simple_csv_set_line(m_stra[iTask]);

      sl.lock();

      m_bPendingSave = true;

   }

   void user_array::delete_task(index iTask)
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

         __pointer(user) ptask = element_at(iTask);

         remove_at(iTask);

         m_stra.remove_at(iTask);

         sl.unlock();

         ptask->set_finish();

      }

      save();

   }


   void user_array::on_notify_change_status(user * ptask)
   {

      index iFind = find_first(ptask);

      if (iFind >= 0)
      {

         m_stra[iFind][user::data_status] = ptask->get(user::data_status);

      }

   }

   void user_array::run()
   {

      {

         synch_lock sl(mutex());

         simple_csv_load_file(m_strPath);

      }

      for (index i = 0; i < m_stra.get_count(); i++)
      {

         m_stra[i].set_size(MAX(m_stra[i].get_size(), ::twitter_auto::user::data_count));

      }

      DWORD dwPreviousSize = 0;

      bool bChanged = false;

      while (thread_get_run())
      {

         try
         {

            {

               synch_lock sl(&m_mutexError);

               if (m_strError.str().length() > dwPreviousSize)
               {

                  bChanged = true;

                  dwPreviousSize = m_strError.str().length();

                  m_dwLastErrorChange = get_tick_count();

               }
               else if (bChanged && get_tick_count() - m_dwLastErrorChange > 2000)
               {

                  ::file::path path = ::dir::home() / "Twitter Auto/error.txt";

                  Context.file().put_contents_utf8(path, m_strError.str());

                  Context.os().file_open(path);

                  bChanged = false;

               }


            }

            synch_lock sl(mutex());

            int iActive = 0;

            for (index i = 0; i < get_count(); i++)
            {

               element_at(i)->m_iIndex = i;

               if (element_at(i)->m_estatusRequest == user::status_on)
               {

                  if (element_at(i)->m_hthread == NULL)
                  {

                     element_at(i)->thread::begin();

                  }

                  if (element_at(i)->thread_get_run())
                  {

                     element_at(i)->m_estatusRequest = user::status_none;

                  }

               }
               else if(element_at(i)->m_estatusRequest == user::status_off)
               {

                  if (element_at(i)->m_hthread != NULL)
                  {

                     element_at(i)->set_finish();

                  }

                  if (!element_at(i)->thread_get_run())
                  {

                     element_at(i)->m_estatusRequest = user::status_none;

                  }
               }

               if (get_tick_count() - element_at(i)->m_dwLastStatusChange < 5000)
               {

                  continue;

               }

               if (element_at(i)->thread_get_run() && element_at(i)->m_hthread != NULL)
               {

                  iActive++;

                  if (m_stra[i][user::data_status].compare_ci("ON") != 0)
                  {

                     m_stra[i][user::data_status] = "on";

                     element_at(i)->m_stra.set_at_grow(user::data_status, "on");

                     m_bPendingSave = true;

                  }

               }
               else
               {

                  if (m_stra[i][user::data_status].compare_ci("ON") == 0)
                  {

                     m_stra[i][user::data_status] = "off";

                     element_at(i)->m_stra.set_at_grow(user::data_status, "off");

                     m_bPendingSave = true;

                  }

               }

            }

            m_iActive = iActive;

            if (m_bPendingSave)
            {

               m_bPendingSave = false;

               save();

            }

         }
         catch (...)
         {

         }

         Sleep(100);

      }


      for (index iTask = 0; iTask < get_size(); iTask++)
      {

         try
         {

            ::multithreading::post_quit_and_wait(element_at(iTask), seconds(10));

         }
         catch (...)
         {

         }

      }

   }


   void user_array::save()
   {

      synch_lock sl(mutex());

      set_header();

      simple_csv_save_file(m_strPath);

   }


   void user_array::backup()
   {

      synch_lock sl(mutex());

      string strPath = Context.dir().home() / "Twitter Auto" / "Twitter Auto - Backup " +
                       System.datetime().international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE) +".csv";

      if (!file_exists(strPath))
      {

         simple_csv_save_file(strPath);

      }

   }


   void user_array::assert_valid() const
   {

   }


   void user_array::dump(dump_context & dumpcontext) const
   {

   }

   void user_array::set_header()
   {
      m_straHeader.set_at_grow(user::data_hint_username, "Account");
      m_straHeader.set_at_grow(user::data_target, "Source");
      m_straHeader.set_at_grow(user::data_source_proxy, "Proxy");
      m_straHeader.set_at_grow(user::data_source_api_consumer_key, "Consumer Key");
      m_straHeader.set_at_grow(user::data_source_api_consumer_sec, "Consumer Secret");
      m_straHeader.set_at_grow(user::data_target_api_token_key, "Token Key");
      m_straHeader.set_at_grow(user::data_target_api_token_sec, "Token Secret");
      ///m_straHeader.set_at_grow(user::data_option, "Copy Mode");
      m_straHeader.set_at_grow(user::data_message, "Message");
      m_straHeader.set_at_grow(user::data_image, "Image");
      m_straHeader.set_at_grow(user::data_status, "Status");
      m_straHeader.set_at_grow(user::data_delay, "Delay");
      m_straHeader.set_at_grow(user::data_reset, "Reset");
      //m_straHeader.set_at_grow(user::data_target_api_consumer_key, "Target Consumer Key");
      //m_straHeader.set_at_grow(user::data_target_api_consumer_sec, "Target Consumer Secret");
//      m_straHeader.set_at_grow(user::data_target_proxy, "Target Proxy");

      //m_straHeader.set_at_grow(user::data_target, "Source");
      //m_straHeader.set_at_grow(user::data_source_api_consumer_key, "Source Consumer Key");
      //m_straHeader.set_at_grow(user::data_source_api_consumer_sec, "Source Consumer Secret");
      //m_straHeader.set_at_grow(user::data_source_proxy, "Source Proxy");
      //m_straHeader.set_at_grow(user::data_hint_username, "Target hint");
      //m_straHeader.set_at_grow(user::data_target_api_consumer_key, "Target Consumer Key");
      //m_straHeader.set_at_grow(user::data_target_api_consumer_sec, "Target Consumer Secret");
      //m_straHeader.set_at_grow(user::data_target_api_token_key, "Target Token Key");
      //m_straHeader.set_at_grow(user::data_target_api_token_sec, "Target Token Secret");
      //m_straHeader.set_at_grow(user::data_target_proxy, "Target Proxy");
      //m_straHeader.set_at_grow(user::data_option, "Copy Mode");
//      m_straHeader.set_at_grow(user::data_same_consumer, "Same Consumer");
      //    m_straHeader.set_at_grow(user::data_same_proxy, "Same Proxy");

   }


   bool user_array::set_item_text(index iItem, index iSubItem, string strText)
   {

      synch_lock sl(mutex());

      if (iItem < 0)
      {

         return false;

      }

      if (iItem >= get_count())
      {

         return false;

      }

      if (iSubItem < 0)
      {

         return false;

      }

      if (iSubItem >= user::data_count)
      {

         return false;

      }

      synch_lock sl2(element_at(iItem)->mutex());

      if (iSubItem == user::data_status)
      {

         if (get_tick_count() - element_at(iItem)->m_dwLastStatusChange < 5000)
         {

            return false;

         }

      }

      m_stra.set_size(MAX(m_stra.get_size(), iItem + 1));

      m_stra[iItem].set_size(user::data_count);

      m_stra[iItem][iSubItem] = strText;

      element_at(iItem)->set((user::e_data)iSubItem, m_stra[iItem][iSubItem]);

      return true;

   }


   void user_array::start_task(index iTask)
   {

      synch_lock sl(mutex());

      element_at(iTask)->m_estatusRequest = user::status_on;

   }


   void user_array::stop_task(index iTask)
   {

      synch_lock sl(mutex());

      try
      {

         element_at(iTask)->m_estatusRequest = user::status_off;

      }
      catch (...)
      {


      }

   }

   void user_array::start_all_tasks()
   {

      synch_lock sl(mutex());

      for (index iTask = 0; iTask < get_size(); iTask++)
      {

         start_task(iTask);

      }

   }

   void user_array::stop_all_tasks()
   {

      synch_lock sl(mutex());

      for (index iTask = 0; iTask < get_size(); iTask++)
      {

         stop_task(iTask);

      }

   }

   void user_array::parse_prefix_stop_before_after(string str)
   {

      synch_lock sl(mutex());

      m_prefixstopbeforeaftera.remove_all();

      stringa stra;

      stra.add_lines(str);

      for (index i = 0; i + 3< stra.get_size(); i += 4)
      {

         prefix_stop_before_after prefixstopbeforeafter;

         prefixstopbeforeafter.m_strPrefix = stra[i + 0];
         prefixstopbeforeafter.m_strStopChars = stra[i + 1];
         prefixstopbeforeafter.m_strBefore = stra[i + 2];
         prefixstopbeforeafter.m_strAfter = stra[i + 3];

         prefixstopbeforeafter.m_strPrefix.trim();
         prefixstopbeforeafter.m_strStopChars.trim();
         prefixstopbeforeafter.m_strBefore.trim();
         prefixstopbeforeafter.m_strAfter.trim();


         m_prefixstopbeforeaftera.add(prefixstopbeforeafter);

      }



   }

   void user_array::parse_prefix_before_after(string str)
   {

      synch_lock sl(mutex());

      m_prefixbeforeaftera.remove_all();

      stringa stra;

      stra.add_lines(str);


      for (index i = 0; i + 2< stra.get_size(); i += 3)
      {

         prefix_before_after prefixbeforeafter;

         prefixbeforeafter.m_strPrefix = stra[i + 0];
         prefixbeforeafter.m_strBefore = stra[i + 1];
         prefixbeforeafter.m_strAfter = stra[i + 2];

         m_prefixbeforeaftera.add(prefixbeforeafter);

      }



   }

   void user_array::parse_domain_find_replace(string str)
   {

      synch_lock sl(mutex());

      m_domainfindreplacea.remove_all();

      stringa stra;

      stra.add_lines(str);


      for (index i = 0; i + 2< stra.get_size(); i += 3)
      {

         domain_find_replace domainfindreplace;

         domainfindreplace.m_strDomain = stra[i + 0];
         domainfindreplace.m_strFind = stra[i + 1];
         domainfindreplace.m_strReplace = stra[i + 2];

         m_domainfindreplacea.add(domainfindreplace);

      }



   }


   string user_array::perform_url_processing(string str)
   {

      str = perform_domain_find_replace(str);

      str = perform_prefix_stop_before_after(str);

      str = perform_prefix_before_after(str);

      return str;

   }

   string user_array::perform_prefix_stop_before_after(string strText)
   {

      synch_lock sl(mutex());

      defer_load_prefix_stop_before_after_file();

      index i = 0;

      string strLink;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strLink = strText.Mid(i, iEnd - i);

         if (m_prefixstopbeforeaftera.apply(strLink))
         {

            strText = strText.Left(i) + strLink + strText.Mid(iEnd);

            i = i + strLink.length();

         }
         else
         {

            i = iEnd;

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strLink = strText.Mid(i, iEnd - i);

         if (m_prefixstopbeforeaftera.apply(strLink))
         {

            strText = strText.Left(i) + strLink + strText.Mid(iEnd);

            i = i + strLink.length();

         }
         else
         {

            i = iEnd;

         }

      }

      return strText;

   }


   string user_array::perform_prefix_before_after(string strText)
   {

      synch_lock sl(mutex());

      defer_load_prefix_before_after_file();

      index i = 0;

      string strLink;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strLink = strText.Mid(i, iEnd - i);

         if (m_prefixbeforeaftera.apply(strLink))
         {

            strText = strText.Left(i) + strLink + strText.Mid(iEnd);

            i = i + strLink.length();

         }
         else
         {

            i = iEnd;

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strLink = strText.Mid(i, iEnd - i);

         if (m_prefixbeforeaftera.apply(strLink))
         {

            strText = strText.Left(i) + strLink + strText.Mid(iEnd);

            i = i + strLink.length();

         }
         else
         {

            i = iEnd;

         }

      }

      return strText;

   }


   string user_array::perform_domain_find_replace(string strText)
   {

      synch_lock sl(mutex());

      defer_load_domain_find_replace_file();

      index i = 0;

      string strLink;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strLink = strText.Mid(i, iEnd - i);

         if (m_domainfindreplacea.apply(strLink))
         {

            strText = strText.Left(i) + strLink + strText.Mid(iEnd);

            i = i + strLink.length();

         }
         else
         {

            i = iEnd;

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strLink = strText.Mid(i, iEnd - i);

         if (m_domainfindreplacea.apply(strLink))
         {

            strText = strText.Left(i) + strLink + strText.Mid(iEnd);

            i = i + strLink.length();

         }
         else
         {

            i = iEnd;

         }

      }

      return strText;

   }

   bool user_array::prefix_before_after::apply(string & strLink)
   {

      //strsize iFind;

      if (!::str::begins_eat_ci(strLink, m_strPrefix))
         return false;

      strLink = m_strBefore + strLink + m_strAfter;

      return true;

   }


   bool user_array::prefix_stop_before_after::apply(string & strLink)
   {

      strsize iFind;

      if (!::str::begins_ci(strLink, m_strPrefix) || (iFind = strLink.FindOneOf(m_strStopChars, m_strPrefix.get_length() + 1)) <= 0)
         return false;

      strLink = strLink.Left(iFind).Mid(m_strPrefix.get_length());

      strLink = m_strBefore + strLink + m_strAfter;

      return true;

   }


   bool user_array::prefix_stop_before_after_array::apply(string & strLink)
   {

      for (auto & prefixstopbeforeafter : *this)
      {

         if (prefixstopbeforeafter.apply(strLink))
         {

            return true;

         }

      }

      return false;

   }



   bool user_array::prefix_before_after_array::apply(string & strLink)
   {

      for (auto & prefixbeforeafter : *this)
      {

         if (prefixbeforeafter.apply(strLink))
         {

            return true;

         }

      }

      return false;

   }

   bool user_array::domain_find_replace::apply(string & strLink)
   {

      if (strLink.find_ci(m_strDomain) < 0)
         return false;

      strLink.replace(m_strFind, m_strReplace);

      return true;

   }


   bool user_array::domain_find_replace_array::apply(string & strLink)
   {

      for (auto & domainfindreplace : *this)
      {

         if (domainfindreplace.apply(strLink))
         {

            return true;

         }

      }

      return false;

   }

   void user_array::defer_load_domain_find_replace_file()
   {

      ::file::path pathDomainFindReplace = Context.dir().home() / "Twitter Auto" / "domain_find_replace.txt";

      if (!file_exists(pathDomainFindReplace))
      {

         file_put_contents_dup(pathDomainFindReplace, Context.file().as_string("matter://default_domain_find_replace.txt"));

      }

      ::file_time ft;

      ft = get_file_time(pathDomainFindReplace);

      if (m_filetimeDomainFindReplace == ft)
      {

         return;

      }

      m_filetimeDomainFindReplace = ft;

      parse_domain_find_replace(Context.file().as_string(pathDomainFindReplace));

   }



   void user_array::defer_load_prefix_before_after_file()
   {

      ::file::path pathPrefixBeforeAfter = Context.dir().home() / "Twitter Auto" / "prefix_before_after.txt";

      if (!file_exists(pathPrefixBeforeAfter))
      {

         file_put_contents_dup(pathPrefixBeforeAfter, Context.file().as_string("matter://default_prefix_before_after.txt"));

      }

      ::file_time ft;

      ft = get_file_time(pathPrefixBeforeAfter);

      if (m_filetimePrefixBeforeAfter == ft)
      {

         return;

      }

      m_filetimePrefixBeforeAfter = ft;

      parse_prefix_before_after(Context.file().as_string(pathPrefixBeforeAfter));

   }


   void user_array::defer_load_prefix_stop_before_after_file()
   {

      ::file::path pathPrefixStopBeforeAfter = Context.dir().home() / "Twitter Auto" / "prefix_stop_before_after.txt";

      if (!file_exists(pathPrefixStopBeforeAfter))
      {

         file_put_contents_dup(pathPrefixStopBeforeAfter, Context.file().as_string("matter://default_prefix_stop_before_after.txt"));

      }

      ::file_time ft;

      ft = get_file_time(pathPrefixStopBeforeAfter);

      if (m_filetimePrefixStopBeforeAfter == ft)
      {

         return;

      }

      m_filetimePrefixStopBeforeAfter = ft;

      parse_prefix_stop_before_after(Context.file().as_string(pathPrefixStopBeforeAfter));

   }

   stringa user_array::path_get_media_ids(user * puser, twitter * ptwitter, const stringa & straSource)
   {

      synch_lock sl(&m_mutexMediaId);


retry:
      stringa stra;
      if (m_mediaids.Lookup(straSource, stra))
      {

         return stra;

      }
      sl.unlock();
      for (auto strPath : straSource)
      {

         bool bGif = str::begins_eat_ci(strPath, "gif://");

         string strName;

         {

            ::file::path path = strPath;

            strsize iFind = path.find('?');

            if (iFind >= 0)
            {

               path = path.Left(iFind);

            }

            strName = path.name();

         }

         string strTime;

         strTime = strPath;

         string strMediaId = ptwitter->media_upload_noauth(strTime, bGif);

         if (!puser->http_connection_ok(strMediaId, ::twitter_auto::log_target))
         {

            goto retry;

         }

         if (strMediaId.begins_ci("error:"))
         {

            puser->log_target("Failed to upload  media file : " + strTime + "; " + strMediaId);

            continue;

         }

         stra.add(strMediaId);

      }

      if (stra.get_size() == straSource.get_size())
      {

         sl.lock();
         m_mediaids.set_at(straSource, stra);

      }

      return stra;

   }


} // namespace twitter_auto


