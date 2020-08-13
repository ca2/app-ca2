#include "framework.h"


namespace gauge_prober
{




   control_terminal::control_terminal(::aura::application * papp) :
      object(papp),
      flight(papp, impact_control_terminal),
      m_fontTime(allocer())
   {

      m_flagNonClient.remove(::user::interaction::non_client_background);
      m_flagNonClient.remove(::user::interaction::non_client_focus_rect);

      m_fontTime->create_pixel_font("Verdana", 20.0, FW_BOLD);


   }


   control_terminal::~control_terminal()
   {

   }


   void control_terminal::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void control_terminal::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }



   void control_terminal::install_message_routing(::channel * pchannel)
   {

      flight::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &control_terminal::_001OnCreate);
      IGUI_MSG_LINK(WM_LBUTTONDOWN, pchannel, this, &control_terminal::_001OnLButtonDown);
      IGUI_MSG_LINK(WM_MOUSEMOVE, pchannel, this, &control_terminal::_001OnMouseMove);
      IGUI_MSG_LINK(WM_LBUTTONUP, pchannel, this, &control_terminal::_001OnLButtonUp);

   }




   void control_terminal::_001OnCreate(::message::message * pmessage)
   {

      pmessage->previous();

      synch_lock sl(mutex());

      m_group1.create(this, 1);

      m_serialptra.add(canew(serial(get_context_application(), "COM1", &m_group1)));

      m_serialptra.add(canew(serial(get_context_application(), "COM2", &m_group1)));

      //m_group2.m_strSerial = "COM2";

      //m_group2.create(this, 2);



      set_need_layout();

   }

   void control_terminal::_001OnLButtonDown(::message::message * pmessage)
   {

      SCAST_PTR(::message::mouse, pmouse, pmessage);

      ::point point;

      pt = pmouse->m_pt;

      ScreenToClient(point);

      if (m_group1.m_throttle.m_rect.contains(point))
      {

         m_group1.m_throttle.on_mouse_down(point);

         SetCapture();

         pmouse->m_bRet = true;
      }


   }

   void control_terminal::_001OnMouseMove(::message::message * pmessage)
   {

      SCAST_PTR(::message::mouse, pmouse, pmessage);

      ::point point;

      pt = pmouse->m_pt;

      ScreenToClient(point);

      if (m_group1.m_throttle.m_bDrag)
      {

         m_group1.m_throttle.on_mouse_move(point);

         pmouse->m_bRet = true;

      }


   }

   void control_terminal::_001OnLButtonUp(::message::message * pmessage)
   {

      SCAST_PTR(::message::mouse, pmouse, pmessage);

      ::point point;

      pt = pmouse->m_pt;

      ScreenToClient(point);

      if (m_group1.m_throttle.m_bDrag)
      {

         m_group1.m_throttle.on_mouse_up(point);

         ReleaseCapture();

         pmouse->m_bRet = true;

      }


   }



   void control_terminal::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnDraw(pgraphics);

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.is_null())
      {

         return;

      }

      //      string strEngineName = engine_name(m_eengine);

      /*string strTime;

      int iTime = (int)(((double)get_tick_count() - m_dStart) / 1000.0);

      int iTotalSeconds;

      if (m_bStart)
      {

         iTotalSeconds = (int)MIN(m_dEnd, iTime);

      }
      else if (m_dNow <= 0.0)
      {

         iTotalSeconds = (int)m_dEnd;

      }
      else if (m_dNow > 0.0)
      {

         iTotalSeconds = (int)m_dNow;

      }

      int iSeconds = iTotalSeconds % 60;

      int iMinutes = iTotalSeconds / 60;

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      strTime.Format("%d:%02d", iMinutes, iSeconds);

      rect rTime;

      rTime = rectClient;

      rTime.top += 10;

      pgraphics->set_text_color(ARGB(255, 255, 255, 255));

      pgraphics->select_font(m_fontTime);

      pgraphics->draw_text(strTime, rTime, DT_TOPCENTER);*/



      m_group1.on_draw(pgraphics);

      ::draw2d::pen_pointer pen(allocer());

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      if (m_bStart)
      {

         pgraphics->set_text_color(ARGB(255, 0, 128, 0));

         pen->create_solid(5.0, ARGB(255, 0, 128, 0));

      }
      else
      {

         pgraphics->set_text_color(ARGB(255, 128, 128, 128));

         pen->create_solid(5.0, ARGB(255, 128, 128, 128));

      }


      ::draw2d::font_pointer font(allocer());

      font->create_pixel_font("Verdana", 42.0, 800);

      pgraphics->SelectObject(font);

      point ptCenter = rectClient.center();

      rect rectCircle;

      rectCircle.left = 0;
      rectCircle.top = 0;
      rectCircle.right = rectClient.minimum_dimension() / 15;
      rectCircle.bottom = rectClient.minimum_dimension() / 15;

      rect r1;

      r1 = rectClient;

      r1.right = ptCenter.x;

      r1.top = ptCenter.y;

      pgraphics->draw_text("Running", r1, DT_HVCENTER);

      r1.top += (LONG) (font->m_dFontSize * 3);

      rectCircle.Align(align_center, r1);

      if (m_bStart)
      {

         pgraphics->FillEllipse(rectCircle);

      }

      pgraphics->SelectObject(pen);

      pgraphics->DrawEllipse(rectCircle);



      //m_group2.on_draw(pgraphics);

      pen->m_bUpdated = true;

      if (m_bStart)
      {

         pgraphics->set_text_color(ARGB(255, 128, 128, 128));

         pen->create_solid(5.0, ARGB(255, 128, 128, 128));

      }
      else
      {

         pgraphics->set_text_color(ARGB(255, 128, 0, 0));

         pen->create_solid(5.0, ARGB(255, 128, 0, 0));

      }

      r1 = rectClient;

      r1.left = ptCenter.x;

      r1.top = ptCenter.y;

      pgraphics->draw_text("Stopped", r1, DT_HVCENTER);

      r1.top += (LONG) (font->m_dFontSize * 3);

      rectCircle.Align(align_center, r1);

      if (!m_bStart)
      {

         pgraphics->FillEllipse(rectCircle);

      }

      pgraphics->SelectObject(pen);

      pgraphics->DrawEllipse(rectCircle);


      //select_font(pgraphics, ::user::font_static);
      //pgraphics->set_text_color(ARGB(255, 255, 255, 255));
      //{
      // string str1;
      // str1.Format("%d Files", 2);
      // pgraphics->text_out(20, 20, str1);
      //}
      //{
      // string str2;
      // str2.Format("%d Files Selected", 1);
      // pgraphics->text_out(20, 40, str2);

      //}

   }

   void control_terminal::start()
   {

      DWORD dwStartTime = get_tick_count();

      if (!m_bStart)
      {

         dwStartTime -= (DWORD)(m_dNow * 1000.0);

      }

      m_dStart = dwStartTime;

      m_bStart = true;

      serial_start();

//      m_group2.start();

   }


   void control_terminal::stop()
   {

      DWORD dwNow = get_tick_count();

      if (m_bStart)
      {

         m_dNow = MIN((dwNow - m_dStart) / 1000.0, MAX(m_group1.m_dEnd, m_group2.m_dEnd));

      }
      else
      {

         m_dNow = 0.0;

      }

      m_bStart = false;

      serial_stop();

      //m_group2.stop();



   }


   void control_terminal::on_layout()
   {

      flight::on_layout();

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.area() <= 0)
      {

         return;
      }

      rect rectGroups(rectClient);

      rectGroups.top += 5;

      rectGroups.bottom = m_iB2;

      m_group1.m_rect = rectGroups;

      int iM = rectGroups.width() / 2;

      update_file();


   }


   bool control_terminal::on_control_event(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_button_clicked
            || pevent->m_eevent == ::user::event_enter_key)
      {

         if (pevent->m_puie->m_id == "button_run")
         {

            start();
            return true;

         }
         else if (pevent->m_puie->m_id == "button_stop")
         {

            stop();
            return true;
         }


      }


      if (m_group1.on_control_event(pevent))
      {

         return true;

      }

      //if (m_group2.on_control_event(pevent))
      //{

      //   return true;

      //}


      return false;

   }


   void control_terminal::update_file()
   {

      m_group1.update_file();

      //m_group2.update_file();

      //m_dEnd = MAX(m_group1.m_dEnd, m_group2.m_dEnd);

      m_dEnd = m_group1.m_dEnd;


   }

} // namespace gauge_prober





