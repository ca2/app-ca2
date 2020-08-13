#pragma once


namespace twitter_automator
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR container :
      virtual public object
   {
   public:


      task_array * m_ptaska;

      container(::aura::application * papp);
      virtual ~container();

      virtual void on_status_changed(int iTask);

   };


} // namespace twitter_automator
