#include "framework.h"
#include "core/user/hi5/hi5.h"

#define MAX_HTTP_ERROR 10
#define MAX_EMPTY_SINCEID 10
#define MAX_TIMEOUT_ERROR 60

class http_connection_error
{
public:

   http_connection_error(string str)
   {

      m_str = str;

   }
   string m_str;

};

CLASS_DECL_AURA void
encode_utf16_pair(u32 character, u16 *units);
//CLASS_DECL_AURA bool is_surrogated(u32 character);

#define TARGET_MINUTES_RECONNECT 60
#define TARGET_SECONDS_RECONNECT (TARGET_MINUTES_RECONNECT * 60)

namespace twitter_automator
{

   task::task(task_array * ptaska) :
      object(ptaska->get_context_application()),
      ::thread(ptaska->get_context_application()),
      m_mutexCache(ptaska->get_context_application())
   {

      m_iConnectionTimeoutError = 0;


      m_iHttpConnectionError = 0;

      m_ptaska = ptaska;

      m_stra.set_size(data_count);
      m_ptarget = NULL;

      set(data_status, "off");
      set(data_source, "Source");
      set(data_hint_target, "Target Hint");
      set(data_option, "Copy All");
      set(data_delay, "30");
      set(data_hashtag, "");
//      set(data_same_consumer, "on");
      //    set(data_same_proxy, "on");

      m_straIgnoreTweetDomain.add("fatkiddeals.com");
      //m_straIgnoreTweetDomain.add("twitter.com/fatkiddeals/");
      m_straIgnoreTweetDomain.add("twitter.com");

      m_straExpandDomain.add("amzn.to");
      m_straExpandDomain.add("ebay.to");
      m_straExpandDomain.add("bit.ly");

      //m_straIgnoreTweetWithText.add("FKD");
      m_straRemoveHashTag.add("FKDE");
      m_straRemoveHashTag.add("FKDE");
      m_straIgnoreLinkIgnoredDomain.add("vine.co");


   }

   task::~task()
   {

      ::aura::del(m_ptarget);

   }


   void task::run()
   {

      log_source("NEW TASK RUN!!");
      log_target("NEW TASK RUN!!");
      general_log("NEW TASK RUN!!");

      log_header(::twitter_automator::log_source);
      log_header(::twitter_automator::log_target);
      log_header(::twitter_automator::log_general);

      m_iConnectionTimeoutError = 0;

      m_iHttpConnectionError = 0;

      bool bDebug3Json = true;

      DWORD dwLastFailure = get_tick_count();

      bool bOkSession;

      int iError44Count = 0;

      DWORD dwLastError44 = 0;

      for (index i = 0; i < 3; i++)
      {

         bOkSession = true;

         try
         {


            if (!check_data())
            {

               general_log("exiting (1)");
               return;

            }

            inside_loop_tests();

            while (thread_get_run())
            {

               if (!check_data())
               {

                  general_log("exiting (1.1)");

                  return;

               }

               string strCopy = get(data_option);

               twitter twitterSource(get_context_application(), this, ::twitter_automator::log_source);

               twitterSource.m_strTwitterConsumerKey = get(data_source_api_consumer_key);
               twitterSource.m_strTwitterConsumerSecret = get(data_source_api_consumer_sec);
               twitterSource.m_strProxy = get(data_source_proxy);

               var varSource = twitterSource.last_twitter_automator(get(data_source));

               if (!http_connection_ok(twitterSource.m_strLastConnectionStatus, ::twitter_automator::log_source))
               {

                  continue;

               }

               if (twitterSource.m_strLastError.begins_ci("ERROR: ") || varSource.is_new() || varSource.has_property("errors"))
               {

                  if (twitterSource.m_strLastError.contains_ci("Empty Response"))
                  {

                     log_source("http empty response: considered temporary error (1).");

                     Sleep(1000);

                     continue;

                  }

                  general_log("exiting (2.0)");

                  return;

               }

               log_source("debug varSource (1) " + varSource.get_json(false));

               string strSinceId = varSource.at(0)["id_str"];

               while (thread_get_run())
               {

                  if (!check_data())
                  {

                     general_log("exiting (2.3)");

                     return;

                  }

                  if (strSinceId.is_empty())
                  {

                     if (m_iEmptySinceId > MAX_EMPTY_SINCEID)
                     {

                        log_source("SINCEID is EMPTY for maximum consecutive times (" + ::str::from(m_iEmptySinceId) + ")!!");
                        log_source("exiting (2.5)");
                        general_log("exiting (2.5)");
                        m_error.set(-45);
                        return;

                     }

                     m_iEmptySinceId++;

                     log_source("SINCEID is EMPTY!!: retry no. " + ::str::from(m_iEmptySinceId) + " considered temporary error (1.1) going to retry.");

                     Sleep(1000);

                     break;

                  }

                  m_iEmptySinceId = 0;

                  var varSource;

                  if (bDebug3Json)
                  {

                     string strJson = Context.file().as_string("C:\\archive\\3.json");

                     if (strJson.is_empty())
                     {

                        goto NotDebug3Json;

                     }

                     const char * pszJson = strJson;

                     varSource.parse_json(pszJson);

                     log_source("get_json:" + varSource.get_json(false));

                     bDebug3Json = false;

                  }
                  else
                  {

NotDebug3Json:

                     varSource = twitterSource.next_twitter_automators(get(data_source), strSinceId);

                  }

                  if (!http_connection_ok(twitterSource.m_strLastConnectionStatus, ::twitter_automator::log_source))
                  {

                     continue;

                  }

                  log_source("debug varSource (2) " + varSource.get_json(false));

                  if (twitterSource.m_strLastError.begins_ci("ERROR: ")
                        || varSource.is_new()
                        || varSource.has_property("errors"))
                  {

                     if (varSource["errors"].at(0)["code"].i32() == 44)
                     {

                        // since_id parameter is invalid.

                        iError44Count++;

                        if (get_tick_count() - dwLastError44 > 5 * 60 * 1000)
                        {

                           dwLastError44 = get_tick_count();

                           iError44Count = 0;

                        }

                        if (iError44Count > 5)
                        {

                           general_log("exiting (2.1) more than 5 code 44 error messages in 5 minutes (since_id parameter is invalid)");

                           return;

                        }

                        break;

                     }
                     else if (twitterSource.m_strLastError.contains_ci("Empty Response"))
                     {

                        log_source("http empty response: considered temporary error (2).");

                        Sleep(1000);

                        break;

                     }

                     general_log("exiting (3)");

                     return;

                  }
                  else
                  {

                     index i = varSource.get_count() - 1;

                     index iGoodSinceId = -1;

                     while (i >= 0)
                     {

                        if (varSource.at(i)["full_text"].is_empty())
                        {

                           i--;

                           continue;

                        }

                        iGoodSinceId = i;

                        DWORD dwStart = ::get_tick_count();

                        while (::thread_get_run() && get_tick_count() - dwStart < atoi(get(data_delay)))
                        {
                           Sleep(100);
                        }

                        if (::thread_get_run())
                        {
                           if (!call_send(varSource.at(i)))
                           {

                              general_log("exiting (4)");

                              return;

                           }
                        }

                        i--;

                     }

                     if (iGoodSinceId >= 0 && iGoodSinceId < varSource.get_count())
                     {

                        strSinceId = varSource.at(iGoodSinceId)["id_str"];

                     }

                  }

                  int iWait = 450;

                  while (thread_get_run() && iWait > 0)
                  {


                     Sleep(100);

                     iWait--;


                  }





               }


            }

            general_log("exiting (5)");

            return;

         }
         catch (http_connection_error &e)
         {

            general_log("fatal_HTTP_CONNECTION_ERROR: Task stopped: Account: \"" + get(data_hint_target) + "\": " + e.m_str);

            m_error.set(-34);

            return;

         }
         catch (::exception::exception & e)
         {

            log_source("Exception in twitter_automator::task::run (2)");

            log_source(e.what());

            bOkSession = false;

            if (::str::find_ci("rethrow", e.what())>= 0)
            {

               throw e;

            }

         }
         catch (...)
         {

            general_log("Unknown exception in twitter_automator::task::run(2)");

            bOkSession = false;

         }

         if (bOkSession)
         {

            break;

         }

         general_log("Failure by EXCEPTION: (" + ::str::from(i) + ")");

         if (get_tick_count() - dwLastFailure > 60 * 60 * 1000)
         {

            general_log("reset the failure counter");

            dwLastFailure = get_tick_count();

            i = 0;

         }

         if (i == 2)
         {

            break;

         }

         general_log("Waiting for about 60 seconds to restart service");

         for (index j = 0; j < 120; j++)
         {

            if (!thread_get_run())
            {

               break;

            }

            Sleep(500);

         }

         if (!thread_get_run())
         {

            break;

         }

      }

      if (!bOkSession)
      {

         general_log("General Failure by EXCEPTION: Maximum number of 3 failures in one-hour time span reached");

      }

   }

   bool task::is_cached()
   {

      return get(data_cache).compare_ci("ON") == 0;

   }

   index task::cache_find_oldest_message()
   {

      if (m_varaCache.has_elements())
      {

         return 0;

      }
      else
      {

         return -1;

      }

   }

   index task::cache_send_random_message()
   {

      index i = System.math().RandRange(0, m_varaCache.get_upper_bound());

      try
      {

         send(m_varaCache[i]);

      }
      catch (...)
      {


      }

      m_varaCache.remove_at(i);

      return i;

   }


   void task::step_cache()
   {

      synch_lock sl(&m_mutexCache);

      index iOldestMessage = cache_find_oldest_message();

      if (iOldestMessage < 0)
      {

         return;

      }

      i64 timeNow = ::datetime::time::get_current_time().m_time;

      i64 timeOld = m_varaCache[iOldestMessage]["cache_time"].i64();

      i64 timeSpan = timeNow - timeOld;

      int iCacheTime = 60 * 60;

      ::file::path path = Context.dir().home() / "Twitter Automator/cache_seconds.txt";

      if (file_exists(path))
      {

         iCacheTime = atoi(file_as_string_dup(path));

         iCacheTime = MAX(60, iCacheTime);

      }

      int iFraction = iCacheTime / m_varaCache.get_count();

      if (timeSpan < iCacheTime + iFraction)
      {

         return;

      }

      cache_send_random_message();

   }


   bool task::cache(var & varSource)
   {

      varSource["cache_time"] = (i64) ::datetime::time::get_current_time().m_time;

      {

         synch_lock sl(&m_mutexCache);

         m_varaCache.add(varSource);

      }

      if (m_pthreadCache.is_null())
      {

         m_pthreadCache = fork([this]()
         {

            while (::thread_get_run())
            {

               try
               {

                  step_cache();

               }
               catch (...)
               {

               }

               Sleep(5000);

            }

            m_pthreadCache.release();

         });

      }

      return true;

   }


   bool task::call_send(var & varSource)
   {

      if (is_cached())
      {

         return cache(varSource);

      }
      else
      {

         return send(varSource);

      }

   }

   bool task::send(var & varSource)
   {

      if (!is_source_ok(varSource))
      {
         log_source("IGNORING RETWEET");
         log_target("IGNORING RETWEET");
         return true;

      }

      log_source("OPTION: " + get(data_option));
      log_source("OPTION: " + get(data_option));
      if (get(data_option) == "Copy One Link")
      {

         return send_photos_text_and_one_link(varSource);

      }
      else if (get(data_option) == "Copy Photos Text and Videos")
      {

         return send_media_or_text(varSource);

      }
      else if (get(data_option) == "Copy Text")
      {

         return send_only_if_text(varSource);

      }
      else if (get(data_option) == "Copy All")
      {

         return send_all(varSource);

      }
      else if (get(data_option) == "Copy Bit.ly")
      {

         return send_all_non_expansion(varSource);

      }
      else if (get(data_option) == "Copy Link")
      {

         return send_only_with_photo__text_and_link(varSource);

      }
      else
      {
         log_source("Unknown Option");
         log_target("Unknown Option");
         return false;

      }

   }

   bool task::is_source_ok(var & varSource)
   {

      if (varSource.has_property("retweeted_status"))
      {

         string strId = varSource["retweeted_status"]["id"];

         string strRetweetOriginUserId = varSource["retweeted_status"]["user"]["id"];

         string strUserId = varSource["user"]["id"];

         if (strRetweetOriginUserId == strUserId)
         {

            log_source("Source OK: Retweet exception, origin user == retweeter");

         }
         else
         {

            log_source("Source NOK: Retweet");

            return false;

         }

      }

      string strText = varSource["full_text"];

      for (auto & strDomain : m_straIgnoreTweetDomain)
      {

         if (strText.find_ci(strDomain) >= 0)
         {

            log_source("Source NOK: Domain to ignore found \"" + strDomain + "\"");

            log_source("Evaluated Text: \"" + strText + "\"");

            return false;

         }

      }

      for (auto & str : m_straIgnoreTweetWithText)
      {

         if (strText.find_ci(str) >= 0)
         {

            log_source("Source NOK: text to make skip tweet found \"" + str + "\"");

            log_source("Evaluated Text: \"" + strText + "\"");

            return false;

         }

      }

      for (auto & str : m_straRemoveHashTag)
      {

         index iFind = -1;

         while (true)
         {

            iFind = strText.find_ci("#" + str, iFind + 1);

            if (iFind < 0)
            {

               break;

            }

            index iEnd = iFind + str.length() + 1;

            if (strText[iEnd] == ' '
                  ||strText[iEnd] == '\r'
                  || strText[iEnd] == '\n'
                  || strText[iEnd] == ':'
                  || strText[iEnd] == ','
                  || strText[iEnd] == '.'
               )
            {

               strText = strText.Left(iFind) + strText.Mid(iEnd + 1);

               return false;

            }


         }

      }


      string strExpandFullText = expand_url(strText, varSource);

      string_to_string map = get_expanded_photos(varSource);

      for (auto & strDomain : m_straIgnoreTweetDomain)
      {

         strsize iFind = -1;

         while((iFind = strExpandFullText.find_ci("http://" + strDomain, iFind + 1)) >= 0)
         {

            bool bIsPhoto = false;

            for (auto & pair : map)
            {

               if (pair.m_element2.get_length() > strDomain.get_length())
               {

                  string strExtractPhoto = strExpandFullText.Mid(iFind, pair.m_element2.get_length());

                  ::str::begins_eat_ci(strExtractPhoto, "http://");

                  ::str::begins_eat_ci(strExtractPhoto, "https://");

                  string strUrlPart = pair.m_element2;

                  ::str::begins_eat_ci(strUrlPart, "http://");

                  ::str::begins_eat_ci(strUrlPart, "https://");

                  if(strExtractPhoto.compare_ci(strUrlPart) == 0)
                  {

                     bIsPhoto = true;
                     break;

                  }

               }

            }

            if (!bIsPhoto)
            {

               log_source("Source NOK: (2) Domain to ignore found \"" + strDomain + "\"");

               log_source("Evaluated Text: \"" + strExpandFullText + "\"");

               return false;

            }

         }

         iFind = -1;

         while ((iFind = strExpandFullText.find_ci("https://" + strDomain, iFind + 1)) >= 0)
         {

            bool bIsPhoto = false;

            for (auto & pair : map)
            {

               if (pair.m_element2.get_length() > strDomain.get_length())
               {

                  string strExtractPhoto = strExpandFullText.Mid(iFind, pair.m_element2.get_length());

                  ::str::begins_eat_ci(strExtractPhoto, "http://");

                  ::str::begins_eat_ci(strExtractPhoto, "https://");

                  string strUrlPart = pair.m_element2;

                  ::str::begins_eat_ci(strUrlPart, "http://");

                  ::str::begins_eat_ci(strUrlPart, "https://");

                  if (strExtractPhoto.compare_ci(strUrlPart) == 0)
                  {

                     bIsPhoto = true;
                     break;

                  }

               }

            }

            if (!bIsPhoto)
            {

               log_source("Source NOK: (2) Domain to ignore found \"" + strDomain + "\"");

               log_source("Evaluated Text: \"" + strExpandFullText + "\"");

               return false;

            }

         }

      }

      strsize iFind = -1;

      while (true)
      {

         iFind = strText.find('@', iFind + 1);

         if (iFind < 0)
         {

            break;

         }

         if (iFind + 1 == strText.length())
         {

            break;

         }

         index iEnd = iFind + varSource["user"]["screen_name"].get_string().length() + 1;

         if (iFind - 3 == strText.find("RT @" + varSource["user"]["screen_name"], MAX(0, iFind - 3))
               && (strText[iEnd] == ' '
                   || strText[iEnd] == '\r'
                   || strText[iEnd] == '\n'
                   || strText[iEnd] == ':'
                   || strText[iEnd] == ','
                   || strText[iEnd] == '.'
                  ))
         {

         }
         else if (strText[iFind + 1] != ' '
                  && strText[iFind + 1] != '\r'
                  && strText[iFind + 1] != '\n'
                 )
         {

            log_source("Source NOK: mention");

            return false;

         }

      }

      return true;

   }

   string_to_string task::get_photos_with_media_id(var & varSource)
   {

      string_to_string straPhoto;

      for (auto photo : varSource["extended_entities"]["media"].vara())
      {
         log_source("photo: " + photo["url"]);
         log_target("photo: " + photo["url"]);
         straPhoto.set_at(photo["url"], photo["id"]);

      }

      return straPhoto;

   }

   string_to_string task::get_media_url_media_ids(var & varSource)
   {

      string_to_string straPhoto;

      m_straPhoto.remove_all();

      for (auto photo : varSource["extended_entities"]["media"].vara())
      {
         log_source("media_url: " + photo["url"]);
         log_target("media_url: " + photo["url"]);
         if (photo.has_property("video_info") &&
               photo["video_info"].has_property("variants")
               && photo["video_info"]["variants"].get_count() > 0
               && photo["video_info"]["variants"].at(0).has_property("url")
               && photo["video_info"]["variants"].at(0)["url"].has_char())
         {

            i64 iBitRate = -1;

            string strUrl;

            // get the URL of the video variant with highest bitrate

            for (index iVariant = 0; iVariant < photo["video_info"]["variants"].get_count(); iVariant++)
            {

               if (photo["video_info"]["variants"].at(iVariant)["bitrate"].i64() > iBitRate)
               {

                  iBitRate = photo["video_info"]["variants"].at(iVariant)["bitrate"].i64();

                  strUrl = photo["video_info"]["variants"].at(iVariant)["url"];

               }

            }


            log_source("Found Video! : chosen_bitrate="+::str::from(iBitRate)+" : " + strUrl);

            if (photo.has_property("type") && photo["type"].get_string().compare_ci("animated_gif") == 0)
            {
               straPhoto.set_at(string("gif://")+ strUrl, photo["id"]);
            }
            else
            {
               straPhoto.set_at(strUrl, photo["id"]);
            }
         }
         else
         {
            straPhoto.set_at(photo["media_url"], photo["id"]);
         }
         m_straPhoto.set_at(photo["media_url"], photo["id"]);

      }

      return straPhoto;

   }

   string task::expand_url(string strText, var & varSource)
   {

      string_to_string map = get_expanded_url(varSource);

      for (auto pair : map)
      {

         //if (pair.element1().ends_ci(unitext("…")) && strText.find(pair.m_element1) >= 0)
         //{

         //   output_debug_string("...");

         //}
         //else
         //{

         string strUrl = pair.m_element1;

         string strExpandedUrl = pair.m_element2;

         strText.replace(strUrl, strExpandedUrl);

         string strEllipsisUrl = strUrl;

         index iFind = strEllipsisUrl.find("://");

         if (iFind >= 0)
         {

            index iFind2 = strEllipsisUrl.find("/", iFind + 3);

            if (iFind2 > iFind)
            {

               for (strsize i = iFind2 + 1; i < strUrl.get_length(); i++)
               {

                  strEllipsisUrl = strUrl.Left(i);

                  strEllipsisUrl += unitext("…");

                  strText.replace(strEllipsisUrl, strExpandedUrl);

               }

            }

         }

         //}

      }

      return strText;

   }

   string_to_string task::get_expanded_url(var & varSource)
   {

      string_to_string & map = m_mapExpandedUrl;

      for (auto set : varSource["entities"].propset())
      {
         for (auto set2 : set.m_element2.propset())
         {
            if (set2.m_element2.has_property("url") && set2.m_element2.has_property("expanded_url"))
            {
               string strUrl = set2.m_element2["url"];
               string strExpandedUrl = set2.m_element2["expanded_url"];
               if (is_expandable_link(strExpandedUrl))
               {

                  strExpandedUrl = expand_link(strExpandedUrl);

               }
               log_source("expand url map (url): " + strUrl + " <- " + strExpandedUrl);
               log_target("expand url map (url): " + strUrl + " <- " + strExpandedUrl);
               map.set_at(strUrl, strExpandedUrl);
            }
         }
      }
      for (auto set : varSource["urls"].propset())
      {
         for (auto set2 : set.m_element2.propset())
         {
            if (set2.m_element2.has_property("url") && set2.m_element2.has_property("expanded_url"))
            {
               string strUrl = set2.m_element2["url"];
               string strExpandedUrl = set2.m_element2["expanded_url"];
               if (is_expandable_link(strExpandedUrl))
               {

                  strExpandedUrl = expand_link(strExpandedUrl);

               }
               log_source("expand url map (url): " + strUrl + " <- " + strExpandedUrl);
               log_target("expand url map (url): " + strUrl + " <- " + strExpandedUrl);
               map.set_at(strUrl, strExpandedUrl);
            }
         }
      }

      return map;

   }


   string_to_string task::get_expanded_photos(var & varSource)
   {

      string_to_string map;

      for (auto set : varSource["entities"]["media"].propset())
      {

         if (set.m_element2.has_property("url") && set.m_element2.has_property("expanded_url"))
         {

            string strUrl = set.m_element2["url"];
            string strExpandedUrl = set.m_element2["expanded_url"];

            log_source("expand url map (photo): " + strUrl + " <- " + strExpandedUrl);
            log_target("expand url map (photo): " + strUrl + " <- " + strExpandedUrl);

            map.set_at(strUrl, strExpandedUrl);

         }

      }

      return map;

   }

   stringa task::get_media_ids(string_to_string & str)
   {

retry:
      stringa stra;
      defer_create_target();
      for (auto pair : str)
      {

         string strPath = pair.m_element1;

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

         {

            synch_lock sl(m_ptaska->mutex());

            cache_path cachepath;

            log_target("Querying cache for URL : \"" + strPath + "\"");

            if (m_ptaska->m_mapCachePath.Lookup(strPath, cachepath)
                  && (get_tick_count() - cachepath.m_dwStart) < 10 * 60 * 1000
                  && file_exists(cachepath.m_strTime)
                  && file_length_dup(cachepath.m_strTime) > 0)
            {

               strTime = cachepath.m_strTime;

               log_target("Found cached file : \"" + strTime + "\"");

            }
            else
            {

               bool bSuccess = false;

               int iTry = 0;

               for (; iTry < 5; iTry++)
               {

                  for (index i = 0; i < 100; i++)
                  {

                     strTime = Context.dir().home() / "Twitter Automator" / "media" / get(data_source) /System.datetime().international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE) + " " + ::str::zero_pad(::str::from(i), 3) + "_" + strName;

                     if (!file_exists(strTime))
                     {

                        break;

                     }

                  }

                  log_target("Going to download to path : \"" + strTime + "\"");

                  property_set set;

                  set["raw_http"] = true;

                  set["disable_common_name_cert_check"] = true;

                  if (!Context.http().download(strPath, strTime, set) || file_length_dup(strTime) <= 0)
                  {

                     log_source("Failed to download media file : " + strPath + " going to retry, attempt no. " + ::str::from(iTry + 1) + " after 5 seconds");

                     log_target("Failed to download media file : " + strPath + " going to retry, attempt no. " + ::str::from(iTry + 1) + " after 5 seconds");

                     Sleep(5000);

                     continue;

                  }

                  log_target("Successfully downloaded " + ::str::from(file_length_dup(strTime)) + " bytes");
                  log_target("Successfully downloaded from URL \"" + strPath + "\"");
                  log_target("Successfully downloaded to path \"" + strTime + "\"");

                  bSuccess = true;

                  cache_path & cachepathSet = m_ptaska->m_mapCachePath[strPath];

                  cachepathSet.m_dwStart = get_tick_count();

                  cachepathSet.m_strTime = strTime;

                  break;

               }

               if (!bSuccess)
               {

                  log_source("Failed to download media file : " + strPath + " after " + ::str::from(iTry) + " attempts");

                  log_target("Failed to download media file : " + strPath + " after " + ::str::from(iTry) + " attempts");

                  continue;

               }

            }

         }

         string strMediaId = m_ptarget->media_upload_noauth(strTime, bGif);

         if(!http_connection_ok(strMediaId, ::twitter_automator::log_target))
         {

            goto retry;

         }

         if (strMediaId.begins_ci("error:"))
         {

            log_target("Failed to upload  media file : " + strTime + "; " + strMediaId);

            continue;

         }

         stra.add(strMediaId);

      }

      return stra;

   }


   bool task::send_photos(var & varSource)
   {

      string strText;

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      return call_tweet("", get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }

   bool task::send_photos_and_text(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      string strId;

      strsize i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);


   }


   bool task::send_photos_and_text_void_link(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      string strId;

      strsize i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }


   bool task::send_only_with_photo_and_text(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      if (mapMedia.is_empty())
      {

         log_source("TWEET IGNORED, No Media");

         return true;

      }

      string_to_string map = get_expanded_url(varSource);

      string strId;

      strsize i = 0;

      stringa straIgnoreLinkIgnoredDomain;

      straIgnoreLinkIgnoredDomain = m_straIgnoreLinkIgnoredDomain;

      straIgnoreLinkIgnoredDomain.surround("://", "/");

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         string strLink = strText.Mid(i, iEnd - i);

         string strLink2 = map[strLink];

         bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
         {
            return strLink2.contains_ci(str);
         }) >= 0;

         if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         string strLink = strText.Mid(i, iEnd - i);

         string strLink2 = map[strLink];

         bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
         {
            return strLink2.contains_ci(str);
         }) >= 0;

         if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      if (strText.trimmed().is_empty())
      {

         log_source("TWEET IGNORED, No Text");

         return true;

      }

      strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }


   bool task::send_media_or_text(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      //if (mapMedia.is_empty())
      //{

      //   log_source("TWEET IGNORED, No Media");

      //   return true;

      //}

      string_to_string map = get_expanded_url(varSource);

      string strId;

      strsize i = 0;

      stringa straIgnoreLinkIgnoredDomain;

      straIgnoreLinkIgnoredDomain = m_straIgnoreLinkIgnoredDomain;

      straIgnoreLinkIgnoredDomain.surround("://", "/");

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         string strLink = strText.Mid(i, iEnd - i);

         string strLink2 = map[strLink];

         bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
         {
            return strLink2.contains_ci(str);
         }) >= 0;

         if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }
         else if (bIgnore)
         {

            i += strLink.get_length();

         }
         else
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         string strLink = strText.Mid(i, iEnd - i);

         string strLink2 = map[strLink];

         bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
         {
            return strLink2.contains_ci(str);
         }) >= 0;

         if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }
         else if (bIgnore)
         {

            i += strLink.get_length();

         }
         else
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }

      }

      //if (strText.trimmed().is_empty())
      //{

      //   log_source("TWEET IGNORED, No Text");

      //   return true;

      //}

      strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }


   bool task::send_photos_text_and_one_link(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      int iLinkCount = 0;

      string strId;

      strsize i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            iLinkCount++;

            if (iLinkCount == 1)
            {

               i++;

            }
            else
            {

               log_source("TWEET IGNORED, Link Count Greater than 1");

               return true;

            }

         }
         else
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            iLinkCount++;

            if (iLinkCount == 1)
            {

               i++;

            }
            else
            {

               log_source("TWEET IGNORED, Link Count Greater than 1");

               return true;

            }

         }
         else
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }

      }

      if (iLinkCount == 0)
      {

         log_source("TWEET IGNORED, Link Count 0");

         return true;

      }

      strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }


   bool task::send_only_with_photo__text_and_link(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      if (mapMedia.is_empty())
      {

         log_source("TWEET IGNORED, No Media");

         return true;

      }

      int iLinkCount = 0;

      string strTextNoLinks = strText;

      strsize i = 0;

      {

         while ((i = strTextNoLinks.find_ci("http://", i)) >= 0)
         {

            strsize iEnd = strTextNoLinks.find_whitespace_or_length(i + 1);

            strTextNoLinks = strTextNoLinks.Left(i) + strTextNoLinks.Mid(iEnd);

         }

         i = 0;

         while ((i = strTextNoLinks.find_ci("https://", i)) >= 0)
         {

            strsize iEnd = strTextNoLinks.find_whitespace_or_length(i + 1);

            strTextNoLinks = strTextNoLinks.Left(i) + strTextNoLinks.Mid(iEnd);

         }

      }

      if (strTextNoLinks.trimmed().is_empty())
      {

         log_source("TWEET IGNORED, No Text");

         return true;

      }

      string strId;

      i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            iLinkCount++;

            i++;

         }
         else
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            iLinkCount++;

            i++;

         }
         else
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }

      }

      if (iLinkCount == 0)
      {

         log_source("TWEET IGNORED, No Links");

         return true;

      }

      strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }


   bool task::send_text(var & varSource)
   {

      string strText = varSource["full_text"];

      strsize i;

      i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      return call_tweet(strText, {}, varSource);

   }


   bool task::send_only_if_text(var & varSource)
   {

      string strText = varSource["full_text"];

      if(strText.find_ci("http://") >= 0)
      {

         log_source("TWEET IGNORED, Found link or photo");

         return true;

      }

      if(strText.find_ci("https://") >= 0)
      {

         log_source("TWEET IGNORED, Found link or photo");

         return true;

      }

      return call_tweet(strText, {}, varSource);

   }


   bool task::send_all(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      string strId;

      strsize i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }
         else
         {

            i++;

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }
         else
         {

            i++;

         }

      }

      strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }


   bool task::send_all_non_expansion(var & varSource)
   {

      string strText = varSource["full_text"];

      string_to_string mapMedia = get_photos_with_media_id(varSource);

      string strId;

      strsize i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }
         else
         {

            i++;

         }

      }

      i = 0;

      while ((i = strText.find_ci("https://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
         {

            strText = strText.Left(i) + strText.Mid(iEnd);

         }
         else
         {

            i++;

         }

      }

      //strText = expand_url(strText, varSource);

      return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)), varSource);

   }

   string task::perform_url_processing(string str)
   {

      str = perform_domain_find_replace(str);

      return str;

   }

   bool task::call_tweet(string strText, stringa straMediaIds, var & varSource)
   {

      log_target("call_tweet: \"" + strText + "\"");

      strText = on_clean_message(strText, varSource);

      strText = m_ptaska->perform_url_processing(strText);

      strText = perform_url_processing(strText);

      strText = hashtag_first_link(strText);

      return send_tweet(strText, straMediaIds);

   }

   string task::hashtag_first_link(string strText)
   {

      synch_lock sl(mutex());

      string strTag = get(data_hashtag);

      ::str::begins_eat_ci(strTag, "#");

      if (strTag.has_char())
      {

         int iFind1 = strText.find_ci("http://");
         int iFind2 = strText.find_ci("https://");
         int iFind = min_non_neg(iFind1, iFind2);

         strText = strText.Left(iFind) + " #" + strTag + " " + strText.Mid(iFind);

      }

      return strText;

   }



   bool task::send_tweet(string strText, stringa straMediaIds, index iAttempt)
   {

retry:

      log_target("send_tweet: \"" + strText + "\"");

      target();

      string strReply = m_ptarget->twitter_automator_noauth(strText, straMediaIds);

      if (!http_connection_ok(m_ptarget->m_strLastConnectionStatus, ::twitter_automator::log_target))
      {

         goto retry;

      }


      if (strReply.is_empty())
      {

         if (m_ptarget->m_strLastError.contains_ci("Won't tweet empty status"))
         {

            log_target("Ignoring error \"Won't tweet empty status\"!!");

            return true;

         }
         else
         {

            log_target("Ignoring empty response! (Connection Problem? Server Error?) Considered Temporary.");

            return true;

         }


      }

      var v;

      const char * psz = strReply;

      try
      {

         v.parse_json(psz);

      }
      catch (...)
      {

         log_target("EXCEPTION parsing: \"" + strReply + "\"");

      }

      if (strReply.is_empty() || v.is_new() || v.has_property("errors"))
      {

         if (v["errors"].at(0)["code"].i32() == 408)
         {

            // "The given URL is invalid." (Error: 408)
            // generally happens with links terminated with …

            log_target("IGNORABLE: Failure to tweet?! (1.1)");

            log_target("Ignoring \"The given URL is invalid.\" (Error: 408)");

            return true;

         }
         else if (v["errors"].at(0)["code"].i32() == 186)
         {

            // "Status is over 140 characters." (Error: 186)
            // Current handling is to ignore error and continue as the message was actually tweeted.
            // Probable future more advanced handling would be to refetch the original message again and process it again
            // and try to copy the tweet again for a couple of attempts (Rationale: Twitter, on error(bug), seems to
            // ocasionally miss the URL expansion information for some links. This could be noticed by analyzing the
            // logs for the same tweet accross multiple fetchs using different API codes, accounts and proxies).

            log_target("IGNORABLE: Failure to tweet!! (1.0)");

            log_target("Ignoring \"Status is over 140 characters.\" (Error: 186)");

            return true;

         }
         else if (v["errors"].at(0)["code"].i32() == 187)
         {

            // "Status is a duplicate." (Error: 187)
            // Could not trace back why this error occurs currently only
            // at one of one hundred similar accounts.
            // Safely can be ignored because it is a duplicate anyway.
            // (Besides it is not consecutive duplicate near close ago. The logged cases would lead to very old
            // collision/duplication, so I don't know why this particular account accuses duplicate from so old messages
            // why 99 other accounts seems ok with it...) But as we concluded, it is safely ignorable...

            log_target("IGNORABLE: Failure to tweet!! (1.1)");

            log_target("Ignoring \"Status is a duplicate.\" (Error: 187)");

            return true;

         }
         else if (v["errors"].at(0)["code"].i32() == 324 && iAttempt <= 1)
         {

            string strMessage = v["errors"].at(0)["message"];

            strsize iFind = strMessage.find_ci("MediaId: snf:");

            string strMedia;

            if (iFind >= 0)
            {

               strsize iFind2 = strMessage.find(')', iFind);

               if (iFind2 > iFind)
               {

                  strMedia = strMessage.Mid(iFind, iFind2 - iFind - 1);

               }

            }

            log_target("media too short, going to retry send with photo, instead of video: mediaid: " + strMedia);

            return send_tweet(strText, get_media_ids(m_straPhoto), iAttempt+1);

         }
         else if (v["errors"].at(0)["code"].i32() == 130)
         {

            if (iAttempt < 3)
            {

               log_target("OVER CAPACITY: Attempt no. " + ::str::from(iAttempt + 1));

               auto iSeconds = (iAttempt + 1) * 10;

               log_target("OVER CAPACITY: Going to retry after about " + ::str::from(iSeconds) + " seconds");

               for (index i = 0; i < iSeconds * 10 && thread_get_run(); i++)
               {

                  Sleep(100);

               }

               if (!thread_get_run())
               {

                  return false;

               }

               return send_tweet(strText, straMediaIds, iAttempt + 1);

            }

            log_target("OVER CAPACITY: Failed to send message after "+::str::from(iAttempt) +" with growing delay attempts");

            log_target("OVER CAPACITY: Message ignored!");

            return true;

         }
         else if (v["errors"].at(0)["code"].i32() == 131)
         {

            if (iAttempt < 5)
            {

               log_target("INTERNAL SERVER ERROR: Attempt no. " + ::str::from(iAttempt + 1));

               auto iSeconds = (iAttempt + 1) * 10;

               log_target("INTERNAL SERVER ERROR: Going to retry after about " + ::str::from(iSeconds) + " seconds");

               for (index i = 0; i < iSeconds * 10 && thread_get_run(); i++)
               {

                  Sleep(100);

               }

               if (!thread_get_run())
               {

                  return false;

               }

               return send_tweet(strText, straMediaIds, iAttempt + 1);

            }

            log_target("INTERNAL SERVER ERROR: Failed to send message after " + ::str::from(iAttempt) + " with growing delay attempts");

            log_target("INTERNAL SERVER ERROR: Message ignored!");

            return true;

         }
         else if (v["errors"].at(0)["code"].i32() == 32)
         {
            // Could not authenticate you.

            if (iAttempt < 3)
            {

               log_target("COULD NOT AUTHENTICATE YOU: Attempt no. " + ::str::from(iAttempt + 1));

               int iSeconds = 60;

               log_target("COULD NOT AUTHENTICATE YOU: Going to retry after about " + ::str::from(iSeconds) + " seconds");

               for (index i = 0; i < iSeconds * 10 && thread_get_run(); i++)
               {

                  Sleep(100);

               }

               if (!thread_get_run())
               {

                  return false;

               }

               return send_tweet(strText, straMediaIds, iAttempt + 1);

            }

            log_target("COULD NOT AUTHENTICATE YOU: Failed to send message after " + ::str::from(iAttempt) + " attempts");

            log_target("COULD NOT AUTHENTICATE YOU: Failure.");

            return false;

         }
         else
         {

            log_target("Failure to tweet!! (2)");

            return false;

         }

      }

      return true;

   }


   twitter * task::target()
   {

      defer_create_target();

      return m_ptarget;

   }

   void task::defer_create_target()
   {

      if (m_ptarget == NULL || get_tick_count() - m_dwLastAuth > (TARGET_SECONDS_RECONNECT * 1000))
      {

         m_dwLastAuth = get_tick_count();

         ::aura::del(m_ptarget);

         m_ptarget = create_target();

      }

   }

   twitter * task::create_target()
   {

      twitter * ptarget = new twitter(get_context_application(), this, ::twitter_automator::log_target);

      ptarget->m_strTwitterConsumerKey = get(data_target_api_consumer_key);
      ptarget->m_strTwitterConsumerSecret = get(data_target_api_consumer_sec);
      ptarget->m_strOAuthAccessTokenKey = get(data_target_api_token_key);
      ptarget->m_strOAuthAccessTokenSecret = get(data_target_api_token_sec);
      ptarget->m_strProxy = get(data_target_proxy);

      return ptarget;

   }



   void task::simple_csv_set_line(stringa & stra)
   {

      stra.set_size(data_count);

      //if (stra.safe_at(data_same_consumer).is_empty())
      //{

      //   stra.set_at_grow(data_same_consumer, "on");

      //}

      //if (stra.safe_at(data_same_proxy).is_empty())
      //{

      //   stra.set_at_grow(data_same_proxy, "on");

      //}

      bool bStatusChanged = stra.safe_at(data_status).compare_ci(m_stra.safe_at(data_status)) != 0;

      bool bStatusOn = stra.safe_at(data_status).compare_ci("ON") == 0;

      if (bStatusChanged && !bStatusOn)
      {

         set(data_status, "off");

      }


      for (index i = 0; i < stra.get_size(); i++)
      {

         if (i != data_status)
         {

            set((e_data)i, stra[i]);

         }

      }

      if (bStatusChanged && !bStatusOn)
      {

         set(data_status, "on");

      }


   }


   void task::simple_csv_get_line(stringa & stra)
   {

      stra.set_size(m_stra.get_size());

      for (index i = 0; i < stra.get_size(); i++)
      {

         stra.set_at_grow(i, get((e_data)i));

      }

   }


   string task::get(e_data edata)
   {

      synch_lock sl(mutex());

      return m_stra[(index)edata];

   }


   void task::set(e_data edata, const string & str)
   {

      if (edata == data_status)
      {

         m_dwLastStatusChange = get_tick_count();

      }

      {

         synch_lock sl(mutex());

         m_stra.set_at_grow((index)edata, str);

      }

      if (edata == data_status)
      {

         if (str.compare_ci("ON") == 0)
         {

            m_estatusRequest = status_on;

         }
         else
         {

            m_estatusRequest = status_off;

         }

      }

      m_ptaska->m_pcontainer->on_status_changed(m_iIndex);

   }


   bool task::check_data()
   {

      synch_lock sl(mutex());

      if (get(data_status) != "on")
      {

         return false;

      }
      string strError;
      if (get(data_source).is_empty())
      {
         strError = "Source cannot be empty";

      }
      if (get(data_source_api_consumer_key).is_empty())
      {

         strError = "Source Consumer Key cannot be empty";

      }
      if (get(data_source_api_consumer_sec).is_empty())
      {

         strError = "Source Consumer Secret cannot be empty";

      }
      if (get(data_target_api_consumer_key).is_empty())
      {

         strError = "Target Consumer Key cannot be empty";

      }
      if (get(data_target_api_consumer_sec).is_empty())
      {

         strError = "Target Consumer Secret cannot be empty";

      }
      if (get(data_target_api_token_key).is_empty())
      {

         strError = "Target Token Key cannot be empty";

      }
      if (get(data_target_api_token_sec).is_empty())
      {

         strError = "Target Token Secret cannot be empty";

      }
      if (get(data_option).is_empty())
      {

         strError = "Copy Mode cannot be empty";

      }

      if (strError.has_char())
      {

         ::fork(get_context_application(), [=]()
         {

            MessageBox(NULL, strError, "Twitter Automator", MB_ICONEXCLAMATION);

         });

         general_log(strError);

         return false;

      }

      return true;

   }


   void task::log(string strModifier, string str)
   {

      string strMessage;

      string strCurDate;

      strCurDate = System.datetime().international().get_gmt_date_time("%Y-%m-%d");

      if (strCurDate != m_strLogDate)
      {

         m_strLogDate = strCurDate;

         m_iLogIndex = 0;

         if (str != "NEW TASK RUN!!")
         {

            log_line("NEW UTC DAY!!", ::twitter_automator::log_source);
            log_line("NEW UTC DAY!!", ::twitter_automator::log_target);
            log_line("NEW UTC DAY!!", ::twitter_automator::log_general);

            log_header(::twitter_automator::log_source);
            log_header(::twitter_automator::log_target);
            log_header(::twitter_automator::log_general);

         }

      }

      strMessage += System.datetime().international().get_gmt_date_time();
      strMessage += " ";
      strMessage += str;
      strMessage += "\r\n";

      for (; m_iLogIndex <= 9999; m_iLogIndex++)
      {

         if (file_append_wait_dup(log_path(strModifier), strMessage, strMessage.get_length(), 1000))
         {

            break;

         }

      }

   }


   void task::log_source(string str)
   {

      log("source", str);

   }


   void task::log_target(string str)
   {

      log("target", str);

   }

   void task::general_log(string str)
   {

      log("general", str);

   }


   ::file::path task::dir_path()
   {

      ::file::path path;

      string str = ::str::zero_pad(::str::from(m_iIndex + 1), 3);

      if (get(data_hint_target).has_char())
      {

         str += " " + get(data_hint_target);

      }

      path = Context.dir().home() / "Twitter Automator" / str;

      return path;

   }


   string task::log_path(string strModifier)
   {

      string strLogIndex = ::str::zero_pad(::str::from(m_iLogIndex), 4);

      return dir_path() / System.datetime().international().get_gmt_date_time("%Y-%m-%d") + " "+strLogIndex + " "+strModifier+".txt";

   }


   //string task::source_path()
   //{

   //   return log_path("source");

   //}


   //string task::target_path()
   //{


   //   return log_path("target");

   //}


   void task::log_header(index iLogTarget)
   {

      stringa stra = log_header();

      for (auto & str : stra)
      {

         log_line(str, iLogTarget);

      }

   }


   stringa task::log_header()
   {

      stringa stra;

      stra.add("|");
      stra.add("-");
      stra.add("--");
      stra.add("---");
      stra.add("LOG_HEADER: INDEX: " + ::str::zero_pad(::str::from(m_iIndex + 1), 3));
      stra.add("LOG_HEADER: SOURCE: " + get(data_source));
      string strTarget;

      if (get(data_hint_target).has_char())
      {

         strTarget = get(data_hint_target);

      }
      else
      {

         string str = get(data_target_api_token_key);

         auto iFind = str.find('-');

         if (iFind > 0)
         {

            strTarget = str.Left(iFind);

         }
         else
         {

            strTarget = str;

         }

      }

      stra.add("LOG_HEADER: TARGET: " + strTarget);
      stra.add("---");
      stra.add("--");
      stra.add("-");
      stra.add("|");

      return stra;

   }


   void task::inside_loop_tests()
   {

      ::file::path t = Context.dir().home() / "Twitter Automator" / "CrashDumps/test_dump.txt";
      if (file_exists(t))
      {

         throw simple_exception(get_context_application(), "rethrow");


      }

      if (1)
      {

         return;

      }

      if (1)
      {

         property_set set;

         set["raw_http"] = true;

         set["redirect_location"] = "";

         Context.http().get("http://amzn.to/2pYtrYf", set);

         string strRedirectLink = set["redirect_location"];

         return;

      }

      if (0)

      {

         defer_create_target();

         m_ptarget->m_strOAuthAccessTokenKey.Empty();

         m_ptarget->m_strOAuthAccessTokenSecret.Empty();

         m_ptarget->auth();

         Sleep(100000);

      }

      if (0)
      {

         ::hi5::twit twitterObj(get_context_application(), this, ::twitter_automator::log_target);



         string tmpStr("");
         string replyMsg("");

         /* OAuth flow begins */
         /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
         twitterObj.get_oauth().setConsumerKey(get(data_target_api_consumer_key));
         twitterObj.get_oauth().setConsumerSecret(get(data_target_api_consumer_sec));

         twitterObj.get_oauth().setOAuthTokenKey(get(data_target_api_token_key));
         twitterObj.get_oauth().setOAuthTokenSecret(get(data_target_api_token_sec));

         string str("tweet xyz no. 273");

         ::file::file_pointer pfile = Context.file().get_file("C:\\archive\\abc.png", ::file::type_binary | ::file::mode_read | ::file::share_deny_none);

         string strMediaId = twitterObj.mediaUpload(pfile, "image/png", "");

         twitterObj.statusUpdate(str, { strMediaId });

         output_debug_string(strMediaId);

         Sleep(600000);
      }

      if (0)
      {

         ::hi5::twit twitterObj(get_context_application(), this, ::twitter_automator::log_target);



         string tmpStr("");
         string replyMsg("");

         /* OAuth flow begins */
         /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
         twitterObj.get_oauth().setConsumerKey(get(data_target_api_consumer_key));
         twitterObj.get_oauth().setConsumerSecret(get(data_target_api_consumer_sec));

         twitterObj.get_oauth().setOAuthTokenKey(get(data_target_api_token_key));
         twitterObj.get_oauth().setOAuthTokenSecret(get(data_target_api_token_sec));

         //string str1("{\"full_text\":\"001 Tom Clancy\\u2019s Ghost Recon Wildlands Deluxe Edition PC for only $52.89!\"");

         string str1 = Context.file().as_string("C:\\archive\\1.json");

         const char * psz = str1;

         var v;

         try
         {

            v.parse_json(psz);

         }
         catch (...)
         {

            log_source("DEBUGG EXCEPTION parsing: \"" + str1 + "\"");

         }
         string str = v["full_text"];

         //         string tmpStr;
         tmpStr = str;
         tmpStr.replace("&gt;", ">");
         tmpStr.replace("&lt;", "<");
         tmpStr.replace("&amp;", "&");
         string strRes;

         str = str.Left(60);

         if (1)
         {
            const char * psz = tmpStr;
            while (*psz)
            {
               string strChar = ::str::get_utf8_char(psz);
               if (strChar.is_empty())
               {
                  break;
               }
               else if (strChar.get_length() == 1)
               {
                  strRes += strChar;
               }
               else
               {
                  u32 uiUniIndex = (u32)::str::ch::uni_index(strChar);
                  if (is_surrogated(uiUniIndex))
                  {
                     u16 ui[2];
                     encode_utf16_pair(uiUniIndex, ui);
                     strRes += "\\u" + ::hex::lower_from(ui[0]).Right(4);
                     strRes += "\\u" + ::hex::lower_from(ui[1]).Right(4);
                  }
                  else
                  {
                     strRes += "\\u" + ::hex::lower_from(uiUniIndex).Right(4);
                  }
               }
               psz += strChar.get_length();
            }
         }
         //::file::file_pointer pfile = Context.file().get_file("C:\\archive\\abc.png", ::file::type_binary | ::file::mode_read | ::file::share_deny_none);

         //string strMediaId = twitterObj.mediaUpload(pfile, "image/png", "");

         twitterObj.statusUpdate(strRes, {});

         //output_debug_string(strMediaId);

         Sleep(600000);
      }

      if (0)
      {

         ::hi5::twit twitterObj(get_context_application(), this, ::twitter_automator::log_target);



         string tmpStr("");
         string replyMsg("");

         /* OAuth flow begins */
         /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
         twitterObj.get_oauth().setConsumerKey(get(data_target_api_consumer_key));
         twitterObj.get_oauth().setConsumerSecret(get(data_target_api_consumer_sec));

         twitterObj.get_oauth().setOAuthTokenKey(get(data_target_api_token_key));
         twitterObj.get_oauth().setOAuthTokenSecret(get(data_target_api_token_sec));

         //string str1("{\"full_text\":\"001 Tom Clancy\\u2019s Ghost Recon Wildlands Deluxe Edition PC for only $52.89!\"");

         string str1 = Context.file().as_string("C:\\archive\\2.json");

         const char * psz = str1;

         var v;

         try
         {

            v.parse_json(psz);

         }
         catch (...)
         {

            log_source("DEBUGG (2) EXCEPTION parsing: \"" + str1 + "\"");

         }



         string str = v.at(0)["full_text"];

         //         string tmpStr;
         tmpStr = str;
         tmpStr.replace("&gt;", ">");
         tmpStr.replace("&lt;", "<");
         tmpStr.replace("&amp;", "&");
         string strRes = str;

         strRes = strRes.Left(90);

         if (0)
         {

            const char * psz = tmpStr;

            while (*psz)
            {

               string strChar = ::str::get_utf8_char(psz);

               if (strChar.is_empty())
               {

                  break;

               }
               else if (strChar.get_length() == 1)
               {

                  strRes += strChar;

               }
               else
               {

                  int iUniIndex = (int) ::str::ch::uni_index(strChar);

                  strRes += "\\u" + ::hex::lower_from(iUniIndex).Right(4);

               }

               psz += strChar.get_length();

            }

         }
         //::file::file_pointer pfile = Context.file().get_file("C:\\archive\\abc.png", ::file::type_binary | ::file::mode_read | ::file::share_deny_none);

         //string strMediaId = twitterObj.mediaUpload(pfile, "image/png", "");

         twitterObj.statusUpdate(strRes, {});

         //output_debug_string(strMediaId);

         Sleep(600000);
      }


   }


   void task::log_line(string str, index iLogTarget)
   {

      if (iLogTarget == ::twitter_automator::log_source)
      {

         log_source("LogLnSrc: " + str);

      }
      else if (iLogTarget == ::twitter_automator::log_target)
      {

         log_target("LogLnDst: " + str);

      }
      else
      {

         general_log("LogLnGen: " + str);

      }

   }

   string task::on_clean_message(string strText, var & varSource)
   {

      strsize iFind = -1;

      while (true)
      {

         iFind = strText.find('@', iFind + 1);

         if (iFind < 0)
         {

            break;

         }

         if (iFind + 1 == strText.length())
         {

            break;

         }

         index iEnd = iFind + varSource["user"]["screen_name"].get_string().length() + 1;

         if (iFind - 3 == strText.find("RT @" + varSource["user"]["screen_name"], MAX(0, iFind - 3))
               && (strText[iEnd] == ' '
                   || strText[iEnd] == '\r'
                   || strText[iEnd] == '\n'
                   || strText[iEnd] == ':'
                   || strText[iEnd] == ','
                   || strText[iEnd] == '.'
                  ))
         {

            strText = strText.Left(iFind - 3) + strText.Mid(iEnd + 1);

            strText.trim_left();

         }

      }



      return remove_hashtags(strText);

   }

   string task::remove_hashtags(string strText)
   {

      strsize iFind = 0;

      while (true)
      {

         iFind = strText.find('#', iFind);

         if (iFind < 0)
         {

            break;

         }

         strsize iFind2 = strText.find_whitespace_or_length(iFind);

         strText = strText.Left(iFind) + strText.Mid(iFind2);

      }

      return strText;

   }


   bool task::http_connection_ok(string strConnectionStatus, index iLogTarget)
   {

      if (strConnectionStatus.begins_ci("ERROR: HTTP_CONNECTION_ERROR: "))
      {

         m_iHttpConnectionError++;

         if (strConnectionStatus.contains_ci("Connection Time Out")
               || strConnectionStatus.contains_ci("Timed Out"))
         {

            m_iConnectionTimeoutError++;

            if (m_iConnectionTimeoutError >= MAX_TIMEOUT_ERROR)
            {

               string strMessage;

               strMessage = "maximum consecutive connection timeout error count (" + ::str::from(m_iConnectionTimeoutError) + ")reached: stopping task.";

               log_line(strMessage, iLogTarget);

               throw http_connection_error(strMessage);

            }


            log_line("connection timeout ignored: retry no. " + ::str::from(m_iConnectionTimeoutError) + "considered temporary (3.1).", iLogTarget);

            Sleep(1000);

            return false;

         }

         m_iConnectionTimeoutError = 0;

         if (m_iHttpConnectionError >= MAX_HTTP_ERROR)
         {

            string strMessage;

            strMessage = "maximum consecutive http connection error count (" + ::str::from(m_iHttpConnectionError) + ")reached: stopping task.";

            log_line(strMessage, iLogTarget);

            throw http_connection_error(strMessage);

         }

         log_line("http connection error ignored: retry no. "+ ::str::from(m_iHttpConnectionError)+ " considered temporary (3.2).", iLogTarget);

         Sleep(1000);

         return false;

      }

      m_iConnectionTimeoutError = 0;

      m_iHttpConnectionError = 0;

      return true;

   }

   string task::expand_link(string strLink, index iLog)
   {

      synch_lock sl(m_ptaska->mutex());

      {

         class expand_link expandlink;

         if (m_ptaska->m_mapExpandLink.Lookup(strLink, expandlink)
               && (get_tick_count() - expandlink.m_dwStart) < 5 * 60 * 1000)
         {

            // return cached value

            return expandlink.m_strExpanded;

         }

      }

      string strRedirectLink;

      int iTry = 0;

      log_line("expand_link: \"" + strLink + "\"", iLog);

      while (iTry < 5)
      {

         property_set set;

         set["raw_http"] = true;

         set["redirect_location"] = "";

         string strProxy = get(data_target_proxy);

         if (strProxy.has_char())
         {

            //set["proxy"] = strProxy;

         }

         Context.http().get(strLink, set);

         string strRedirectLink = set["redirect_location"];

         if (strRedirectLink.has_char())
         {

            log_line("Found redirect location: \"" + strRedirectLink + "\"", iLog);

            class expand_link & expandlink = m_ptaska->m_mapExpandLink[strLink];

            expandlink.m_dwStart = get_tick_count();

            expandlink.m_strExpanded = strRedirectLink;

            return strRedirectLink;

         }

         iTry++;

         log_line("redirect location empty going to retry no. " + ::str::from(iTry), iLog);

         Sleep(1000);

      }

      log_line("redirect location empty maximum attempts " + ::str::from(iTry), iLog);

      log_line("using original link \""+strLink+"\"", iLog);

      class expand_link & expandlink = m_ptaska->m_mapExpandLink[strLink];

      expandlink.m_dwStart = get_tick_count();

      expandlink.m_strExpanded = strLink;

      return strLink;

   }

   bool task::is_expandable_link(string strLink)
   {

      string strExpression;

      for (auto & str : m_straExpandDomain)
      {

         strExpression = "://" + str + "/";

         if (strLink.contains_ci(strExpression))
         {

            return true;

         }

      }

      return false;

   }


   void task::defer_load_domain_find_replace_file()
   {

      ::file::path pathDomainFindReplace = dir_path() / "domain_find_replace.txt";

      if (!file_exists(pathDomainFindReplace))
      {

         Context.file().put_contents_utf8(pathDomainFindReplace, Context.file().as_string("matter://default_domain_find_replace.txt"));

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


   void task::parse_domain_find_replace(string str)
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

   string task::perform_domain_find_replace(string strText)
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



} // namespace twitter_automator







