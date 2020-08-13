#pragma once


namespace twitter_automator2
{


   class task_dialog :
      virtual public simple_dialog
   {
   public:


      main_form_list *                          m_plist;
      ::twitter_automator::task_array *         m_ptaska;
      index                                     m_iTask;


      task_dialog(::aura::application * papp);
      virtual ~task_dialog();

      virtual void show();

      void on_control_event(
      ::user::control_event * pevent);


   };


} // namespace twitter_automator2