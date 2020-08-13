#include "framework.h"


namespace grbl_merge_tool
{


   ::impact_update::::impact_update()
   {

      m_epane = pane_none;

      m_etype = type_none;

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


} // namespace grbl_merge_tool



