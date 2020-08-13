#include "framework.h"


namespace twitter_automator
{


   container::container(::aura::application * papp) :
      object(papp)
   {

      m_ptaska = canew(task_array(this));

   }

   container::~container()
   {

      ::multithreading::post_quit_and_wait(m_ptaska, seconds(20));


   }

   void container::on_status_changed(int iTask)
   {


   }


} // namespace twitter_automator



