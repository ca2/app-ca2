#include "framework.h"

const char * g_psz_br_init_symbol_app = "";
extern char _binary_data_txt_start;
extern char _binary_data_txt_end;


::aura::application * get_acid_app(::aura::application * pappParent)
{

   return new ::image_display::application();

}


#define BR_INIT_SYMBOL g_psz_br_init_symbol_app

#include "aura/aura/app/acid.inl"


