#include "framework.h"


namespace twitter_auto
{


   container::container(::aura::application * papp) :
      object(papp)
   {

      m_ptaska = canew(user_array(this));

   }

   container::~container()
   {

      ::multithreading::post_quit_and_wait(m_ptaska, seconds(20));


   }


} // namespace twitter_auto



