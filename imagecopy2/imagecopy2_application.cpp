#include "framework.h"

//void throw_a_simple_exception_through_one_single_function_boundary(string str)
//{
//
//   throw simple_exception(::get_thread_app(), str + " this is also a very artificial simple exception to test if exception can be safely thrown and caught at this environment (in this specific case: Android)... but across a single function boundary (thrown in function, should be caught in caller...) ...");
//
//}

namespace imagecopy2
{








   application::application()
   {
      ::aura::system::g_p->m_bThreadToolsForIncreasedFps = false;
      m_bThreadToolsForIncreasedFps = false;
      ::sockets::tcp_socket::s_bReuseSession = false;
      m_emode = mode_anthill;

      m_strAppName            = "imagecopy2";
      m_strBaseSupportId      = "ca2_flag";
      m_bLicense              = false;
      m_eexclusiveinstance = ExclusiveInstanceLocal;




   }


   application::~application()
   {
   }


   bool application::initialize_application()
   {

      System.factory().creatable_small < ::imagecopy2::main_form_list >();

      System.factory().creatable_small < ::imagecopy2::document >();
      System.factory().creatable_small < ::imagecopy2::frame >();
      System.factory().creatable_small < ::imagecopy2::main_frame >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::imagecopy2::pane_view >();
      System.factory().creatable_small < ::imagecopy2::main_view >();
      System.factory().creatable_small < ::imagecopy2::info_view >();
      System.factory().creatable_small < ::imagecopy2::left_view >();

      if(!::core::application::initialize_application())
         return false;


      User.filemanager().std().m_pathDefault = Context.dir().home() / "Image Copy";

      m_dataid.m_id = m_dataid.m_id + ".local://";

      ::user::single_document_template* pDocTemplate;

      pDocTemplate = canew(::user::single_document_template(
                              this,
                              "main",
                              System.type_info < document >(),
                              System.type_info < main_frame > (),
                              System.type_info < main_view > ()));

      add_document_template(pDocTemplate);

      m_ptemplateMain = pDocTemplate;

      return true;

   }


   i32 application::exit_application()
   {

      return ::core::application::exit_application();

   }


   void application::on_request(__pointer(::create) pcreatecontext)
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

      return "wndfrm_anthill";

   }


} // namespace imagecopy2




extern "C"
::aura::library * app_ca2_imagecopy2_get_new_library(::aura::application * papp)
{

   return canew(::aura::single_application_library < ::imagecopy2::application >(papp, "app-ca2/imagecopy2"));

}




