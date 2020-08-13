#include "framework.h"
#include "aura/net/net_sockets.h"


namespace twitter_auto
{


   application::application()
   {
      ::sockets::tcp_socket::s_bReuseSession = false;
      m_emode = mode_anthill;

      m_ptemplateTwitterAutomatorMain = NULL;

      m_strAppName            = "twitter_auto";
      m_strBaseSupportId      = "ca2_flag";
      m_bLicense              = false;
      m_eexclusiveinstance = ExclusiveInstanceLocal;




   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

      System.factory().creatable_small < ::twitter_auto::main_form_list >();

      System.factory().creatable_small < ::twitter_auto::document >();
      System.factory().creatable_small < ::twitter_auto::frame >();
      System.factory().creatable_small < ::twitter_auto::main_frame >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::twitter_auto::pane_view >();

      if(!::core::application::init_instance())
         return false;


      User.filemanager()->m_pathDefault = Context.dir().home() / "Twitter Auto";

      set_local_data_key_modifier();

      ::user::single_document_template* pDocTemplate;

      pDocTemplate = canew(::user::single_document_template(
                           this,
                           "main",
                           System.type_info < document >(),
                           System.type_info < main_frame > (),
                           System.type_info < main_form_list > ()));

      add_document_template(pDocTemplate);

      m_ptemplateTwitterAutomatorMain = pDocTemplate;

      return true;

   }


   void application::term_instance()
   {

      ::twitter_auto::application::term_instance();

   }


   void application::on_request(::create * pcreatecontext)
   {

      if(m_ptemplateTwitterAutomatorMain->get_document_count() == 0)
      {

         m_ptemplateTwitterAutomatorMain->request_create(pcreatecontext);

      }

   }





   i64 application::add_ref()
   {

      return ::object::add_ref();

   }

   i64 application::dec_ref()
   {

      return ::object::dec_ref();

   }


   string application::preferred_experience()
   {

      return "wndfrm_anthill";

   }


} // namespace twitter_auto




extern "C"
::aura::library * app_ca2_twitter_auto_get_new_library(::aura::application * papp)
{

   return canew(::aura::single_application_library < ::twitter_auto::application >(papp, "app-ca2/twitter_auto"));

}




