#pragma once



namespace twitter_auto
{

   enum e_mode
   {

      mode_normal,
      mode_anthill,

   };

   class CLASS_DECL_APP_CORE_TWITTER_AUTO application :
      virtual public ::core::application
   {
   public:


      ::user::single_document_template *     m_ptemplateTwitterAutomatorFormList;
      ::user::single_document_template *     m_ptemplateTwitterAutomatorMain;

      e_mode         m_emode;


      application();
      virtual ~application();


      virtual bool init_instance();
      virtual void term_instance() override;

      virtual void on_request(::create * pcreatecontext);

      virtual i64 add_ref();
      virtual i64 dec_ref();

      virtual string preferred_experience();

   };


} // namespace twitter_auto



