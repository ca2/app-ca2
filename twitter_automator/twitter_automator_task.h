#pragma once


namespace twitter_automator
{

   enum e_log
   {

      log_general,
      log_source,
      log_target

   };


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR task :
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
      //   data_source,
      //   data_source_api_consumer_key,
      //   data_source_api_consumer_sec,
      //   data_source_proxy,
      //   data_hint_target,
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
         data_hint_target = 0,
         data_source = 1,
         data_source_proxy = 2,
         data_target_proxy = 2,
         data_source_api_consumer_key = 3,
         data_target_api_consumer_key = 3,
         data_source_api_consumer_sec = 4,
         data_target_api_consumer_sec = 4,
         data_target_api_token_key = 5,
         data_target_api_token_sec = 6,
         data_option = 7,
         data_status = 8,
         data_delay = 9,
         data_hashtag = 10,
         data_cache = 12,
         data_count = 13,
      };

      ::mutex                m_mutexCache;
      var_array            m_varaCache;



      string_to_string     m_mapExpandedUrl;


      index                m_iEmptySinceId;
      index                m_iConnectionTimeoutError;
      index                m_iHttpConnectionError;
      DWORD                m_dwLastStatusChange;
      stringa              m_stra;
      task_array *         m_ptaska;
      index                m_iIndex;

      string               m_strLogDate;
      index                m_iLogIndex;

      // dynamic
      DWORD                m_dwLastAuth;
      twitter *            m_ptarget;
      ::estatus                 m_estatusRequest;

      stringa              m_straIgnoreTweetDomain;
      string_to_string     m_straPhoto;
      stringa              m_straExpandDomain;
      stringa              m_straIgnoreLinkIgnoredDomain;
      stringa              m_straIgnoreTweetWithText;
      stringa              m_straRemoveHashTag;
      domain_find_replace_array        m_domainfindreplacea;
      file_time                        m_filetimeDomainFindReplace;
      __pointer(::thread)         m_pthreadCache;


      task(task_array * ptaska);
      virtual ~task();

      virtual void run() override;

      virtual ::file::path dir_path();

      stringa get_media_ids(string_to_string & str);
      string_to_string get_photos_with_media_id(var & varSource);
      string_to_string get_media_url_media_ids(var & varSource);
      string_to_string get_expanded_url(var & varSource);
      string_to_string get_expanded_photos(var & varSource);
      string expand_url(string strText, var & varSource);

      index cache_find_oldest_message();
      index cache_send_random_message();

      bool is_cached();

      bool cache(var & varSource);
      bool call_send(var & varSource);
      bool send(var & varSource);
      bool is_source_ok(var & varSource);
      bool send_photos_text_and_one_link(var & varSource);
      bool send_photos(var & varSource);
      bool send_only_with_photo__text_and_link(var & varSource);
      bool send_photos_and_text(var & varSource);
      bool send_text(var & varSource);
      bool send_media_or_text(var & varSource);
      bool send_only_with_photo_and_text(var & varSource);
      bool send_photos_and_text_void_link(var & varSource);
      bool send_only_if_text(var & varSource);
      bool send_all(var & varSource);
      bool send_all_non_expansion(var & varSource);

      string hashtag_first_link(string str);


      virtual void step_cache();

      twitter * target();
      void defer_create_target();
      twitter * create_target();

      virtual bool call_tweet(string strText, stringa straMediaIds, var & varSource);
      virtual bool send_tweet(string strText, stringa straMediaIds, index iAttempt = 1);

      virtual void simple_csv_set_line(stringa & strLine);
      virtual void simple_csv_get_line(stringa & strLine);

      string get(e_data e_data);
      void set(e_data, const string & str);
      bool check_data();

      virtual string remove_hashtags(string strText);
      virtual string on_clean_message(string strText, var & varSource);

      void log(string strModifier, string str);
      void log_source(string str);
      void log_target(string str);
      void general_log(string str);
      string log_path(string strModifier);
      //string source_path();
      //string target_path();

      void inside_loop_tests();

      virtual void log_line(string str, index iLogTarget) override;

      virtual bool http_connection_ok(string strConnectionStatus, index iLogTarget);

      virtual stringa log_header();

      virtual void log_header(index iLogTarget);

      virtual string expand_link(string strLink, index iLog = ::twitter_automator::log_target);

      virtual bool is_expandable_link(string strLink);

      void defer_load_domain_find_replace_file();

      void parse_domain_find_replace(string str);

      string perform_domain_find_replace(string str);

      string perform_url_processing(string str);

   };


} // namespace twitter_automator



