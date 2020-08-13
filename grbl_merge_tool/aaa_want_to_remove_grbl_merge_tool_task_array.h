#pragma once


namespace grbl_merge_tool
{




   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOLMATOR task_array :
      virtual public __pointera(task),
      virtual public simple_csv,
      virtual public ::thread
   {
   public:


      
      main_form_list *     m_pformlist;
      string               m_strPath;
      bool                 m_bPendingSave;
      int                  m_iActive;
      

      task_array(main_form_list * pformlist);
      virtual ~task_array();

      virtual void save();
      
   
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      virtual void stop_all_tasks();
     
      
      virtual void simple_csv_on_load() override;
      virtual void simple_csv_on_save() override;


      virtual void update_task(int iTask);
      virtual void delete_task(int iTask);

      virtual void backup();
      
      void on_notify_change_status(task * ptask);

      int get_active_count() { return m_iActive; }

      //virtual void simple_csv_on_before_load(::count c) override;
      //virtual void simple_csv_set_line(index iLine, stringa & strLine) override;
      //virtual void simple_csv_get_line(index iLine, stringa & strLine) override;
      //virtual ::count simple_csv_get_line_count() override;

      virtual i32 run() override;

      void set_header();

   };


} // namespace grbl_merge_tool



