#pragma once



namespace imagecopy2
{

   enum e_mode
   {

      mode_normal,
      mode_anthill,

   };

   class CLASS_DECL_APP_CA2_IMAGECOPY2 application :
      virtual public ::core::application
   {
   public:

      __pointer(::user::single_document_template)      m_ptemplateMain;

      e_mode         m_emode;


      application();
      virtual ~application();


      virtual bool initialize_application();
      virtual i32  exit_application() override;

      virtual void on_request(__pointer(::create) pcreatecontext);

      virtual i64 add_ref();
      virtual i64 dec_ref();

      virtual string preferred_experience();

   };


} // namespace imagecopy2



