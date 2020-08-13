#include "framework.h"

const char * g_psz_br_init_symbol_app = "";
extern char _binary_data_txt_start;
extern char _binary_data_txt_end;


extern
::aura::PFN_GET_NEW_APP g_pfnNewApp;

::aura::application * get_acid_app(::aura::application * pappParent)
{
   return new ::grbl_merge_tool::application();
}
i32 __lnx_main2(i32 argc, char * argv[], const char * pszAppApp);


class app_prelude :
   public aura_prelude
{
public:

   app_prelude() :
      aura_prelude(&get_acid_app)
   {
   }

   virtual ~app_prelude()
   {
   }

   virtual bool construct(app_core * pappcore)
   {

      pappcore->m_bGtkApp = true;

   //   gdk_threads_init();
///
     /// g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);

      //gtk_init (&pappcore->m_pmaindata->m_argc, &pappcore->m_pmaindata->m_argv);

      ///g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

      return true;

   }


};


#include "aura/aura/app/app_prelude.inl"

