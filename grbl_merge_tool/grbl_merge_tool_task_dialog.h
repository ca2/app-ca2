#pragma once


namespace grbl_merge_tool
{


   class task_dialog :
      virtual public simple_dialog
   {
   public:


      main_form_list *                          m_plist;
      ::grbl_merge_tool::user_array *         m_ptaska;
      index                                     m_iTask;


      task_dialog(::aura::application * papp);
      virtual ~task_dialog();

      virtual void show();

      bool on_control_event(::user::form_window * pview, ::user::control_event * pevent);

      virtual void on_update_image_path();

   };


} // namespace grbl_merge_tool