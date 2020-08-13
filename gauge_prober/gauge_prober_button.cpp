#include "framework.h"

namespace gauge_prober
{

   button::button(::aura::application * papp) :
      object(papp),
      ::user::button(papp),
      m_pen(allocer())
   {
      m_pen->create_solid(5.0, ARGB(255, 0, 0, 0));
   }


   button::~button()
   {

   }

   void button::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::button::_001OnDraw(pgraphics);

      pgraphics->SelectClipRgn(NULL);

      rect rectClient;

      GetClientRect(rectClient);

      rectClient.deflate(1, 1);

      pgraphics->DrawRect(rectClient, m_pen);

   }

} // namespace gauge_prober




