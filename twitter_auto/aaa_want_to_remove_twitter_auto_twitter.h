#pragma once


namespace twitter_auto
{

   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR twitter :
      virtual public object
   {
   public:


      string                  m_strLogFile;
      string                  m_strTwitterConsumerKey;
      string                  m_strTwitterConsumerSecret;
      string                  m_strOAuthAccessTokenKey;
      string                  m_strOAuthAccessTokenSecret;
      string                  m_strAppToken;
      string                  m_strProxy;

      string                  m_strLastRequest;
      string                  m_strLastReply;

      twitter(::aura::application * papp);
      virtual ~twitter();


      virtual bool auth();
      virtual string twitter_auto(const char * pszMessage);
      virtual string twitter_automator_noauth(const char * pszMessage, stringa straMediaIds);
      virtual string media_upload_noauth(string strFile, bool bGif);
      virtual var last_twitter_automator(const string & strUser, bool bForceAppAuth = false);
      virtual var next_twitter_automators(const string & strUser, const string & strSince, bool bForceAppAuth = false);

      void app_auth(bool bForceAppAuth);

   };


} // namespace twitter_auto



