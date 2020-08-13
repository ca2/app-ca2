#pragma once


namespace twitter_auto
{


   class task_dialog :
      virtual public simple_dialog
   {
   public:


      main_form_list *                          m_plist;
      ::twitter_auto::user_array *         m_ptaska;
      index                                     m_iTask;


      task_dialog(::aura::application * papp);
      virtual ~task_dialog();

      virtual void show();

      bool on_control_event(::user::form_window * pview, ::user::control_event * pevent);

      virtual void on_update_image_path();

   };


} // namespace twitter_auto