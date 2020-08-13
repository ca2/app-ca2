#include "framework.h"
#include "core/user/hi5/hi5.h"



namespace twitter_auto
{


   twitter::twitter(::aura::application * papp) :
      object(papp)
   {

   }

   twitter::~twitter()
   {


   }

   string twitter::twitter_auto(const char * pszMessage)
   {

      i32 iRetry = 0;

   Retry2:

      ::hi5::twit twitterObj(get_context_application());

      if (m_strProxy.has_char())
      {

         twitterObj.m_setHttp["proxy"] = m_strProxy;

      }

      string tmpStr("");
      string replyMsg("");

      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      ////string strPathKey = Context.dir().userappdata() / "twitterClient_token_key" + ::str::from(m_eversion) + ".txt";
      ////string strPathSecret = Context.dir().userappdata() / "twitterClient_token_secret" + ::str::from(m_eversion) + ".txt";
      ///* Step 1: Check if we alredy have OAuth access token from a previous run */
      ////    char szKey[1024];
      //string myOAuthAccessTokenKey = Context.file().as_string(strPathKey);
      //string myOAuthAccessTokenSecret = Context.file().as_string(strPathSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);
         twitterObj.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);
      }
      else
      {
         return "failed";
      }

      /* OAuth flow ends */

      // /* set twitter username and password */
      //twitterObj.setTwitterUsername( userName );
      //twitterObj.setTwitterPassword( passWord );

      /* Post a new status message */
      tmpStr = pszMessage;
      replyMsg = "";
      if (twitterObj.statusUpdate(tmpStr))
      {
         //replyMsg=twitterObj.get_response(  );

         //property_set set;

         //set.parse_json(replyMsg);

         //set[""]
         /*xml::document document(get_context_application());
         if(document.load(replyMsg))
         {
         if(document.get_root() != NULL)
         {
         if(document.get_root()->child_at(0) != NULL && document.get_root()->child_at(0)->get_name() == "error")
         {
         if(document.get_root()->child_at(0)->attr("code") != 34
         && document.get_root()->child_at(0)->get_value().compare_ci("Status is a duplicate.") != 0)
         {

         goto retry1;

         }
         }
         }
         }*/
         //printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
      }
      else
      {
         goto retry1;

         //printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
      }
      return replyMsg;
   retry1:
      if (iRetry >= 3)
      {
         return replyMsg = "failed";
      }
      m_strOAuthAccessTokenKey.Empty();
      m_strOAuthAccessTokenSecret.Empty();
      auth();
      iRetry++;
      goto Retry2;

   }

   string twitter::twitter_automator_noauth(const char * pszMessage, stringa straMediaIds)
   {

      ::hi5::twit twit(get_context_application());

      if (m_strProxy.has_char())
      {

         twit.m_setHttp["proxy"] = m_strProxy;

      }
      
      string strSend;

      string strResponse;

      twit.get_oauth().setConsumerKey(m_strTwitterConsumerKey);

      twit.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {

         twit.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);

         twit.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);

      }
      else
      {
         
         return "error: failed";

      }

      m_strLastRequest.Format("twitter_automator_noauth(\"%s\")", pszMessage);

      strSend = pszMessage;

      strSend.replace("&gt;", ">");

      strSend.replace("&lt;", "<");

      strSend.replace("&amp;", "&");

      twit.statusUpdate(strSend, straMediaIds);
      
      strResponse = twit.get_response();

      m_strLastReply = strResponse;

      return strResponse;

   }


   string twitter::media_upload_noauth(string strFile, bool bGif)
   {

      ::hi5::twit twitterObj(get_context_application());

      if (m_strProxy.has_char())
      {

         twitterObj.m_setHttp["proxy"] = m_strProxy;

      }

      string tmpStr("");
      string replyMsg("");

      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);
         twitterObj.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);
      }
      else
      {
         return "error: missing either tokey key or token scret";
      }

      m_strLastRequest.Format("media_upload_noauth(\"%s\")", strFile);

      /* OAuth flow ends */

      // /* set twitter username and password */
      //twitterObj.setTwitterUsername( userName );
      //twitterObj.setTwitterPassword( passWord );

      /* Post a new status message */


      ::file::file_pointer pfile = Context.file().get_file(strFile, ::file::type_binary | ::file::mode_read | ::file::share_deny_none);

      if (pfile.is_null())
      {

         return "error: could not open file: " + strFile;

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

         //if (bGif)
         //{

           // strCategory = "tweet_gif";

         //}
         //else
         {

            strCategory = "tweet_video";

         }

         strMimeType = "video/mp4";

      }


      string strMediaId = twitterObj.mediaUpload(pfile, strMimeType, strCategory);

      return strMediaId;

   }


   bool twitter::auth()
   {


      ::hi5::twit twitterObj(get_context_application());



      if (m_strProxy.has_char())
      {

         twitterObj.m_setHttp["proxy"] = m_strProxy;

      }


      string tmpStr("");
      string replyMsg("");


      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

//      string strPathKey = Context.dir().userappdata() / "twitterClient_token_key" + ::str::from(m_eversion) + ".txt";
  //    string strPathSecret = Context.dir().userappdata() / "twitterClient_token_secret" + ::str::from(m_eversion) + ".txt";
      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      //    char szKey[1024];
    //  string myOAuthAccessTokenKey = Context.file().as_string(strPathKey);
      //string myOAuthAccessTokenSecret = Context.file().as_string(strPathSecret);

      if (m_strOAuthAccessTokenKey.has_char() && m_strOAuthAccessTokenSecret.has_char())
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey(m_strOAuthAccessTokenKey);
         twitterObj.get_oauth().setOAuthTokenSecret(m_strOAuthAccessTokenSecret);
      }
      else
      {
         /* Step 2: Get request token key and secret */
         twitterObj.oAuthRequestToken(tmpStr);

         /* Step 3: Ask user to visit web link and get PIN */
         string szOAuthVerifierPin;

         ::hi5::twitter::authorization authapp(get_context_application(), tmpStr, "twitter\\authorization.xhtml", true);
         szOAuthVerifierPin = authapp.get_pin();

         tmpStr = szOAuthVerifierPin;
         twitterObj.get_oauth().setOAuthPin(tmpStr);

         /* Step 4: Exchange request token with access token */
         twitterObj.oAuthAccessToken();

         /* Step 5: Now, save this access token key and secret for future use without PIN */
         twitterObj.get_oauth().getOAuthTokenKey(m_strOAuthAccessTokenKey);
         twitterObj.get_oauth().getOAuthTokenSecret(m_strOAuthAccessTokenSecret);

         /* Step 6: Save these keys in a file or wherever */

         Context.file().put_contents("C:\\archive\\token_key.txt", m_strOAuthAccessTokenKey);
         Context.file().put_contents("C:\\archive\\token_sec.txt", m_strOAuthAccessTokenSecret);

      }

      return true;
   }



   var twitter::last_twitter_automator(const string & strUser, bool bForceAppAuth)
   {

      app_auth(bForceAppAuth);

      string strResponse;

      property_set set;

      set["headers"]["Authorization"] = "Bearer " + m_strAppToken;

      string strUrl = "https://api.twitter.com/1.1/statuses/user_timeline.json?count=1&screen_name=" + strUser+ "&tweet_mode=extended";

      set["raw_http"] = true;

      if (m_strProxy.has_char())
      {

         set["proxy"] = m_strProxy;

      }

      m_strLastRequest.Format("last_twitter_automator(\"%s\", %d)", strUser, (int) bForceAppAuth);
      

      string str = Context.http().get(strUrl, set);

      m_strLastReply = str;

      var v;
      const char * p = str;
      try
      {

         v.parse_json(p);

      }
      catch (...)
      {

         v["errors"].vara().add("EXCEPTION parsing: \"" + str + "\"");

      }

      if (!bForceAppAuth && (str.is_empty() || v["errors"].get_count() > 0))
      {

         return last_twitter_automator(strUser, true);

      }

      return v;


   }

   var twitter::next_twitter_automators(const string & strUser, const string & strSince, bool bForceAppAuth)
   {

      string strResponse;

      app_auth(bForceAppAuth);

      property_set set;

      set["headers"]["Authorization"] = "Bearer " + m_strAppToken;

      string strUrl = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=" + strUser + "&since_id="+strSince+"&tweet_mode=extended";

      set["raw_http"] = true;
      if (m_strProxy.has_char())
      {

         set["proxy"] = m_strProxy;

      }

      m_strLastRequest.Format("next_twitter_automators(\"%s\", \"%s\" %d)", strUser, strSince, (int)bForceAppAuth);

      string str = Context.http().get(strUrl, set);

      m_strLastReply = str;

      var v;
      const char * p = str;
      v.parse_json(p);

      if (!bForceAppAuth && (str.is_empty() || v.has_property("errors") && v["errors"].get_count() > 0))
      {

         return next_twitter_automators(strUser, strSince, true);

      }

      return v;

   }

   void twitter::app_auth(bool bForceAppAuth)
   {

      if (!bForceAppAuth)
      {

         if (m_strAppToken.has_char())
         {
          
            return;

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

      string str = Context.http().get(strUrl, set);

      var v;
      const char * p = str;
      v.parse_json(p);

      if (v["token_type"] == "bearer")
      {

         m_strAppToken = v["access_token"];

      }



   }

}
