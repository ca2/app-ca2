#pragma once


namespace twitter_automator
{

   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR twitter :
      virtual public simple_log
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
      string                  m_strLastError;
      string                  m_strLastConnectionStatus;

      
      twitter(::aura::application * papp, simple_log * psimplelog, int iLogTarget);
      virtual ~twitter();


      virtual bool auth();
      virtual string twitter_automator(const char * pszMessage);
      virtual var last_twitter_automator(const string & strUser, bool bForceAppAuth = false);
      virtual var next_twitter_automators(const string & strUser, const string & strSince, bool bForceAppAuth = false);

      virtual string twitter_automator_noauth(const char * pszMessage, stringa straMediaIds);
      virtual string media_upload_noauth(string strFile, bool bGif);

      bool app_auth(bool bForceAppAuth);


   };


} // namespace twitter_automator



