#pragma once


namespace twitter_automator2
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR task :
      virtual public ::thread
   {
   public:


      enum ::estatus    
      {

         status_none,
         status_on,
         status_off,

      };



      // serializable strings
      enum e_data
      {
         data_status = 0,
         data_source,
         data_source_api_consumer_key,
         data_source_api_consumer_sec,
         data_source_proxy,
         data_hint_target,
         data_target_api_consumer_key,
         data_target_api_consumer_sec,
         data_target_api_token_key,
         data_target_api_token_sec,
         data_target_proxy,
         data_option,
         data_same_consumer,
         data_same_proxy,
         data_count,
      };

      stringa              m_stra;
      task_array *         m_ptaska;
      int                  m_iIndex;

      string               m_strLogDate;
      int                  m_iLogIndex;

      // dynamic
      DWORD                m_dwLastAuth;
      twitter *            m_ptarget;
      ::estatus                 m_estatusRequest;

      stringa              m_straIgnoreTweetDomain;
      string_to_string     m_straPhoto;


      task(task_array * ptaska);
      virtual ~task();

      virtual i32 run() override;

      stringa get_media_ids(string_to_string & str);
      string_to_string get_photos_with_media_id(var & varSource);
      string_to_string get_media_url_media_ids(var & varSource);
      string_to_string get_expanded_url(var & varSource);
      string_to_string get_expanded_photos(var & varSource);
      string expand_url(string strText, var & varSource);

      bool send(var & varSource);
      bool is_source_ok(var & varSource);
      bool send_photos_text_and_one_link(var & varSource);
      bool send_photos(var & varSource);
      bool send_only_with_photo__text_and_link(var & varSource);
      bool send_photos_and_text(var & varSource);
      bool send_text(var & varSource);
      bool send_only_with_photo_and_text(var & varSource);
      bool send_photos_and_text_void_link(var & varSource);
      bool send_only_if_text(var & varSource);
      bool send_all(var & varSource);

      twitter * target();
      void defer_create_target();
      twitter * create_target();

      virtual bool tweet(string strText, stringa straMediaIds, int iAttempt = 1);

      virtual void simple_csv_set_line(stringa & strLine);
      virtual void simple_csv_get_line(stringa & strLine);

      string get(e_data e_data);
      void set(e_data, const string & str);
      bool check_data();

      void log(string strFile, string str);
      void log_source(string str);
      void log_target(string str);
      void general_log(string str);
      string log_path(string strModifier);
      string source_path();
      string target_path();

      void inside_loop_tests();

   };


} // namespace twitter_automator2



