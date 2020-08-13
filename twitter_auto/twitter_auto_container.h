#pragma once


namespace twitter_auto
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTO container :
      virtual public object
   {
   public:


      user_array * m_ptaska;

      container(::aura::application * papp);
      virtual ~container();


   };


} // namespace twitter_auto
