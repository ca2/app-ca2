#include "framework.h"

#include "aura/aura/app/acid.inl"

::aura::application * get_acid_app(::aura::application * papp)
{

   return new ::quasar_science_update_utility::application();

}


