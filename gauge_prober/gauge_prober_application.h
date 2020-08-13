#pragma once



namespace gauge_prober
{

   enum e_mode
   {

      mode_normal,
      mode_anthill,

   };

   class CLASS_DECL_APP_CA2_GAUGE_PROBER application :
      virtual public ::core::application
   {
   public:

      ::mutex                                        m_mutexSerial;
      string_map < __pointer(::serial::Serial) >          m_mapSerial;



      __pointer(::user::single_document_template)      m_ptemplateMain;

      e_mode         m_emode;


      application();
      virtual ~application();


      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(::create * pcreatecontext) override;

      virtual i64 add_ref();
      virtual i64 dec_ref();

      virtual string preferred_experience();

   };


} // namespace gauge_prober



