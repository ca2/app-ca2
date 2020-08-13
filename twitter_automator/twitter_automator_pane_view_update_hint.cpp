#include "framework.h"


namespace twitter_automator
{


   ::impact_update::::impact_update()
   {
      m_eview = impact_none;
      m_etype = TypeNone;

   }


   ::impact_update::~::impact_update()
   {

   }


   bool ::impact_update::is_type_of(e_type e_type)
   {

      return m_etype == e_type;

   }


   void ::impact_update::set_type(e_type e_type)
   {

      m_etype = e_type;

   }


} // namespace twitter_automator



