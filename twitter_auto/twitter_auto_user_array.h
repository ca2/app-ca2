#pragma once


namespace twitter_auto
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTO user_array :
      virtual public __pointera(user),
      virtual public simple_csv,
      virtual public ::thread
   {
   public:

      class domain_find_replace
      {
      public:

         string      m_strDomain;
         string      m_strFind;
         string      m_strReplace;

         bool apply(string & strLink);

      };

      class domain_find_replace_array :
         virtual public ::array < domain_find_replace >
      {
      public:

         bool apply(string & strLink);

      };

      class prefix_before_after
      {
      public:

         string      m_strPrefix;
         string      m_strBefore;
         string      m_strAfter;

         bool apply(string & strLink);

      };

      class prefix_before_after_array :
         virtual public ::array < prefix_before_after >
      {
      public:

         bool apply(string & strLink);

      };


      class prefix_stop_before_after
      {
      public:

         string      m_strPrefix;
         string      m_strStopChars;
         string      m_strBefore;
         string      m_strAfter;

         bool apply(string & strLink);

      };

      class prefix_stop_before_after_array :
         virtual public ::array < prefix_stop_before_after >
      {
      public:

         bool apply(string & strLink);

      };


      class expand_link
      {
      public:

         DWORD       m_dwStart;
         string      m_strExpanded;

         expand_link()
         {
            m_dwStart = 0;
         }

      };

      class cache_path
      {
      public:

         DWORD       m_dwStart;
         string      m_strTime;

         cache_path()
         {
            m_dwStart = 0;
         }

      };

      ::mutex                            m_mutexError;
      std::stringstream                m_strError;
      DWORD                            m_dwLastErrorChange;
      container *                      m_pcontainer;
      string                           m_strPath;
      bool                             m_bPendingSave;
      int                              m_iActive;
      domain_find_replace_array        m_domainfindreplacea;
      prefix_stop_before_after_array   m_prefixstopbeforeaftera;
      prefix_before_after_array        m_prefixbeforeaftera;
      file_time                        m_filetimeDomainFindReplace;
      file_time                        m_filetimePrefixStopBeforeAfter;
      file_time                        m_filetimePrefixBeforeAfter;
      string_map < expand_link >       m_mapExpandLink;
      string_map < cache_path >        m_mapCachePath;

      ::mutex                                                       m_mutexMediaId;
      map < stringa, const stringa &, stringa, const stringa & > m_mediaids;


      user_array(container * pcontainer);
      virtual ~user_array();

      virtual void save();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      virtual void simple_csv_on_load() override;
      virtual void simple_csv_on_save() override;


      virtual void update_task(index iTask);
      virtual void delete_task(index iTask);

      virtual void start_task(index iTask);
      virtual void stop_task(index iTask);

      virtual void start_all_tasks();
      virtual void stop_all_tasks();

      virtual void backup();

      void on_notify_change_status(user * ptask);

      int get_active_count()
      {
         return m_iActive;
      }

      //virtual void simple_csv_on_before_load(::count c) override;
      //virtual void simple_csv_set_line(index iLine, stringa & strLine) override;
      //virtual void simple_csv_get_line(index iLine, stringa & strLine) override;
      //virtual ::count simple_csv_get_line_count() override;

      virtual void run() override;

      void set_header();

      bool set_item_text(index iItem, index iSubItem, string strText);


      stringa path_get_media_ids(user * puser, twitter * ptwitter, const stringa & stra);

      void defer_load_prefix_stop_before_after_file();

      void parse_prefix_stop_before_after(string str);

      string perform_prefix_stop_before_after(string str);



      void defer_load_prefix_before_after_file();

      void parse_prefix_before_after(string str);

      string perform_prefix_before_after(string str);



      void defer_load_domain_find_replace_file();

      void parse_domain_find_replace(string str);

      string perform_domain_find_replace(string str);



      string perform_url_processing(string str);


   };


} // namespace twitter_auto



