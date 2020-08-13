#include "framework.h"

namespace os
{


   font::font()
   {

      m_pdescription = NULL;

   }
   font::~font()
   {
      destroy();

   }

   bool font::create_point_font(int iPoint, const char * pszFontFamily, int iWeight)
   {

      destroy();

      m_pdescription = pango_font_description_new();

      if (m_pdescription == NULL)
      {

         return false;

      }

      pango_font_description_set_family(m_pdescription, pszFontFamily);

      pango_font_description_set_size(m_pdescription, iPoint * PANGO_SCALE / 10);

      pango_font_description_set_weight(m_pdescription, (PangoWeight) iWeight);

      return true;

   }


   void font::destroy()
   {

      if (m_pdescription == NULL)
      {

         return;

      }

      pango_font_descriptions_free(&m_pdescription, 1);

      m_pdescription = NULL;

   }



} //  namespace os


