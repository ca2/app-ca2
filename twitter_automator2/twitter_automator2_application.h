#pragma once



namespace twitter_automator2
{

   enum e_mode
   {

      mode_normal,
      mode_anthill,

   };

   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR2 application :
      virtual public ::twitter_automator::application
   {
   public:



      e_mode         m_emode;


      application();
      virtual ~application();


      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(::create * pcreatecontext);

      virtual i64 add_ref();
      virtual i64 dec_ref();

      virtual string preferred_experience();

   };


} // namespace twitter_automator2



