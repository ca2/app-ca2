#include "framework.h"
#include <stdio.h>


namespace gauge_prober
{

   line_chart::line_chart(::aura::application * papp) :
      ::object(papp),
      m_sppen(allocer()),
      m_sppenRun(allocer()),
      m_sppenMouse(allocer()),
      m_sppenBorder(allocer()),
      m_spbrushBk(allocer()),
      m_fontTitle(allocer()),
      m_fontScale(allocer())
   {

      m_pgroup = NULL;

      m_strTitle = "Title";

      defer_create_mutex();

      m_sppenMouse->create_solid(1.0, ARGB(255, 128, 0, 0));
      m_sppenRun->create_solid(1.0, ARGB(255, 0, 128, 0));
      m_sppen->create_solid(2.0, ARGB(255, 255, 0, 0));
      m_sppenBorder->create_solid(5.0, ARGB(255, 0, 0, 0));
      m_spbrushBk->create_solid(ARGB(255, 192, 192, 193));

      m_fontTitle->create_pixel_font("Verdana", 18.0, FW_BOLD);
      m_fontScale->create_pixel_font("Verdana", 12.0);

   }


   line_chart::~line_chart()
   {


   }


   void line_chart::create(group * pgroup)
   {

      m_pgroup = pgroup;

   }


   void line_chart::on_draw(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_pgroup == NULL || m_pgroup->m_pflight == NULL)
      {

         return;

      }

      synch_lock sl(mutex());

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillRect(m_rect, m_spbrushBk);

      pgraphics->set_text_color(ARGB(205, 255, 255, 255));

      pgraphics->selectFont(m_fontTitle);


      //double dMin = 100000.0;

      double dMin = m_dMin;

      double dMax = m_dMax;

      double dSpan = dMax - dMin;

      dSpan *= 1.2;

      dMin -= dSpan * 0.1;

      double dWSpan = m_pgroup->m_pflight->m_dEnd;

      index iTimeRun = -1;

      m_pgroup->m_iTimeMouse = -1;

      if (m_pgroup->m_pflight->m_bStart)
      {

         iTimeRun = (index)((((double)get_tick_count() - m_pgroup->m_pflight->m_dStart) / (double)(1000.0 / m_pgroup->m_pflight->m_iTimeDivisor)));

      }

      if(m_pgroup->m_dX >= 0.0)
      {

         m_pgroup->m_iTimeMouse = (int)(m_pgroup->m_dX * m_pgroup->m_pflight->m_dEnd * m_pgroup->m_pflight->m_iTimeDivisor);

         if (m_pgroup->m_iTimeMouse >= m_pointda.get_size())
         {

            m_pgroup->m_iTimeMouse = -1;

         }

      }


      if (iTimeRun < 0 && m_pgroup->m_iTimeMouse < 0)
      {

         pgraphics->text_out(m_rect.left + 10, m_rect.top + 10, m_strTitle);

      }

      if (m_pointda.has_elements())
      {

         if (iTimeRun >= 0)
         {
            char sz[256];


            ZERO(sz);
            if (m_etype == type_egt)
            {

               sprintf(sz, unitext("EGT %0.1f°C"), m_pointda[iTimeRun].y);

            }
            else if (m_etype == type_rpm)
            {

               sprintf(sz, unitext("%d RPM"), (int)m_pointda[iTimeRun].y);

            }
            else if (m_etype == type_throttle)
            {

               sprintf(sz, unitext("%0.1f%% Throttle"), m_pointda[iTimeRun].y);

            }

            if (strlen(sz) > 0)
            {

               pgraphics->set_text_color(ARGB(255, 0, 128, 0));

               pgraphics->text_out(m_rect.left + 10, m_rect.top + 10, sz);

            }

         }

         if (m_pgroup->m_iTimeMouse >= 0)
         {
            char sz[256];


            ZERO(sz);
            if (m_etype == type_egt)
            {

               sprintf(sz, unitext("EGT %0.1f°C"), m_pointda[m_pgroup.m_iTimeMouse].y);

            }
            else if (m_etype == type_rpm)
            {

               sprintf(sz, unitext("%d RPM"), (int)m_pointda[m_pgroup.m_iTimeMouse].y);

            }
            else if (m_etype == type_throttle)
            {

               sprintf(sz, unitext("%0.1f%% Throttle"), m_pointda[m_pgroup.m_iTimeMouse].y);

            }

            if (strlen(sz) > 0)
            {

               pgraphics->set_text_color(ARGB(255, 128, 0, 0));

               rect r = m_rect;
               r.deflate(10, 10);
               pgraphics->draw_text(sz, r, DT_TOPRIGHT);

            }

         }


         //double dMax = -100000.0;


         pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

         //for (index i = 0; i < m_pointda.get_size(); i++)
         //{

         //   if (m_pointda[i].y > dMax)
         //   {

         //      dMax = m_pointda[i].y;

         //   }

         //   if (m_pointda[i].y < dMin)
         //   {

         //      dMin = m_pointda[i].y;

         //   }

         //}

         pgraphics->SelectObject(m_sppen);

         pgraphics->MoveTo((double)m_rect.left, m_rect.bottom - m_rect.height() * (m_pointda[0].y - dMin) / dSpan);


         for (index i = 1; i < m_pointda.get_size(); i+=50)
         {

            pgraphics->LineTo(m_rect.left + m_rect.width() * m_pointda[i].x / dWSpan, m_rect.bottom - m_rect.height() * (m_pointda[i].y - dMin) / dSpan);

         }

         if (iTimeRun >= 0)
         {

            pgraphics->SelectObject(m_sppenRun);

            pgraphics->MoveTo(m_rect.left + m_rect.width() * m_pointda[iTimeRun].x / dWSpan, (double) m_rect.top);

            pgraphics->LineTo(m_rect.left + m_rect.width() * m_pointda[iTimeRun].x / dWSpan, (double)m_rect.bottom);

         }



         if (m_pgroup->m_iTimeMouse >= 0)
         {

            pgraphics->SelectObject(m_sppenMouse);

            pgraphics->MoveTo(m_rect.left + m_rect.width() * m_pointda[m_pgroup.m_iTimeMouse].x / dWSpan, (double)m_rect.top);

            pgraphics->LineTo(m_rect.left + m_rect.width() * m_pointda[m_pgroup.m_iTimeMouse].x / dWSpan, (double)m_rect.bottom);

         }

         ::draw2d::pen_pointer pen(allocer());

         pen->create_solid(1.0, ARGB(155, 255, 255, 255));

         pgraphics->set_text_color(ARGB(255, 255, 255, 255));

         pgraphics->SelectObject(pen);

         pgraphics->selectFont(m_fontScale);

         rect r1;

         r1.bottom = (LONG) (m_rect.bottom - m_rect.height() * (m_dMax - dMin) / dSpan - 1);
         r1.top = m_rect.top;
         r1.right = m_rect.left - 1;
         r1.left = r1.right - 200;

         pgraphics->set_smooth_mode(::draw2d::smooth_mode_none);
         pgraphics->MoveTo((int)m_rect.left-1, (int) (m_rect.bottom - m_rect.height() * (m_dMax - dMin) / dSpan));
         pgraphics->LineTo((int)m_rect.left - 20, (int)(m_rect.bottom - m_rect.height() * (m_dMax - dMin) / dSpan));
         pgraphics->draw_text(m_strMax, r1, DT_RIGHT | DT_BOTTOM);

         rect r2;

         r2.top = (LONG) (m_rect.bottom - m_rect.height() * (m_dMin - dMin) / dSpan + 1);
         r2.bottom = m_rect.bottom;
         r2.right = m_rect.left - 1;
         r2.left = r2.right - 200;

         pgraphics->set_smooth_mode(::draw2d::smooth_mode_none);
         pgraphics->MoveTo((int) m_rect.left-1, (int)(m_rect.bottom - m_rect.height() * (m_dMin - dMin) / dSpan));
         pgraphics->LineTo((int)m_rect.left - 20, (int) (m_rect.bottom - m_rect.height() * (m_dMin - dMin) / dSpan));
         pgraphics->draw_text(m_strMin, r2, DT_RIGHT | DT_TOP);

      }
      else
      {


      }
      pgraphics->DrawRect(m_rect, m_sppenBorder);

   }

} // namespace gauge_prober