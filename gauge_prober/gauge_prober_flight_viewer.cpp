#include "framework.h"


namespace gauge_prober
{




   flight_viewer::flight_viewer(::aura::application * papp) :
      object(papp),
      flight(papp, impact_flight_viewer)
   {

      m_dNow = 0.0;
      m_bStart = false;
      m_flagNonClient.remove(::user::interaction::non_client_background);
      m_flagNonClient.remove(::user::interaction::non_client_focus_rect);

   }


   flight_viewer::~flight_viewer()
   {

   }


   void flight_viewer::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void flight_viewer::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }



   void flight_viewer::install_message_routing(::channel * pchannel)
   {

      flight::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &flight_viewer::_001OnCreate);

   }




   void flight_viewer::_001OnCreate(::message::message * pmessage)
   {

      pmessage->previous();

      m_group1.create(this, 1);

      m_group2.create(this, 2);


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


   void flight_viewer::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnDraw(pgraphics);

      m_group1.on_draw(pgraphics);

      m_group2.on_draw(pgraphics);


      draw_timer(pgraphics);





   }

   void flight_viewer::start()
   {

      DWORD dwStartTime = get_tick_count();

      if (!m_bStart)
      {

         dwStartTime -= (DWORD) (m_dNow * 1000.0);

      }

      m_dStart = dwStartTime;

      m_bStart = true;

   }


   void flight_viewer::stop()
   {

      DWORD dwNow = get_tick_count();

      if(m_bStart)
      {

         m_dNow = MIN((dwNow - m_dStart) / 1000.0, MAX(m_group1.m_dEnd, m_group2.m_dEnd));

      }
      else
      {

         m_dNow = 0.0;

      }

      m_bStart = false;





   }


   void flight_viewer::on_layout()
   {

      flight::on_layout();

      rect rectClient;

      GetClientRect(rectClient);

      if (rectClient.area() <= 0)
      {

         return;
      }

      rect rectGroups(rectClient);

      rectGroups.top = m_iT2;

      rectGroups.bottom = m_iB2;

      int iM = rectGroups.width() / 2;

      m_group1.m_rect = rectGroups;
      m_group1.m_rect.right = iM;

      m_group2.m_rect = rectGroups;
      m_group2.m_rect.left = iM;

      update_file();


   }


   bool flight_viewer::on_control_event(::user::control_event * pevent)
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

      if (m_group2.on_control_event(pevent))
      {

         return true;

      }


      return false;

   }


   void flight_viewer::update_file()
   {

      m_group1.update_file();

      m_group2.update_file();

      m_dEnd = MAX(m_group1.m_dEnd, m_group2.m_dEnd);


   }


} // namespace gauge_prober





