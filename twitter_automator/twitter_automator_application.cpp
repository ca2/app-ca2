#include "framework.h"

//void throw_a_simple_exception_through_one_single_function_boundary(string str)
//{
//
//   throw simple_exception(::get_thread_app(), str + " this is also a very artificial simple exception to test if exception can be safely thrown and caught at this environment (in this specific case: Android)... but across a single function boundary (thrown in function, should be caught in caller...) ...");
//
//}


//extern CLASS_DECL_BASE bool g_bCreateWindowScreen;

namespace twitter_automator
{








   application::application()
   {

      m_ptemplateTwitterAutomatorMain = NULL;

      m_strAppName            = "twitter_automator";
      m_strBaseSupportId      = "ca2_flag";
      m_bLicense              = false;
      m_eexclusiveinstance = ExclusiveInstanceLocal;

      //g_bCreateWindowScreen = false;

   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

      System.factory().creatable_small < ::twitter_automator::main_form_list >();

      System.factory().creatable_small < ::twitter_automator::document >();
      System.factory().creatable_small < ::twitter_automator::frame >();
      System.factory().creatable_small < ::twitter_automator::main_frame >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::twitter_automator::pane_view >();

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
                           System.type_info < pane_view > ()));

      add_document_template(pDocTemplate);

      m_ptemplateTwitterAutomatorMain = pDocTemplate;

      pDocTemplate = canew(::user::single_document_template(
                           this,
                           "main",
                           System.type_info < document >(),
                           System.type_info < frame >(),
                           System.type_info < main_form_list >()));

      add_document_template(pDocTemplate);

      m_ptemplateTwitterAutomatorFormList = pDocTemplate;

      return true;

   }


   void application::term_instance()
   {

      ::core::application::term_instance();

   }


   void application::on_request(__pointer(::create) pcreatecontext)
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

      return "wndfrm_metro";

   }


} // namespace twitter_automator




extern "C"
::aura::library * app_ca2_twitter_automator_get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < ::twitter_automator::application >(papp, "app-ca2/twitter_automator");

}




