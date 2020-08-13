#pragma once


namespace twitter_automator
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR task_array :
      virtual public __pointera(task),
      virtual public simple_csv,
      virtual public ::thread
   {
   public:



      container *                      m_pcontainer;
      string                           m_strPath;
      bool                             m_bPendingSave;
      int                              m_iActive;
      domain_find_replace_array        m_domainfindreplacea;
      prefix_stop_before_after_array   m_prefixstopbeforeaftera;
      prefix_before_after_array        m_prefixbeforeaftera;
      stringa                          m_straRemoveText;
      file_time                        m_filetimeRemoveText;
      file_time                        m_filetimeDomainFindReplace;
      file_time                        m_filetimePrefixStopBeforeAfter;
      file_time                        m_filetimePrefixBeforeAfter;
      string_map < expand_link >       m_mapExpandLink;
      string_map < cache_path >        m_mapCachePath;


      task_array(container * pcontainer);
      virtual ~task_array();

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

      void on_notify_change_status(task * ptask);

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


      void defer_load_prefix_stop_before_after_file();

      void parse_prefix_stop_before_after(string str);

      string perform_prefix_stop_before_after(string str);



      void defer_load_prefix_before_after_file();

      void parse_prefix_before_after(string str);

      string perform_prefix_before_after(string str);




      void defer_load_remove_text();

      void parse_remove_text(string str);

      string perform_remove_text(string str);

      string perform_remove_text(string str, string strLine);

      string perform_remove_text_with_link(string str, string strBefore, string strAfter);


      void defer_load_domain_find_replace_file();

      void parse_domain_find_replace(string str);

      string perform_domain_find_replace(string str);



      string perform_url_processing(string str);


   };


} // namespace twitter_automator



