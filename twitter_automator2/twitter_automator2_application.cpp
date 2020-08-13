#include "framework.h"
#include "aura/net/sockets/sockets.h"

//void throw_a_simple_exception_through_one_single_function_boundary(string str)
//{
//
//   throw simple_exception(::get_thread_app(), str + " this is also a very artificial simple exception to test if exception can be safely thrown and caught at this environment (in this specific case: Android)... but across a single function boundary (thrown in function, should be caught in caller...) ...");
//
//}

namespace twitter_automator2
{








   application::application()
   {
      ::sockets::tcp_socket::s_bReuseSession = false;
      m_emode = mode_anthill;

      m_ptemplateTwitterAutomatorMain = NULL;

      m_strAppName            = "app-ca2/twitter_automator2";
      m_strBaseSupportId      = "app-ca2/twitter_automator2";
      m_bLicense              = false;
      m_eexclusiveinstance    = ExclusiveInstanceLocal;




   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

      //DWORD dw1 = get_tick_count();
      //output_debug_string("dir test start");

      //auto & dir = Application.dir();

      //for (index i = 0; i < 500000; i++)
      //{

      //   if (!dir.is("C:\\a\\pendrivetest\\001 PriceBargain"))
      //   {
      //      throw 0;
      //   }
      //   if (dir.is("C:\\a\\pendrivetest\\001 PriceBargaind"))
      //   {
      //      throw 0;
      //   }
      //   if (!dir.is("C:\\a\\pendrivetest\\002 BargainHooked"))
      //   {
      //      throw 0;
      //   }
      //   if (!dir.is("C:\\a\\pendrivetest\\003 Price Bargain"))
      //   {
      //      throw 0;
      //   }
      //   if (!dir.is("C:\\a\\pendrivetest\\004 DiscountSteals"))
      //   {
      //      throw 0;
      //   }
      //   if (!dir.is("C:\\a\\pendrivetest\\005 NiceBargain"))
      //   {
      //      throw 0;
      //   }
      //   if (!dir.is("C:\\a\\pendrivetest\\media"))
      //   {
      //      throw 0;
      //   }
      //   if (!dir.is("C:\\a\\pendrivetest"))
      //   {
      //      throw 0;
      //   }

      //}

      //output_debug_string("dir test end");
      //DWORD dw2 = get_tick_count();
      //DWORD dw = dw2 - dw1;

      System.factory().creatable_small < ::twitter_automator2::main_form_list >();

      System.factory().creatable_small < ::twitter_automator2::document >();
      System.factory().creatable_small < ::twitter_automator2::frame >();
      System.factory().creatable_small < ::twitter_automator2::main_frame >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::twitter_automator2::pane_view >();

      if(!::core::application::init_instance())
         return false;


      User.filemanager()->m_pathDefault = Context.dir().home() / "Twitter Automator";

      set_local_data();

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

      ::twitter_automator::application::term_instance();

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


} // namespace twitter_automator2




extern "C"
::aura::library * app_ca2_twitter_automator2_get_new_library(::aura::application * papp)
{

   return canew(::aura::single_application_library < ::twitter_automator2::application >(papp, "app-ca2/twitter_automator2"));

}




