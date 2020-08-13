#include "framework.h"
#include "aura/net/sockets/sockets.h"

//void throw_a_simple_exception_through_one_single_function_boundary(string str)
//{
//
//   throw simple_exception(::get_thread_app(), str + " this is also a very artificial simple exception to test if exception can be safely thrown and caught at this environment (in this specific case: Android)... but across a single function boundary (thrown in function, should be caught in caller...) ...");
//
//}

namespace gauge_prober
{








   application::application() :
      m_mutexSerial(this)
   {
      ::aura::system::g_p->m_bThreadToolsForIncreasedFps = false;
      m_bThreadToolsForIncreasedFps = false;
      ::sockets::tcp_socket::s_bReuseSession = false;
      m_emode = mode_anthill;

      m_strAppName            = "gauge_prober";
      m_strBaseSupportId      = "ca2_flag";
      m_bLicense              = false;
      m_eexclusiveinstance = ExclusiveInstanceLocal;




   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

      System.factory().creatable_small < ::gauge_prober::document >();
      System.factory().creatable_small < ::gauge_prober::frame >();
      System.factory().creatable_small < ::gauge_prober::main_frame >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::gauge_prober::pane_view >();
      System.factory().creatable_small < ::gauge_prober::flight_viewer >();
      System.factory().creatable_small < ::gauge_prober::flight_simulator >();
      System.factory().creatable_small < ::gauge_prober::control_terminal >();
      System.factory().creatable_small < ::gauge_prober::info >();


      if(!::core::application::init_instance())
         return false;


      User.filemanager()->m_pathDefault = Context.dir().home() / "Gauge Prober";

      set_local_data_key_modifier();

      ::user::single_document_template* pDocTemplate;

      pDocTemplate = canew(::user::single_document_template(
                           this,
                           "main",
                           System.type_info < document >(),
                           System.type_info < main_frame > (),
                           System.type_info < pane_view > ()));

      add_document_template(pDocTemplate);

      m_ptemplateMain = pDocTemplate;

      return true;

   }


   void application::term_instance()
   {

      ::core::application::term_instance();

   }


   void application::on_request(::create * pcreatecontext)
   {


      if(m_ptemplateMain->get_document_count() == 0)
      {

         m_ptemplateMain->request_create(pcreatecontext);

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

      return "wndfrm_bockengineering";

   }


} // namespace gauge_prober





::aura::library * app_ca2_gauge_prober_get_new_library(::aura::application * papp)
{

   return canew(::aura::single_application_library < ::gauge_prober::application >(papp, "app-ca2/gauge_prober"));

}






