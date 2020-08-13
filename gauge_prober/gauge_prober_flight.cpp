#include "framework.h"


namespace gauge_prober
{




   flight::flight(::aura::application * papp, e_impact eview) :
      object(papp),
      m_eview(eview),
      m_group1(papp),
      m_group2(papp),
      m_fontTime(allocer()),
      m_penTime(allocer())
   {

      m_iTimeDivisor = 100;

      m_dNow = 0.0;
      m_bStart = false;
      m_fontTime->create_pixel_font("Verdana", 55.0, FW_BOLD);
      m_penTime->create_solid(5.0, ARGB(255, 0, 0, 0));

      m_pbuttonRun = NULL;
      m_pbuttonStop = NULL;
      m_iT2 = 0;
      m_iB2 = 0;
      m_bIn = false;

   }


   flight::~flight()
   {


   }


   void flight::install_message_routing(::channel * pchannel)
   {

      ::user::impact::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &flight::_001OnCreate);
      IGUI_MSG_LINK(WM_MOUSEMOVE, pchannel, this, &flight::_001OnMouseMove);
      IGUI_MSG_LINK(WM_MOUSELEAVE, pchannel, this, &flight::_001OnMouseLeave);

   }

   void flight::_001OnCreate(::message::message * pmessage)
   {

      pmessage->previous();


      {

         m_pbuttonRun = new button(get_context_application());

         ::user::control_descriptor d;

         d.m_id = "button_run";
         d.m_puiParent = this;
         d.m_etype = ::user::control_type_button;

         m_pbuttonRun->create_control(&d);

         m_pbuttonRun->set_window_text("Run");

      }


      {

         m_pbuttonStop = new button(get_context_application());

         ::user::control_descriptor d;

         d.m_id = "button_stop";
         d.m_puiParent = this;
         d.m_etype = ::user::control_type_button;

         m_pbuttonStop->create_control(&d);

         m_pbuttonStop->set_window_text("Stop");

      }

      set_need_layout();

   }


   void flight::_001OnMouseMove(::message::message * pmessage)
   {

      SCAST_PTR(::message::mouse, pmouse, pmessage);

      ::point point = pmouse->m_pt;

      rect rectClient;

      GetClientRect(rectClient);

      ScreenToClient(point);

      if (rectClient.contains(point))
      {

         if (!m_bIn)
         {

            m_bIn = true;
            track_mouse_leave();

         }

         m_iX = point.x;

         m_group1.on_mouse_move();
         m_group2.on_mouse_move();

      }
      else
      {

         if(m_bIn)
         {

            m_bIn = false;

         }

         m_iX = -1.0;

         m_group1.on_mouse_move();
         m_group2.on_mouse_move();

      }


   }

   void flight::_001OnMouseLeave(::message::message * pmessage)
   {
      if (m_bIn)
      {

         m_bIn = false;

      }

      m_iX = -1.0;

      m_group1.on_mouse_move();
      m_group2.on_mouse_move();

   }

   void flight::draw_timer(::draw2d::graphics_pointer & pgraphics)
   {

      ::draw2d::savedc savedc(pgraphics);

      pgraphics->SelectClipRgn(NULL);

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.is_null())
      {

         return;

      }

      //      string strEngineName = engine_name(m_eengine);

      string strTime;

      int iNow = (int)(get_tick_count() - m_dStart);

      int iTimeMouse = -1;

      if (!m_bStart)
      {

         if (m_group1.m_iTimeMouse >= 0)
         {

            iNow = (int) (m_group1.m_iTimeMouse * (double)(1000.0 / m_iTimeDivisor));

            iTimeMouse = iNow;

         }
         else if (m_group2.m_iTimeMouse >= 0)
         {

            iNow = (int) (m_group2.m_iTimeMouse * (double)(1000.0 / m_iTimeDivisor));

            iTimeMouse = iNow;

         }

      }

      int iTime = (int)(iNow / 1000.0);

      int iCentis = (iNow % 1000) / (1000 / m_iTimeDivisor);

      int iTotalSeconds;

      if (m_bStart || iTimeMouse >= 0)
      {

         if (iTime > m_dEnd)
         {

            iTotalSeconds = (int)m_dEnd;

            iCentis = (int)(fmod(m_dEnd, 1000.0) / (double)(1000.0 / m_iTimeDivisor));

         }
         else
         {

            iTotalSeconds = iTime;

         }

      }
      else if (m_dNow <= 0.0)
      {

         iTotalSeconds = (int)m_dEnd;

         iCentis = (int)(fmod(m_dEnd, 1000.0) / (double)(1000.0 / m_iTimeDivisor));

      }
      else if (m_dNow > 0.0)
      {

         iTotalSeconds = (int)m_dNow;

         iCentis = (int)(fmod(m_dNow, 1000.0) / (double)(1000.0 / m_iTimeDivisor));

      }

      int iSeconds = iTotalSeconds % 60;

      int iMinutes = iTotalSeconds / 60;

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      rect rText;

      pgraphics->select_font(m_fontTime);


      int iTop = -60;

      sized size = pgraphics->GetTextExtent("00:00:00");

      size.cx += 5;
      size.cy += 5;

      sized sizeBox(size);

      sizeBox.cx = (LONG)(size.cx * 1.4);
      sizeBox.cy = (LONG)(size.cy * 1.1);

      int iM = (rectClient.right + rectClient.left) / 2;

      int iBorderTop = 5;

      rText.left = (LONG) (iM - size.cx / 2);
      rText.top = iTop + iBorderTop;
      rText.right = (LONG) (iM + size.cx / 2);
      rText.bottom = iTop + (LONG)sizeBox.cy + iBorderTop;

      rect rectBox;

      rectBox.left = (LONG) (iM - sizeBox.cx / 2);
      rectBox.top = iTop + iBorderTop;
      rectBox.right = (LONG) (iM + sizeBox.cx / 2);
      rectBox.bottom = iTop + (LONG)sizeBox.cy + iBorderTop;

      int iT2 = rectBox.bottom + iBorderTop * 3;

      if (iT2 != m_iT2)
      {

         m_iT2 = iT2;

         on_layout();

      }

      strTime.Format("%02d:%02d:%02d", iMinutes, iSeconds, iCentis);

      rect rTime;

      rTime = rectClient;

      rTime.top += 10;

      pgraphics->FillSolidRect(rectBox, ARGB(255, 192, 192, 192));

      pgraphics->DrawRect(rectBox, m_penTime);

      pgraphics->set_text_color(ARGB(255, 0, 0, 0));

      pgraphics->draw_text(strTime, rText, DT_HVCENTER);

   }


   void flight::update_file()
   {



   }

   
   string flight::get_group_error_message(group * pgroup)
   {

      synch_lock sl(mutex());

      stringa stra;

      if (pgroup->m_strError.has_char())
      {

         stra.add(pgroup->m_strError);

      }

      for (auto & pserial : m_serialptra)
      {

         if (pserial->m_pgroup == pgroup && pserial->m_strError.has_char())
         {

            stra.add(pserial->m_strError);

         }

      }

      return stra.implode(", ");

   }


   CLASS_DECL_APP_CA2_GAUGE_PROBER string engine_name(e_engine e)
   {

      switch (e)
      {
      case engine_none:
         return "(NONE)";
      case engine_MERCURY:
         return "MERCURY";
      case engine_PEGASUS:
         return "PEGASUS";
      case engine_OLYMPUS:
         return "OLYMPUS";
      case engine_TITAN:
         return "TITAN";
      case engine_NIKE:
         return "NIKE";
      case engine_LYNX:
         return "LYNX";
      default:
         return "(UNKNOWN)";
      }

   }


   void flight::on_layout()
   {


      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.area() <= 0)
      {

         return;
      }


      int iM = (rectClient.right + rectClient.left) / 2;

      m_iB2 = rectClient.bottom - 90;

      if (m_pbuttonRun != NULL)
      {

         m_pbuttonRun->SetWindowPos(ZORDER_TOP, iM - 210, rectClient.bottom - 55, 200, 50, SWP_SHOWWINDOW);

      }

      if (m_pbuttonStop != NULL)
      {

         m_pbuttonStop->SetWindowPos(ZORDER_TOP, iM + 10, rectClient.bottom - 55, 200, 50, SWP_SHOWWINDOW);

      }


   }



   void flight::serial_start()
   {

      synch_lock sl(mutex());

      for (auto & pserial : m_serialptra)
      {

         pserial->start();

      }

   }

   void flight::serial_stop()
   {

      synch_lock sl(mutex());

      for (auto & pserial : m_serialptra)
      {

         pserial->stop();

      }


   }





} // namespace gauge_prober

