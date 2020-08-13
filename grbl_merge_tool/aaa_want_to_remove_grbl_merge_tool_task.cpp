#include "framework.h"
#include "core/user/hi5/hi5.h"


CLASS_DECL_AURA void
encode_utf16_pair(u32 character, u16 *units);
CLASS_DECL_AURA bool is_surrogated(u32 character);

#define TARGET_MINUTES_RECONNECT 5
#define TARGET_SECONDS_RECONNECT (TARGET_MINUTES_RECONNECT * 60)

namespace grbl_merge_tool
{

   task::task(task_array * ptaska) :
      object(ptaska->get_context_application()),
      ::thread(ptaska->get_context_application())
   {

      m_ptaska = ptaska;

      m_stra.set_size(data_count);
      m_ptarget = NULL;

      set(data_status, "off");
      set(data_source, "Source");
      set(data_hint_target, "Target Hint");
      set(data_option, "Copy All");
      set(data_same_consumer, "on");
      set(data_same_proxy, "on");

      m_straIgnoreTweetDomain.add("fatkiddeals.com");
      m_straIgnoreTweetDomain.add("twitter.com/fatkiddeals/");

   }

   task::~task()
   {


   }

   
   i32 task::run()
   {

      bool bDebug3Json = false;

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
               return 0;

            }


#if 0

            inside_loop_tests();

#endif

            while (get_run_thread())
            {

               try
               {

                  string strCopy = get(data_option);

                  twitter twitterSource(get_context_application());

                  twitterSource.m_strTwitterConsumerKey = get(data_source_api_consumer_key);
                  twitterSource.m_strTwitterConsumerSecret = get(data_source_api_consumer_sec);
                  twitterSource.m_strProxy = get(data_source_proxy);

                  var varSource = twitterSource.last_grbl_merge_toolmator(get(data_source));

                  log_source("REQUEST-->");
                  log_source(twitterSource.m_strLastRequest);
                  log_source("RESPONSE<--");
                  log_source(twitterSource.m_strLastReply);
                  log_source("");

                  if (varSource.is_new() || varSource.has_property("errors"))
                  {

                     general_log("exiting (2.0)");
                     return 0;

                  }

                  string strSinceId = varSource.at(0)["id_str"];

                  while (thread_get_run())
                  {


                     if (!check_data())
                     {

                        general_log("exiting (2)");

                        return 0;

                     }
                     twitterSource.m_strTwitterConsumerKey = get(data_source_api_consumer_key);
                     twitterSource.m_strTwitterConsumerSecret = get(data_source_api_consumer_sec);
                     twitterSource.m_strProxy = get(data_source_proxy);

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

                        bDebug3Json = false;

                     }
                     else
                     {

                        NotDebug3Json:

                        varSource = twitterSource.next_grbl_merge_toolmators(get(data_source), strSinceId);

                     }


                     log_source("REQUEST-->");
                     log_source(twitterSource.m_strLastRequest);
                     log_source("RESPONSE<--");
                     log_source(twitterSource.m_strLastReply);
                     log_source("");

                     if (varSource.is_new() || varSource.has_property("errors"))
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

                              return 0;

                           }

                           break;

                        }

                        general_log("exiting (3)");

                        return 0;

                     }
                     else
                     {

                        int i = varSource.get_count() - 1;

                        int iGoodSinceId = -1;

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

                              return 0;

                           }

                           i--;

                        }

                        if (iGoodSinceId >= 0 && iGoodSinceId < varSource.get_count())
                        {

                           strSinceId = varSource.at(iGoodSinceId)["id_str"];

                        }

                     }

                     Sleep(10000);

                  }

               }
               catch (...)
               {


               }

            }

            general_log("exiting (5)");

            return 0;

         }
         catch (::exception::exception & e)
         {

            log_source("Exception in grbl_merge_tool::task::run");

            log_source(e.what());

            bOkSession = false;

         }
         catch (...)
         {

            general_log("Unknown exception in grbl_merge_tool::task::run");

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

            if (!get_run_thread())
            {

               break;

            }

            Sleep(500);
            
         }

         if (!get_run_thread())
         {

            break;

         }

      }

      if (!bOkSession)
      {

         general_log("General Failure by EXCEPTION: Maximum number of 3 failures in one-hour time span reached");

      }

      return 0;

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

         return send_only_with_photo_and_text(varSource);

      }
      else if (get(data_option) == "Copy Text")
      {

         return send_only_if_text(varSource);

      }
      else if (get(data_option) == "Copy All")
      {

         return send_all(varSource);

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

         while((iFind = strExpandFullText.find_ci(strDomain, iFind + 1)) >= 0)
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
            log_source("Found Video! : " + photo["video_info"]["variants"].at(0)["url"]);

            if (photo.has_property("type") && photo["type"].get_string().compare_ci("animated_gif") == 0)
            {
               straPhoto.set_at(string("gif://")+ photo["video_info"]["variants"].at(0)["url"], photo["id"]);
            }
            else
            {
               straPhoto.set_at(photo["video_info"]["variants"].at(0)["url"], photo["id"]);
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

         strText.replace(pair.m_element1, pair.m_element2);

      }

      return strText;

   }

   string_to_string task::get_expanded_url(var & varSource)
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
               log_source("expand url map: " + strUrl + " <- " + strExpandedUrl);
               log_target("expand url map: " + strUrl + " <- " + strExpandedUrl);
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
            log_source("expand url map: " + strUrl + " <- " + strExpandedUrl);
            log_target("expand url map: " + strUrl + " <- " + strExpandedUrl);
            map.set_at(strUrl, strExpandedUrl);
         }
      }

      return map;

   }

   stringa task::get_media_ids(string_to_string & str)
   {
      stringa stra;
      defer_create_target();
      for (auto pair : str)
      {
         
         string strPath = pair.m_element1;

         bool bGif = str::begins_eat_ci(strPath, "gif://");

         ::file::path path = strPath;

         strsize iFind = path.find('?');

         if (iFind >= 0)
         {

            path = path.Left(iFind);

         }

         string strTime = Context.dir().home() / "Twitter Automator" / "media" / System.datetime().international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE) + "_" + path.name();

         property_set set;

         set["raw_http"] = true;

         set["disable_common_name_cert_check"] = true;

         if (!Context.http().download(strPath, strTime, set))
         {

            log_source("Failed to download media file : " + strPath);

            log_target("Failed to download media file : " + strPath);

            continue;

         }

         string strMediaId = m_ptarget->media_upload_noauth(strTime, bGif);

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

      return tweet("", get_media_ids(get_media_url_media_ids(varSource)));

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

      return tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));


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

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
         {

            log_source("TWEET IGNORED, Found link");

            return true;

         }

         strText = strText.Left(i) + strText.Mid(iEnd);

      }

      return tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

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

      string strId;

      strsize i = 0;

      while ((i = strText.find_ci("http://", i)) >= 0)
      {

         strsize iEnd = strText.find_whitespace_or_length(i + 1);

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

      return tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

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

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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
         
         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

      return tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

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

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

         if (!mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

      return tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));

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

      return tweet(strText, {});

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

      return tweet(strText, {});

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

          if (mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
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

          if (mapMedia.lookup(strText.Mid(i, iEnd - i + 1), strId))
          {

             strText = strText.Left(i) + strText.Mid(iEnd);

          }
          else
          {

            i++;

          }

       }

       strText = expand_url(strText, varSource);

       return tweet(strText, get_media_ids(get_media_url_media_ids(varSource)));
   
   }


   bool task::tweet(string strText, stringa straMediaIds, int iAttempt)
   {

      target();

      string strReply = m_ptarget->grbl_merge_toolmator_noauth(strText, straMediaIds);

      log_target("REQUEST-->");
      log_target(m_ptarget->m_strLastRequest);
      log_target("RESPONSE<--");
      log_target(m_ptarget->m_strLastReply);
      log_target("");

      const char * psz = strReply;

      var v;

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

         if (v["errors"].at(0)["code"].int32() == 324 && iAttempt <= 1)
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

            return tweet(strText, get_media_ids(m_straPhoto), iAttempt+1);

         }

         return false;

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

         ::aura::del(m_ptarget);

         m_ptarget = create_target();

      }

   }

   twitter * task::create_target()
   {

      twitter * ptarget = new twitter(get_context_application());
         
      ptarget->m_strTwitterConsumerKey = get(data_target_api_consumer_key);
      ptarget->m_strTwitterConsumerSecret = get(data_target_api_consumer_sec);
      ptarget->m_strOAuthAccessTokenKey = get(data_target_api_token_key);
      ptarget->m_strOAuthAccessTokenSecret = get(data_target_api_token_sec);
      ptarget->m_strProxy = get(data_target_proxy);
      
      return ptarget;

   }
  


   void task::simple_csv_set_line(stringa & stra)
   {

      if (stra.safe_at(data_same_consumer).is_empty())
      {

         stra.set_at_grow(data_same_consumer, "on");

      }

      if (stra.safe_at(data_same_proxy).is_empty())
      {

         stra.set_at_grow(data_same_proxy, "on");

      }

      if (stra.safe_at(data_status) == "on")
      {

      }
      else
      {

         set((e_data)0, stra[0]);

      }

      
      for (index i = 1; i < stra.get_size(); i++)
      {

         set((e_data)i, stra[i]);

      }

      if (stra.safe_at(data_status) == "on")
      {

         set((e_data)0, stra[0]);

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
      if (edata == data_status)
      {

         if (get_run_thread())
         {

            m_stra[(index)edata] = "on";

         }
         else
         {

            m_stra[(index)edata] = "off";

         }

      }

      return m_stra[(index)edata];

   }


   void task::set(e_data edata, const string & str)
   {


      {
         synch_lock sl(mutex());
         m_stra.set_at_grow((index)edata, str);
      }

      if (edata == data_status)
      {

         if (str == "on")
         {

            if (!get_run_thread())
            {

               thread::begin();

            }

         }
         else
         {

            if (get_run_thread())
            {

               set_finish();

            }

         }

      }

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

   
   void task::log(string strFile, string str)
   {

      string strMessage;

      string strCurDate;

      strCurDate = System.datetime().international().get_gmt_date_time("%H:%M:%S");

      if (strCurDate != m_strLogDate)
      {

         m_strLogDate = strCurDate;

         m_iLogIndex = 0;

      }

      strMessage += m_strLogDate;
      strMessage += " ";
      strMessage += str;
      strMessage += "\r\n";

      for (; m_iLogIndex <= 9999; m_iLogIndex++)
      {
         
         string strFinalFile;

         strFinalFile = (strFile);

         ::str::ends_eat_ci(strFinalFile, ".txt");

         string strFormat;

         strFormat.Format("%04d", m_iLogIndex);

         strFinalFile += " ";
         strFinalFile += strFormat;
         strFinalFile += ".txt";

         if (file_append_wait_dup(strFinalFile, strMessage, strMessage.get_length(), 1000))
         {

            break;

         }

      }

   }


   void task::log_source(string str)
   {

      log(source_path(), str);

   }


   void task::log_target(string str)
   {

      log(target_path(), str);

   }

   void task::general_log(string str)
   {

      log(log_path("general-" + ::str::from(m_iIndex) + "-"), str);

   }

   string task::log_path(string strModifier)
   {
      
      return Context.dir().home() / "Twitter Automator" / strModifier + System.datetime().international().get_gmt_date_time("%Y-%m-%d") + ".txt";

   }

   string task::source_path()
   {

      return log_path(get(data_source) + "-"+::str::from(m_iIndex)+"/");

   }

   string task::target_path()
   {

      if (get(data_hint_target).has_char())
      {

         return log_path(get(data_hint_target) + "-" + ::str::from(m_iIndex) + "/");

      }
      else
      {

         string str = get(data_target_api_token_key);

         int iFind = str.find('-');

         if (iFind > 0)
         {

            return log_path(str.Left(iFind) + "-" + ::str::from(m_iIndex) + "/");

         }
         else
         {

            return log_path(str + "-" + ::str::from(m_iIndex) + "/");
         }

      }


   }


   void task::inside_loop_tests()
   {

      if (0)
      {



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

         ::hi5::twit twitterObj(get_context_application());



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

         ::hi5::twit twitterObj(get_context_application());



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
                  u32 uiUniIndex = ::str::ch::uni_index(strChar);
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

         ::hi5::twit twitterObj(get_context_application());



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
                  int iUniIndex = ::str::ch::uni_index(strChar);
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


} // namespace grbl_merge_tool







