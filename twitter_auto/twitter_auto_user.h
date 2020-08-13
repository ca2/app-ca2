#pragma once


namespace twitter_auto
{

   class user_array;

   enum e_log
   {

      log_general,
      log_source,
      log_target

   };


   class CLASS_DECL_APP_CORE_TWITTER_AUTO user :
      virtual public ::thread,
      virtual public simple_log
   {
   public:




      enum ::estatus    
      {

         status_none,
         status_on,
         status_off,

      };



      // serializable strings
      //enum e_data
      //{
      //   data_status = 0,
      //   data_target,
      //   data_source_api_consumer_key,
      //   data_source_api_consumer_sec,
      //   data_source_proxy,
      //   data_hint_username,
      //   data_target_api_consumer_key,
      //   data_target_api_consumer_sec,
      //   data_target_api_token_key,
      //   data_target_api_token_sec,
      //   data_target_proxy,
      //   data_option,
      //   data_same_consumer,
      //   data_same_proxy,
      //   data_count,
      //};
      enum e_data
      {
         data_hint_username = 0,
         data_target = 1,
         data_source_proxy = 2,
         data_target_proxy = 2,
         data_source_api_consumer_key = 3,
         data_target_api_consumer_key = 3,
         data_source_api_consumer_sec = 4,
         data_target_api_consumer_sec = 4,
         data_target_api_token_key = 5,
         data_target_api_token_sec = 6,
         data_message, // 7
         data_image, // 8
         data_status, // 9
         data_delay, // 10
         data_reset, // 11
         data_count,
      };



      int                  m_iTweetCount;
      datetime::time       m_timeResetCount;
      index                m_iEmptySinceId;
      index                m_iConnectionTimeoutError;
      index                m_iHttpConnectionError;
      DWORD                m_dwLastStatusChange;
      stringa              m_stra;
      user_array *         m_ptaska;
      index                m_iIndex;

      string               m_strLogDate;
      index                m_iLogIndex;

      // dynamic
      DWORD                m_dwLastAuth;
      //twitter *            m_ptarget;
      ::estatus                 m_estatusRequest;

      stringa              m_straIgnoreTweetDomain;
      string_to_string     m_straPhoto;
      stringa              m_straExpandDomain;
      stringa              m_straIgnoreLinkIgnoredDomain;


      user(user_array * ptaska);
      virtual ~user();

      virtual void run() override;

      string get_current_image();
      void set_current_image(string strImage);

      stringa get_media_ids(twitter * ptwitter, const stringa & stra);
      stringa get_media_ids(twitter * ptwitter, string_to_string & str);
      string_to_string get_photos_with_media_id(var & varSource);
      string_to_string get_media_url_media_ids(var & varSource);
      string_to_string get_expanded_url(var & varSource);
      string_to_string get_expanded_photos(var & varSource);
      string expand_url(string strText, var & varSource);

      bool send(var & varSource);
      bool is_source_ok(var & varSource);
      //bool send_photos_text_and_one_link(var & varSource);
      //bool send_photos(var & varSource);
      //bool send_only_with_photo__text_and_link(var & varSource);
      //bool send_photos_and_text(var & varSource);
      //bool send_text(var & varSource);
      //bool send_media_or_text(var & varSource);
      //bool send_only_with_photo_and_text(var & varSource);
      //bool send_photos_and_text_void_link(var & varSource);
      //bool send_only_if_text(var & varSource);
      //bool send_all(var & varSource);
      //bool send_all_non_expansion(var & varSource);

      //twitter * target();
      //void defer_create_target();
      twitter * create_target();

      virtual bool call_tweet(twitter * ptwitter, string strText, stringa straMediaIds, string strReplayStatusId = "");
      virtual bool send_tweet(twitter * ptwitter, string strText, stringa straMediaIds, string strReplyStatusId, index iAttempt = 1);

      virtual void simple_csv_set_line(stringa & strLine);
      virtual void simple_csv_get_line(stringa & strLine);

      string get(e_data e_data);
      void set(e_data, const string & str);
      bool check_data();

      virtual string remove_hashtags(string strText);
      virtual string on_clean_message(string strText);

      void log(string strModifier, string str);
      void log_source(string str);
      void log_target(string str);
      void general_log(string str);
      string log_path(string strModifier);
      string user_path(string strModifier);
      //string source_path();
      //string target_path();

      void inside_loop_tests();

      virtual void log_line(string str, index iLogTarget) override;

      virtual bool http_connection_ok(string strConnectionStatus, index iLogTarget);

      virtual stringa log_header();

      virtual void log_header(index iLogTarget);

      virtual string expand_link(string strLink, index iLog = ::twitter_auto::log_target);

      virtual bool is_expandable_link(string strLink);

   };


} // namespace twitter_auto



