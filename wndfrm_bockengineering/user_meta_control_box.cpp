#include "framework.h"


MetaControlBox::MetaControlBox(::aura::application * papp) :
object(papp),
::user::interaction(papp),
::user::wndfrm::frame::control_box(papp)
{
   m_iDefaultButtonMargin = 0;

   m_fontMarlett->create_point_font("Marlett", 24);

}


MetaControlBox::~MetaControlBox()
{

}


void MetaControlBox::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
{


}

void MetaControlBox::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
{

   if(GetTopLevel()->frame_is_transparent() && GetTopLevel() != GetActiveWindow())
   {

      return;

   }

   rect rectClient;

   GetClientRect(rectClient);

   if (rectClient.area() <= 0)
      return;

   pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

   COLORREF crBackground;

   if(GetTopLevel()->frame_is_transparent())
   {

      crBackground = ARGB(84,colorref_get_r_value(m_crBackground),colorref_get_g_value(m_crBackground),colorref_get_b_value(m_crBackground));

   }
   else
   {

      crBackground = m_crBackground;

   }

   pgraphics->FillSolidRect(rectClient, m_crBackground);

}


void MetaControlBox::_001OnShowWindow(::message::message * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

}

void MetaControlBox::_001OnLButtonDown(::message::message * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

}

void MetaControlBox::_001OnLButtonUp(::message::message * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   RedrawWindow();

}



void MetaControlBox::_001OnTimer(::timer * ptimer)
{

   ::user::wndfrm::frame::control_box::_001OnTimer(ptimer);

}


void MetaControlBox::install_message_routing(::channel * pchannel)
{
   
   ::user::wndfrm::frame::control_box::install_message_routing(pchannel);

   IGUI_MSG_LINK(WM_SHOWWINDOW, pchannel, this, &MetaControlBox::_001OnShowWindow);
   IGUI_MSG_LINK(WM_LBUTTONDOWN, pchannel, this, &MetaControlBox::_001OnLButtonDown);
   IGUI_MSG_LINK(WM_LBUTTONUP, pchannel, this, &MetaControlBox::_001OnLButtonUp);
//   IGUI_MSG_LINK(WM_SIZE, pchannel, this, &MetaControlBox::_001OnSize);
//   //IGUI_MSG_LINK(WM_TIMER, pchannel, this, &MetaControlBox::_001OnTimer);

}

