#include "framework.h"


namespace gauge_prober
{




   info::info(::aura::application * papp) :
      object(papp),
      m_font(allocer())
   {

      m_flagNonClient.remove(::user::interaction::non_client_background);
      m_flagNonClient.remove(::user::interaction::non_client_focus_rect);


      m_strVersion = "0.08";

      m_font->create_pixel_font("Verdana", 18.0, FW_NORMAL);

   }


   info::~info()
   {


   }


   void info::install_message_routing(::channel * pchannel)
   {

      ::user::impact::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &info::_001OnCreate);

   }

   void info::_001OnCreate(::message::message * pmessage)
   {

      pmessage->previous();


      set_need_layout();

   }



   void info::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnDraw(pgraphics);

      pgraphics->SelectObject(m_font);

      pgraphics->set_text_color(ARGB(255, 255, 255, 255));

      pgraphics->text_out(100, 100, "Application Name: Gauge Prober");
      pgraphics->text_out(100, 150, "Version: " + m_strVersion);


   }


} // namespace gauge_prober

