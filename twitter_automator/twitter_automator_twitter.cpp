#include "framework.h"
#include "core/user/hi5/hi5.h"



namespace twitter_automator
{


   twitter::twitter(::aura::application * papp, simple_log * psimplelog, int iLogTarget) :
      object(papp),
      simple_log(psimplelog, iLogTarget)
   {

   }


   twitter::~twitter()
   {


   }


   string twitter::twitter_automator(const char * pszMessage)
   {

      i32 iRetry = 0;

retry:

      m_strLastRequest = "twitter::twitter_automator(\"" + string(pszMessage) + "\") iRetry=" + ::str::from(iRetry);

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST: " + m_strLastRequest);

      ::hi5::twit twit(get_context_application(), m_psimplelog, m_iLogTarget);

      if (m_strProxy.has_char())
      {

         twit.m_setHttp["proxy"] = m_strProxy;

      }

      string tmpStr;

      string replyMsg;

      twit.get_oauth().setConsumerKey(m_strTwitterConsumerKey);

      twit.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {

         twit.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);

         twit.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);

      }
      else
      {

         goto request_authorization;

      }

      tmpStr = pszMessage;

      replyMsg = "";

      bool bOk = twit.statusUpdate(tmpStr);

      if (!bOk && twit.m_strConnectionStatus.begins_ci("ERROR: HTTP_CONNECTION_ERROR: "))
      {

         return twit.m_strConnectionStatus;

      }

      if (!bOk)
      {

         replyMsg = twit.m_strError;

      }
      else
      {

         replyMsg = twit.get_response();

      }

      m_strLastReply = replyMsg;

      if (bOk)
      {

         log_line("REPLY: " + m_strLastReply);

         return replyMsg;

      }

      log_line("ERROR: REPLY: " + m_strLastReply);

request_authorization:

      if (iRetry >= 3)
      {

         log_line("ERROR: Maximum number of retries reached ("+::str::from(iRetry)+")");

         return "ERROR: ";

      }

      m_strOAuthAccessTokenKey.Empty();

      m_strOAuthAccessTokenSecret.Empty();

      log_line("Going to retry authorization");

      auth();

      iRetry++;

      goto retry;

   }


   string twitter::twitter_automator_noauth(const char * pszMessage, stringa straMediaIds)
   {

      m_strLastRequest = "twitter::twitter_automator_noauth(\"" + string(pszMessage) + "\", {"+straMediaIds.implode(", ") +"})";

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST: " + m_strLastRequest);

      ::hi5::twit twit(get_context_application(), m_psimplelog, m_iLogTarget);

      if (m_strProxy.has_char())
      {

         twit.m_setHttp["proxy"] = m_strProxy;

      }

      string strSend;

      twit.get_oauth().setConsumerKey(m_strTwitterConsumerKey);

      twit.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {

         twit.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);

         twit.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);

      }
      else
      {

         m_strLastReply = "ERROR: Either access token key and/or secret empty";

         log_line(m_strLastReply);

         return m_strLastReply;

      }

      strSend = pszMessage;

      strSend.replace("&gt;", ">");

      strSend.replace("&lt;", "<");

      strSend.replace("&amp;", "&");

      bool bOk = twit.statusUpdate(strSend, straMediaIds);

      m_strLastReply = twit.get_response();

      m_strLastError = twit.m_strError;

      m_strLastConnectionStatus = twit.m_strConnectionStatus;

      return m_strLastReply;

   }


   string twitter::media_upload_noauth(string strFile, bool bGif)
   {

      m_strLastRequest = "twitter::media_upload_noauth(\"" + strFile + "\", bGif=" + (bGif?"true":"false")+ ")";

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST: " + m_strLastRequest);

      ::hi5::twit twit(get_context_application(), m_psimplelog, m_iLogTarget);

      if (m_strProxy.has_char())
      {

         twit.m_setHttp["proxy"] = m_strProxy;

      }

      string tmpStr("");

      string replyMsg("");

      twit.get_oauth().setConsumerKey(m_strTwitterConsumerKey);

      twit.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {

         twit.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);

         twit.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);

      }
      else
      {

         m_strLastReply = "ERROR: Either access token key and/or secret empty";

         log_line(m_strLastReply);

         return m_strLastReply;

      }

      ::file::file_pointer pfile = Context.file().get_file(strFile, ::file::type_binary | ::file::mode_read | ::file::share_deny_none);

      if (pfile.is_null())
      {

         m_strLastReply = "ERROR: Could Not Open File \"" + strFile + "\"";

         log_line(m_strLastReply);

         return m_strLastReply;

      }

      string strMimeType;

      string strCategory;

      if (strFile.ends_ci(".jpg"))
      {

         strCategory = "tweet_image";
         strMimeType = "image/jpeg";

      }
      else if(strFile.ends_ci(".png"))
      {

         strCategory = "tweet_image";
         strMimeType = "image/png";

      }
      else if (strFile.ends_ci(".gif"))
      {

         strCategory = "tweet_gif";
         strMimeType = "image/gif";

      }
      else
      {

         strCategory = "tweet_video";
         strMimeType = "video/mp4";

      }

      pfile->m_strFileName = "media." + pfile->m_strFileName.final_extension();

      string strMediaId = twit.mediaUpload(pfile, strMimeType, strCategory);

      if (twit.m_strConnectionStatus.begins_ci("ERROR: HTTP_CONNECTION_ERROR: "))
      {

         return twit.m_strConnectionStatus;

      }

      m_strLastReply = twit.get_response();

      if (strMediaId.begins_ci("ERROR: "))
      {

         log_line("ERROR: REPLY: " + m_strLastReply);

      }
      else
      {

         log_line("REPLY: " + m_strLastReply);

      }

      return strMediaId;

   }


   bool twitter::auth()
   {

      m_strLastRequest = "twitter::auth()";

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST : " + m_strLastRequest);

      ::hi5::twit twit(get_context_application(), m_psimplelog, m_iLogTarget);

      if (m_strProxy.has_char())
      {

         twit.m_setHttp["proxy"] = m_strProxy;

      }

      string tmpStr;

      string replyMsg;

      twit.get_oauth().setConsumerKey(m_strTwitterConsumerKey);

      twit.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {

         twit.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);

         twit.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);

      }
      else
      {

         twit.oAuthRequestToken(tmpStr);

         string szOAuthVerifierPin;

         ::hi5::twitter::authorization authapp(get_context_application(), tmpStr, "twitter\\authorization.xhtml", true);

         szOAuthVerifierPin = authapp.get_pin();

         tmpStr = szOAuthVerifierPin;

         twit.get_oauth().setOAuthPin(tmpStr);

         twit.oAuthAccessToken();

         twit.get_oauth().getOAuthTokenKey(m_strOAuthAccessTokenKey);

         twit.get_oauth().getOAuthTokenSecret(m_strOAuthAccessTokenSecret);

         Context.file().put_contents("C:\\archive\\token_key.txt", m_strOAuthAccessTokenKey);

         Context.file().put_contents("C:\\archive\\token_sec.txt", m_strOAuthAccessTokenSecret);

      }

      return true;

   }


   var twitter::last_twitter_automator(const string & strUser, bool bForceAppAuth)
   {

      m_strLastRequest = "twitter::last_twitter_automator(1)(\"" + strUser + "\", bForceAppAuthorization=" + (bForceAppAuth ? "true" : "false") + ")";

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST: " + m_strLastRequest);

      if (!app_auth(bForceAppAuth))
      {

         return var::type_new;

      }

      if (m_strLastConnectionStatus.has_char())
      {

         m_strLastRequest = "twitter::last_twitter_automator(2)(\"" + strUser + "\", bForceAppAuthorization=" + (bForceAppAuth ? "true" : "false") + ")";

         m_strLastReply = "";

         m_strLastConnectionStatus = "";

         m_strLastError = "";

         log_line("REQUEST : " + m_strLastRequest);

      }

      string strResponse;

      property_set set;

      set["headers"]["Authorization"] = "Bearer " + m_strAppToken;

      string strUrl = "https://api.twitter.com/1.1/statuses/user_timeline.json?count=1&screen_name=" + strUser+ "&tweet_mode=extended";

      set["raw_http"] = true;

      if (m_strProxy.has_char())
      {

         log_line("setting PROXY: " + m_strProxy);

         set["proxy"] = m_strProxy;

      }

      log_line("URL : " + strUrl);

      m_strLastReply = Context.http().get(strUrl, set);

      m_strLastConnectionStatus = ::http::conn_status(set);

      log_line(m_strLastConnectionStatus);

      log_line("REPLY: " + m_strLastReply);

      if (str::begins_ci(m_strLastConnectionStatus, "ERROR: "))
      {

         return var::type_new;

      }

      if (m_strLastReply.is_empty())
      {

         m_strLastError = "ERROR: Empty Response!!";

         return var::type_new;

      }

      var v;

      const char * p = m_strLastReply;

      try
      {

         v.parse_json(p);

      }
      catch (exception::exception & e)
      {

         m_strLastError = "ERROR: EXCEPTION: \"" + string(e.what()) + "\" parsing \"" + m_strLastReply +"\"";

         log_line(m_strLastError);

         v["errors"].vara().add(m_strLastError);

      }
      catch (...)
      {

         m_strLastError = "ERROR: EXCEPTION: ... parsing \"" + m_strLastReply + "\"";

         log_line(m_strLastError);

         v["errors"].vara().add(m_strLastError);

      }

      if (!bForceAppAuth && (m_strLastReply.is_empty() || (v.has_property("errors") && v["errors"].get_count() > 0)))
      {

         log_line("ERROR: REPLY " + m_strLastReply);

         log_line("Going to retry with app reauthorization.");

         return last_twitter_automator(strUser, true);

      }

      return v;

   }


   var twitter::next_twitter_automators(const string & strUser, const string & strSince, bool bForceAppAuth)
   {

      m_strLastRequest = "twitter::next_twitter_automators(1)(\"" + strUser + "\", "+strSince+",bForceAppAuthorization=" + (bForceAppAuth ? "true" : "false") + ")";

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST : " + m_strLastRequest);

      string strResponse;

      if (!app_auth(bForceAppAuth))
      {

         return ::var::type_new;

      }

      if (m_strLastConnectionStatus.has_char())
      {

         m_strLastRequest = "twitter::next_twitter_automators(2)(\"" + strUser + "\", " + strSince + ",bForceAppAuthorization=" + (bForceAppAuth ? "true" : "false") + ")";

         m_strLastReply = "";

         m_strLastConnectionStatus = "";

         m_strLastError = "";

         log_line("REQUEST : " + m_strLastRequest);

      }

      property_set set;

      set["headers"]["Authorization"] = "Bearer " + m_strAppToken;

      string strUrl = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=" + strUser + "&since_id="+strSince+"&tweet_mode=extended";

      set["raw_http"] = true;

      if (m_strProxy.has_char())
      {

         log_line("setting PROXY: " + m_strProxy);

         set["proxy"] = m_strProxy;

      }

      log_line("URL : " + strUrl);

      m_strLastReply = Context.http().get(strUrl, set);

      m_strLastConnectionStatus = ::http::conn_status(set);

      log_line(m_strLastConnectionStatus);

      log_line("REPLY: " + m_strLastReply);

      if (str::begins_ci(m_strLastConnectionStatus, "ERROR: "))
      {

         return var::type_new;

      }

      if (m_strLastReply.is_empty())
      {

         m_strLastError = "ERROR: Empty Response!!";

         return var::type_new;

      }

      var v;

      const char * p = m_strLastReply;

      try
      {

         v.parse_json(p);

      }
      catch (exception::exception & e)
      {

         m_strLastError = "ERROR: EXCEPTION: \"" + string(e.what()) + "\" parsing \"" + m_strLastReply + "\"";

         log_line(m_strLastError);

         v["errors"].vara().add(m_strLastError);

      }
      catch (...)
      {

         m_strLastError = "ERROR: EXCEPTION: ... parsing \"" + m_strLastReply + "\"";

         log_line(m_strLastError);

         v["errors"].vara().add(m_strLastError);

      }

      if (!bForceAppAuth && (m_strLastReply.is_empty() || (v.has_property("errors") && v["errors"].get_count() > 0)))
      {

         log_line("ERROR: REPLY: " + m_strLastReply);

         log_line("Going to retry with app reauthorization.");

         return next_twitter_automators(strUser, strSince, true);

      }

      return v;

   }

   bool twitter::app_auth(bool bForceAppAuth)
   {

      m_strLastRequest = string("twitter::app_auth(bForceAppAuthorization=") + (bForceAppAuth ? "true" : "false") + ")";

      m_strLastReply = "";

      m_strLastConnectionStatus = "";

      m_strLastError = "";

      log_line("REQUEST : " + m_strLastRequest);

      if (!bForceAppAuth)
      {

         if (m_strAppToken.has_char())
         {

            return true;

         }

      }

      property_set set;

      set["headers"]["Authorization"] = "Basic " + System.base64().encode(m_strTwitterConsumerKey + ":" + m_strTwitterConsumerSecret);

      set["post"]["grant_type"] = "client_credentials";

      set["headers"]["content-type"] = "application/x-www-form-urlencoded;charset=UTF-8";

      string strUrl = "https://api.twitter.com/oauth2/token";

      set["raw_http"] = true;

      if (m_strProxy.has_char())
      {

         set["proxy"] = m_strProxy;

      }

      m_strLastReply = Context.http().get(strUrl, set);

      m_strLastConnectionStatus = ::http::conn_status(set);

      log_line(m_strLastConnectionStatus);

      if (str::begins_ci(m_strLastConnectionStatus, "ERROR: "))
      {

         return false;

      }

      if (m_strLastReply.is_empty())
      {

         m_strLastError = "ERROR: Empty Response!!";

         return false;

      }

      var v;

      const char * p = m_strLastReply;

      try
      {

         v.parse_json(p);

      }
      catch (exception::exception & e)
      {

         m_strLastError = "ERROR: EXCEPTION: \"" + string(e.what()) + "\" parsing \"" + m_strLastReply + "\"";

         log_line(m_strLastError);

         v["errors"].vara().add(m_strLastError);

      }
      catch (...)
      {

         m_strLastError = "ERROR: EXCEPTION: ... parsing \"" + m_strLastReply + "\"";

         log_line(m_strLastError);

         v["errors"].vara().add(m_strLastError);

      }

      if (v["token_type"] == "bearer")
      {

         m_strAppToken = v["access_token"];

         log_line("REPLY : Authorization Success!!");

         return true;

      }
      else
      {


         m_strAppToken = "";

         m_strLastError = "ERROR: " + m_strLastReply;

         log_line(m_strLastError);

         return false;

      }

   }




} // namespace twitter_automator





