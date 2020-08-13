#pragma once


namespace imagecopy2
{


   class task_dialog :
      virtual public simple_dialog
   {
   public:

      main_form_list *                          m_plist;

      int                                       m_iTask;
      
      task_dialog(::aura::application * papp);
      virtual ~task_dialog();

      virtual void show();

      bool on_control_event(::user::form_window * pview, ::user::control_event * pevent);
   };


} // namespace imagecopy2