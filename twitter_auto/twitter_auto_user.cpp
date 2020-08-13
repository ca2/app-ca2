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

namespace twitter_auto
{

   user::user(user_array * ptaska) :
      object(ptaska->get_context_application()),
      ::thread(ptaska->get_context_application())
   {

      m_iConnectionTimeoutError = 0;


      m_iHttpConnectionError = 0;

      m_ptaska = ptaska;

      m_stra.set_size(data_count);

      set(data_status, "off");
      set(data_target, "Source");
      set(data_hint_username, "Target Hint");
      set(data_delay,  "10");
      set(data_reset, "99");

      m_straIgnoreTweetDomain.add("fatkiddeals.com");
      //m_straIgnoreTweetDomain.add("twitter.com/fatkiddeals/");
      m_straIgnoreTweetDomain.add("twitter.com");

      m_straExpandDomain.add("amzn.to");
      m_straExpandDomain.add("ebay.to");
      m_straExpandDomain.add("bit.ly");


      m_straIgnoreLinkIgnoredDomain.add("vine.co");


   }

   user::~user()
   {


   }


   void user::run()
   {

      m_iTweetCount = 0;
      log_source("NEW TASK RUN!!");
      log_target("NEW TASK RUN!!");
      general_log("NEW TASK RUN!!");

      log_header(::twitter_auto::log_source);
      log_header(::twitter_auto::log_target);
      log_header(::twitter_auto::log_general);

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

               twitter twitterSource(get_context_application(), this, ::twitter_auto::log_source);

               twitterSource.m_strTwitterConsumerKey = get(data_source_api_consumer_key);
               twitterSource.m_strTwitterConsumerSecret = get(data_source_api_consumer_sec);
               twitterSource.m_strProxy = get(data_source_proxy);

               var varSource = twitterSource.last_twitter_automator(get(data_target));

               if (!http_connection_ok(twitterSource.m_strLastConnectionStatus, ::twitter_auto::log_source))
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

                     varSource = twitterSource.next_twitter_automators(get(data_target), strSinceId);

                  }

                  if (!http_connection_ok(twitterSource.m_strLastConnectionStatus, ::twitter_auto::log_source))
                  {

                     continue;

                  }

                  log_source("debug varSource (2) " + varSource.get_json(false));

                  if (twitterSource.m_strLastError.begins_ci("ERROR: ")
                        || varSource.is_new()
                        || varSource.has_property("errors"))
                  {

                     if (varSource["errors"].at(0)["code"].int32() == 44)
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

                        if (!send(varSource.at(i)))
                        {

                           general_log("exiting (4)");

                           return;

                        }

                        i--;

                     }

                     if (iGoodSinceId >= 0 && iGoodSinceId < varSource.get_count())
                     {

                        strSinceId = varSource.at(iGoodSinceId)["id_str"];

                     }

                  }

                  int iWait = 100;

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

            general_log("fatal_HTTP_CONNECTION_ERROR: Task stopped: Account: \"" + get(data_hint_username) + "\": " + e.m_str);

            m_error.set(-34);

            return;

         }
         catch (::exception::exception & e)
         {

            log_source("Exception in twitter_auto::user::run (2)");

            log_source(e.what());

            bOkSession = false;

            if (::str::find_ci("rethrow", e.what())>= 0)
            {

               throw e;

            }

         }
         catch (...)
         {

            general_log("Unknown exception in twitter_auto::user::run(2)");

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


   bool user::send(var & varSourceParam)
   {

      var varSourceCopy(varSourceParam);

      fork([=]()
      {

         var varSource(varSourceCopy);

         string strDataReset = get(data_reset);

         int iResetCount = atoi(strDataReset);

         if (iResetCount >= 1)
         {

            ::datetime::time timeNow = ::datetime::time::get_current_time();

            timeNow.m_time = (timeNow.m_time / 3600) * 3600;

            if (timeNow != m_timeResetCount)
            {

               m_timeResetCount = timeNow;

               m_iTweetCount = 0;

            }
            else
            {

               m_iTweetCount++;

               if (m_iTweetCount >= iResetCount)
               {

                  log_source("IGNORING reply - Tweet reset count hour limit reached " + ::str::from(m_iTweetCount) + " from " + get(data_reset));
                  log_target("IGNORING reply - Tweet reset count hour limit reached " + ::str::from(m_iTweetCount) + " from " + get(data_reset));

                  return;

               }

            }

         }

         if (!is_source_ok(varSource))
         {

            log_source("IGNORING RETWEET");

            log_target("IGNORING RETWEET");

            return ;

         }

         __pointer(twitter) ptwitter = create_target();

         string strPath = get(data_image);

         stringa straMedia;

         if (Context.dir().is(strPath))
         {

            ::file::listing ls(get_context_application());

            stringa straPattern;

            straPattern.add("*.png");
            straPattern.add("*.jpg");
            straPattern.add("*.gif");

            ls.ls_pattern_file(strPath, straPattern);

            if (ls.is_empty())
            {


               ::std::stringstream strError;

               string strLine = "(Line " + str::from(m_iIndex + 1) + ")";

               strError << "No image Found on folder (" << straPattern.implode(",") << ") " << strLine << ::std::endl;

               synch_lock sl(&m_ptaska->m_mutexError);

               m_ptaska->m_strError << strError;

               return;

            }

            ls.sort();

            index iFind = ls.reverse_find_ci(get_current_image());

            strPath = (iFind + 1) % ls;

            set_current_image(strPath);

         }

         if (strPath.has_char())
         {

            straMedia = m_ptaska->path_get_media_ids(this, ptwitter, { strPath });

         }

         string strMessage = get(data_message);

         string strTarget = "@" + get(data_target);

         strMessage.replace("$NAME", strTarget);

         string strStatusId = varSource["id"];

         strMessage = strTarget + " " + strMessage;

         int iDelay = atoi(get(data_delay));

         for (int i = 0; i < iDelay && thread_get_run(); i++)
         {

            Sleep(1000);
         }

         if (!thread_get_run())
         {
            return;
         }

         call_tweet(ptwitter, strMessage, straMedia, strStatusId);

      });

      return true;

      //return send_photos_text_and_one_link(varSource);

      //log_source("OPTION: " + get(data_option));
      //log_source("OPTION: " + get(data_option));
      //if (get(data_option) == "Copy One Link")
      //{

      //   return send_photos_text_and_one_link(varSource);

      //}
      //else if (get(data_option) == "Copy Photos Text and Videos")
      //{

      //   return send_media_or_text(varSource);

      //}
      //else if (get(data_option) == "Copy Text")
      //{

      //   return send_only_if_text(varSource);

      //}
      //else if (get(data_option) == "Copy All")
      //{

      //   return send_all(varSource);

      //}
      //else if (get(data_option) == "Copy Bit.ly")
      //{

      //   return send_all_non_expansion(varSource);

      //}
      //else if (get(data_option) == "Copy Link")
      //{

      //   return send_only_with_photo__text_and_link(varSource);

      //}
      //else
      //{
      //   log_source("Unknown Option");
      //   log_target("Unknown Option");
      //   return false;

      //}

   }

   bool user::is_source_ok(var & varSource)
   {

      if (varSource.has_property("retweeted_status"))
      {

         string str = varSource["retweeted_status"];

         log_source("Source NOK: Retweet");

         return false;

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

         if (strText[iFind + 1] != ' '
               && strText[iFind + 1] != '\r'
               && strText[iFind + 1] != '\n')
         {

            log_source("Source NOK: mention");

            return false;

         }

      }

      return true;

   }

   string_to_string user::get_photos_with_media_id(var & varSource)
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

   string_to_string user::get_media_url_media_ids(var & varSource)
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

               if (photo["video_info"]["variants"].at(iVariant)["bitrate"].int64() > iBitRate)
               {

                  iBitRate = photo["video_info"]["variants"].at(iVariant)["bitrate"].int64();

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

   string user::expand_url(string strText, var & varSource)
   {

      string_to_string map = get_expanded_url(varSource);

      for (auto pair : map)
      {

         strText.replace(pair.m_element1, pair.m_element2);

      }

      return strText;

   }

   string_to_string user::get_expanded_url(var & varSource)
   {
      string_to_string map;

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
               log_source("expand url map: " + strUrl + " <- " + strExpandedUrl);
               log_target("expand url map: " + strUrl + " <- " + strExpandedUrl);
               map.set_at(strUrl, strExpandedUrl);
            }
         }
      }

      return map;

   }


   string_to_string user::get_expanded_photos(var & varSource)
   {
      string_to_string map;

      for (auto set : varSource["entities"]["media"].propset())
      {
         if (set.m_element2.has_property("url") && set.m_element2.has_property("expanded_url"))
         {
            string strUrl = set.m_element2["url"];
            string strExpandedUrl = set.m_element2["expanded_url"];
            log_source("expand url map: " + strUrl + " <- " + strExpandedUrl);
            log_target("expand url map: " + strUrl + " <- " + strExpandedUrl);
            map.set_at(strUrl, strExpandedUrl);
         }
      }

      return map;

   }

   stringa user::get_media_ids(twitter * ptwitter, string_to_string & str)
   {
      stringa stra;
      for (auto pair : str)
      {

         stra.add(pair.m_element1);
      }
      return get_media_ids(ptwitter, stra);
   }

   stringa user::get_media_ids(twitter * ptwitter, const stringa & straSource)
   {

retry:
      stringa stra;
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

         {

            synch_lock sl(m_ptaska->mutex());

            user_array::cache_path cachepath;

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

                     strTime = Context.dir().home() / "Twitter Auto" / "media" / get(data_target) /System.datetime().international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE) + " " + ::str::zero_pad(::str::from(i), 3) + "_" + strName;

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

                  user_array::cache_path & cachepathSet = m_ptaska->m_mapCachePath[strPath];

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

         stra.add(strTime);


      }

      return m_ptaska->path_get_media_ids(this, ptwitter, stra);

   }



   //bool user::send_photos(var & varSource)
   //{

   //   string strText;

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   return call_tweet("", get_media_ids(ptwitter, get_media_url_media_ids(varSource)));

   //}

   //bool user::send_photos_and_text(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   string strId;

   //   strsize i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));


   //}


   //bool user::send_photos_and_text_void_link(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   string strId;

   //   strsize i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         log_source("TWEET IGNORED, Found link");

   //         return true;

   //      }

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         log_source("TWEET IGNORED, Found link");

   //         return true;

   //      }

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   //bool user::send_only_with_photo_and_text(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   if (mapMedia.is_empty())
   //   {

   //      log_source("TWEET IGNORED, No Media");

   //      return true;

   //   }

   //   string_to_string map = get_expanded_url(varSource);

   //   string strId;

   //   strsize i = 0;

   //   stringa straIgnoreLinkIgnoredDomain;

   //   straIgnoreLinkIgnoredDomain = m_straIgnoreLinkIgnoredDomain;

   //   straIgnoreLinkIgnoredDomain.surround("://", "/");

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      string strLink = strText.Mid(i, iEnd - i);

   //      string strLink2 = map[strLink];

   //      bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
   //      {
   //         return strLink2.contains_ci(str);
   //      }) >= 0;

   //      if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
   //      {

   //         log_source("TWEET IGNORED, Found link");

   //         return true;

   //      }

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      string strLink = strText.Mid(i, iEnd - i);

   //      string strLink2 = map[strLink];

   //      bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
   //      {
   //         return strLink2.contains_ci(str);
   //      }) >= 0;

   //      if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
   //      {

   //         log_source("TWEET IGNORED, Found link");

   //         return true;

   //      }

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   if (strText.trimmed().is_empty())
   //   {

   //      log_source("TWEET IGNORED, No Text");

   //      return true;

   //   }

   //   strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   //bool user::send_media_or_text(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   //if (mapMedia.is_empty())
   //   //{

   //   //   log_source("TWEET IGNORED, No Media");

   //   //   return true;

   //   //}

   //   string_to_string map = get_expanded_url(varSource);

   //   string strId;

   //   strsize i = 0;

   //   stringa straIgnoreLinkIgnoredDomain;

   //   straIgnoreLinkIgnoredDomain = m_straIgnoreLinkIgnoredDomain;

   //   straIgnoreLinkIgnoredDomain.surround("://", "/");

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      string strLink = strText.Mid(i, iEnd - i);

   //      string strLink2 = map[strLink];

   //      bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
   //      {
   //         return strLink2.contains_ci(str);
   //      }) >= 0;

   //      if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
   //      {

   //         log_source("TWEET IGNORED, Found link");

   //         return true;

   //      }
   //      else if (bIgnore)
   //      {

   //         i += strLink.get_length();

   //      }
   //      else
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      string strLink = strText.Mid(i, iEnd - i);

   //      string strLink2 = map[strLink];

   //      bool bIgnore = straIgnoreLinkIgnoredDomain.pred_find_first([&](auto str)
   //      {
   //         return strLink2.contains_ci(str);
   //      }) >= 0;

   //      if (!mapMedia.Lookup(strLink, strId) && !bIgnore)
   //      {

   //         log_source("TWEET IGNORED, Found link");

   //         return true;

   //      }
   //      else if (bIgnore)
   //      {

   //         i += strLink.get_length();

   //      }
   //      else
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }

   //   }

   //   //if (strText.trimmed().is_empty())
   //   //{

   //   //   log_source("TWEET IGNORED, No Text");

   //   //   return true;

   //   //}

   //   strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   //bool user::send_photos_text_and_one_link(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   int iLinkCount = 0;

   //   string strId;

   //   strsize i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         iLinkCount++;

   //         if (iLinkCount == 1)
   //         {

   //            i++;

   //         }
   //         else
   //         {

   //            log_source("TWEET IGNORED, Link Count Greater than 1");

   //            return true;

   //         }

   //      }
   //      else
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         iLinkCount++;

   //         if (iLinkCount == 1)
   //         {

   //            i++;

   //         }
   //         else
   //         {

   //            log_source("TWEET IGNORED, Link Count Greater than 1");

   //            return true;

   //         }

   //      }
   //      else
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }

   //   }

   //   if (iLinkCount == 0)
   //   {

   //      log_source("TWEET IGNORED, Link Count 0");

   //      return true;

   //   }

   //   strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   //bool user::send_only_with_photo__text_and_link(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   if (mapMedia.is_empty())
   //   {

   //      log_source("TWEET IGNORED, No Media");

   //      return true;

   //   }

   //   int iLinkCount = 0;

   //   string strTextNoLinks = strText;

   //   strsize i = 0;

   //   {

   //      while ((i = strTextNoLinks.find_ci("http://", i)) >= 0)
   //      {

   //         strsize iEnd = strTextNoLinks.find_whitespace_or_length(i + 1);

   //         strTextNoLinks = strTextNoLinks.Left(i) + strTextNoLinks.Mid(iEnd);

   //      }

   //      i = 0;

   //      while ((i = strTextNoLinks.find_ci("https://", i)) >= 0)
   //      {

   //         strsize iEnd = strTextNoLinks.find_whitespace_or_length(i + 1);

   //         strTextNoLinks = strTextNoLinks.Left(i) + strTextNoLinks.Mid(iEnd);

   //      }

   //   }

   //   if (strTextNoLinks.trimmed().is_empty())
   //   {

   //      log_source("TWEET IGNORED, No Text");

   //      return true;

   //   }

   //   string strId;

   //   i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         iLinkCount++;

   //         i++;

   //      }
   //      else
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (!mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         iLinkCount++;

   //         i++;

   //      }
   //      else
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }

   //   }

   //   if (iLinkCount == 0)
   //   {

   //      log_source("TWEET IGNORED, No Links");

   //      return true;

   //   }

   //   strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   //bool user::send_text(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   strsize i;

   //   i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      strText = strText.Left(i) + strText.Mid(iEnd);

   //   }

   //   return call_tweet(strText, {});

   //}


   //bool user::send_only_if_text(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   if(strText.find_ci("http://") >= 0)
   //   {

   //      log_source("TWEET IGNORED, Found link or photo");

   //      return true;

   //   }

   //   if(strText.find_ci("https://") >= 0)
   //   {

   //      log_source("TWEET IGNORED, Found link or photo");

   //      return true;

   //   }

   //   return call_tweet(strText, {});

   //}


   //bool user::send_all(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   string strId;

   //   strsize i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }
   //      else
   //      {

   //         i++;

   //      }

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }
   //      else
   //      {

   //         i++;

   //      }

   //   }

   //   strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   //bool user::send_all_non_expansion(var & varSource)
   //{

   //   string strText = varSource["full_text"];

   //   string_to_string mapMedia = get_photos_with_media_id(varSource);

   //   string strId;

   //   strsize i = 0;

   //   while ((i = strText.find_ci("http://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }
   //      else
   //      {

   //         i++;

   //      }

   //   }

   //   i = 0;

   //   while ((i = strText.find_ci("https://", i)) >= 0)
   //   {

   //      strsize iEnd = strText.find_whitespace_or_length(i + 1);

   //      if (mapMedia.Lookup(strText.Mid(i, iEnd - i), strId))
   //      {

   //         strText = strText.Left(i) + strText.Mid(iEnd);

   //      }
   //      else
   //      {

   //         i++;

   //      }

   //   }

   //   //strText = expand_url(strText, varSource);

   //   return call_tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

   //}


   bool user::call_tweet(twitter * ptwitter, string strText, stringa straMediaIds, string strReplyStatusId)
   {

      log_target("call_tweet: \"" + strText + "\"");

      strText = on_clean_message(strText);

      strText = m_ptaska->perform_url_processing(strText);

      return send_tweet(ptwitter, strText, straMediaIds, strReplyStatusId);

   }

   bool user::send_tweet(twitter * ptwitter, string strText, stringa straMediaIds, string strReplyStatusId, index iAttempt)
   {

retry:

      log_target("send_tweet: \"" + strText + "\"");

//      target();

      string strReply = ptwitter->twitter_automator_noauth(strText, straMediaIds, strReplyStatusId);

      if (!http_connection_ok(ptwitter->m_strLastConnectionStatus, ::twitter_auto::log_target))
      {

         goto retry;

      }


      if (strReply.is_empty())
      {

         if (ptwitter->m_strLastError.contains_ci("Won't tweet empty status"))
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

         if (v["errors"].at(0)["code"].int32() == 186)
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
         else if (v["errors"].at(0)["code"].int32() == 187)
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
         else if (v["errors"].at(0)["code"].int32() == 324 && iAttempt <= 1)
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

            return send_tweet(ptwitter, strText, get_media_ids(ptwitter, m_straPhoto), strReplyStatusId,  iAttempt+1);

         }
         else if (v["errors"].at(0)["code"].int32() == 130)
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

               return send_tweet(ptwitter, strText, straMediaIds, strReplyStatusId, iAttempt + 1);

            }

            log_target("OVER CAPACITY: Failed to send message after "+::str::from(iAttempt) +" with growing delay attempts");

            log_target("OVER CAPACITY: Message ignored!");

            return true;

         }
         else if (v["errors"].at(0)["code"].int32() == 131)
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

               return send_tweet(ptwitter, strText, straMediaIds, strReplyStatusId, iAttempt + 1);

            }

            log_target("INTERNAL SERVER ERROR: Failed to send message after " + ::str::from(iAttempt) + " with growing delay attempts");

            log_target("INTERNAL SERVER ERROR: Message ignored!");

            return true;

         }
         else if (v["errors"].at(0)["code"].int32() == 32)
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

               return send_tweet(ptwitter, strText, straMediaIds, strReplyStatusId, iAttempt + 1);

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


   //twitter * user::target()
   //{

   //   defer_create_target();

   //   return ptwitter;

   //}

   //void user::defer_create_target()
   //{

   //   if (ptwitter == NULL || get_tick_count() - m_dwLastAuth > (TARGET_SECONDS_RECONNECT * 1000))
   //   {

   //      m_dwLastAuth = get_tick_count();

   //      ::aura::del(ptwitter);

   //      ptwitter = create_target();

   //   }

   //}

   twitter * user::create_target()
   {

      twitter * ptarget = canew(twitter(get_context_application(), this, ::twitter_auto::log_target));

      ptarget->m_strTwitterConsumerKey = get(data_target_api_consumer_key);
      ptarget->m_strTwitterConsumerSecret = get(data_target_api_consumer_sec);
      ptarget->m_strOAuthAccessTokenKey = get(data_target_api_token_key);
      ptarget->m_strOAuthAccessTokenSecret = get(data_target_api_token_sec);
      ptarget->m_strProxy = get(data_target_proxy);

      return ptarget;

   }



   void user::simple_csv_set_line(stringa & stra)
   {

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


   void user::simple_csv_get_line(stringa & stra)
   {

      stra.set_size(m_stra.get_size());

      for (index i = 0; i < stra.get_size(); i++)
      {

         stra.set_at_grow(i, get((e_data)i));

      }

   }


   string user::get(e_data edata)
   {

      synch_lock sl(mutex());

      return m_stra[(index)edata];

   }


   void user::set(e_data edata, const string & str)
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

   }


   bool user::check_data()
   {

      synch_lock sl(mutex());

      if (get(data_status) != "on")
      {

         return false;

      }

      std::stringstream strError;

      string strLine = "(Line " + str::from(m_iIndex + 1) + ")";

      if (get(data_target).is_empty())
      {

         strError << "Source cannot be empty" << strLine << std::endl;

      }
      if (get(data_source_api_consumer_key).is_empty())
      {

         strError << "Source Consumer Key cannot be empty" << strLine << std::endl;

      }
      if (get(data_source_api_consumer_sec).is_empty())
      {

         strError << "Source Consumer Secret cannot be empty" << strLine << std::endl;

      }
      if (get(data_target_api_consumer_key).is_empty())
      {

         strError << "Target Consumer Key cannot be empty" << strLine << std::endl;

      }
      if (get(data_target_api_consumer_sec).is_empty())
      {

         strError << "Target Consumer Secret cannot be empty" << strLine << std::endl;

      }
      if (get(data_target_api_token_key).is_empty())
      {

         strError << "Target Token Key cannot be empty" << strLine << std::endl;

      }
      if (get(data_target_api_token_sec).is_empty())
      {

         strError << "Target Token Secret cannot be empty" << strLine << std::endl;

      }
      if (get(data_message).is_empty())
      {

         strError << "Message cannot be empty" << strLine << std::endl;

      }
      //if (get(data_image).is_empty())
      //{

      //   strError << "Image cannot be empty" << strLine << std::endl;

      //}


      if (strError.str().has_char())
      {

         synch_lock sl(&m_ptaska->m_mutexError);

         m_ptaska->m_strError << strError;

         return false;

      }

      return true;

   }


   void user::log(string strModifier, string str)
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

            log_line("NEW UTC DAY!!", ::twitter_auto::log_source);
            log_line("NEW UTC DAY!!", ::twitter_auto::log_target);
            log_line("NEW UTC DAY!!", ::twitter_auto::log_general);

            log_header(::twitter_auto::log_source);
            log_header(::twitter_auto::log_target);
            log_header(::twitter_auto::log_general);

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


   void user::log_source(string str)
   {

      log("source", str);

   }


   void user::log_target(string str)
   {

      log("target", str);

   }

   void user::general_log(string str)
   {

      log("general", str);

   }

   string user::log_path(string strModifier)
   {

      string str = ::str::zero_pad(::str::from(m_iIndex + 1), 3);

      if (get(data_hint_username).has_char())
      {

         str += " " + get(data_hint_username);

      }

      string strLogIndex = ::str::zero_pad(::str::from(m_iLogIndex), 4);

      return user_path(str + " " + System.datetime().international().get_gmt_date_time("%Y-%m-%d") + " "+strLogIndex + " "+strModifier+".txt");

   }

   string user::user_path(string strModifier)
   {

      string str = ::str::zero_pad(::str::from(m_iIndex + 1), 3);

      if (get(data_hint_username).has_char())
      {

         str += " " + get(data_hint_username);

      }

      return Context.dir().home() / "Twitter Auto" / str / strModifier;

   }

   string user::get_current_image()
   {

      return Context.file().as_string(user_path("current_image.txt"));

   }

   void user::set_current_image(string strImage)
   {

      Context.file().put_contents_utf8(user_path("current_image.txt"), strImage);

   }


//string user::source_path()
//{

//   return log_path("source");

//}


//string user::target_path()
//{


//   return log_path("target");

//}


   void user::log_header(index iLogTarget)
   {

      stringa stra = log_header();

      for (auto & str : stra)
      {

         log_line(str, iLogTarget);

      }

   }


   stringa user::log_header()
   {

      stringa stra;

      stra.add("|");
      stra.add("-");
      stra.add("--");
      stra.add("---");
      stra.add("LOG_HEADER: INDEX: " + ::str::zero_pad(::str::from(m_iIndex + 1), 3));
      stra.add("LOG_HEADER: SOURCE: " + get(data_target));
      string strTarget;

      if (get(data_hint_username).has_char())
      {

         strTarget = get(data_hint_username);

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


   void user::inside_loop_tests()
   {

      ::file::path t = Context.dir().home() / "Twitter Auto" / "CrashDumps/test_dump.txt";
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

         __pointer(twitter) ptwitter = create_target();

         ptwitter->m_strOAuthAccessTokenKey.Empty();

         ptwitter->m_strOAuthAccessTokenSecret.Empty();

         ptwitter->auth();

         Sleep(100000);

      }

      if (0)
      {

         ::hi5::twit twitterObj(get_context_application(), this, ::twitter_auto::log_target);



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

         ::hi5::twit twitterObj(get_context_application(), this, ::twitter_auto::log_target);



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

         ::hi5::twit twitterObj(get_context_application(), this, ::twitter_auto::log_target);



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


   void user::log_line(string str, index iLogTarget)
   {

      if (iLogTarget == ::twitter_auto::log_source)
      {

         log_source("LogLnSrc: " + str);

      }
      else if (iLogTarget == ::twitter_auto::log_target)
      {

         log_target("LogLnDst: " + str);

      }
      else
      {

         general_log("LogLnGen: " + str);

      }

   }

   string user::on_clean_message(string strText)
   {

      return remove_hashtags(strText);

   }

   string user::remove_hashtags(string strText)
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


   bool user::http_connection_ok(string strConnectionStatus, index iLogTarget)
   {

      synch_lock sl(mutex());

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

               strMessage = "maximum consecutive connection timeout error count (" + ::str::from(m_iConnectionTimeoutError) + ")reached: stopping user.";

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

            strMessage = "maximum consecutive http connection error count (" + ::str::from(m_iHttpConnectionError) + ")reached: stopping user.";

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

   string user::expand_link(string strLink, index iLog)
   {

      synch_lock sl(m_ptaska->mutex());

      {

         user_array::expand_link expandlink;

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

            user_array::expand_link & expandlink = m_ptaska->m_mapExpandLink[strLink];

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

      user_array::expand_link & expandlink = m_ptaska->m_mapExpandLink[strLink];

      expandlink.m_dwStart = get_tick_count();

      expandlink.m_strExpanded = strLink;

      return strLink;

   }

   bool user::is_expandable_link(string strLink)
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


} // namespace twitter_auto







