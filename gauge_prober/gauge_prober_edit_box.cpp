#include "framework.h"


namespace gauge_prober
{

   edit_box::edit_box(::aura::application * papp) :
      object(papp),
      ::user::plain_edit(papp),
      m_pen(allocer())
   {
      m_pen->create_solid(5.0, ARGB(255, 0, 0, 0));
   }


   edit_box::~edit_box()
   {

   }


   void edit_box::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      {

         ::draw2d::savedc s(pgraphics);

         pgraphics->OffsetViewportOrg(7, 3);

         ::user::plain_edit::_001OnDraw(pgraphics);

      }

      pgraphics->SelectClipRgn(NULL);

      rect rectClient;

      GetClientRect(rectClient);

      rectClient.deflate(1, 1);

      pgraphics->DrawRect(rectClient, m_pen);

   }


} // namespace gauge_prober




