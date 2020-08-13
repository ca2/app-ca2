#pragma once


namespace grbl_merge_tool
{

   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOLMATOR twitter :
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
      virtual string grbl_merge_tool(const char * pszMessage);
      virtual string grbl_merge_toolmator_noauth(const char * pszMessage, stringa straMediaIds);
      virtual string media_upload_noauth(string strFile, bool bGif);
      virtual var last_grbl_merge_toolmator(const string & strUser, bool bForceAppAuth = false);
      virtual var next_grbl_merge_toolmators(const string & strUser, const string & strSince, bool bForceAppAuth = false);

      void app_auth(bool bForceAppAuth);

   };


} // namespace grbl_merge_tool



