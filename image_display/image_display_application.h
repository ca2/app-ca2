#pragma once

void show_main_dialog();
void show_wait_dialog();


namespace image_display
{


   class CLASS_DECL_APP_CORE_HELLOAURA application :
      virtual public ::aura::application
   {
   public:

      string m_strImageId;

      
      __pointera(socket_thread)      m_socketthreada;
      

      window *       m_pwindow;




      application();
      virtual ~application();

      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(::create * pcreate) override;

      virtual i64 add_ref() override;
      virtual i64 dec_ref() override;


   };

   extern application * g_papp;

} // namespace image_display




