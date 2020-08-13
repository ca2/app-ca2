#pragma once



namespace twitter_automator
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR application :
      virtual public ::core::application
   {
   public:


      ::user::single_document_template *     m_ptemplateTwitterAutomatorFormList;
      ::user::single_document_template *     m_ptemplateTwitterAutomatorMain;


      application();
      virtual ~application();


      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(__pointer(::create) pcreatecontext);

      virtual i64 add_ref();
      virtual i64 dec_ref();

      virtual string preferred_experience();

   };


} // namespace twitter_automator



