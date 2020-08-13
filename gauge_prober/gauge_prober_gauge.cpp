#include "framework.h"
#include <stdio.h>


namespace gauge_prober
{

   gauge::gauge(::aura::application * papp) :
      ::object(papp),
      m_dibRim(allocer()),
      m_sppen(allocer()),
      m_sppenBorder(allocer()),
      m_sppenMarker(allocer()),
      m_spbrushBk(allocer()),
      m_spbrushDotMarker(allocer()),
      m_fontTitle(allocer()),
      m_fontInternal(allocer()),
      m_fontScale(allocer()),
      m_spbrushAxis(allocer()),
      m_spbrushAxisHighlight(allocer()),
      m_spbrushAxisShadow(allocer()),
      m_spbrushHand(allocer()),
      m_fontTitle1(allocer())
   {

      m_bControl = false;

      m_pgroup = NULL;

      m_strTitle = "Title";

      defer_create_mutex();

      m_bMilli = true;
      m_bKilo = true;

      m_sppen->create_solid(2.0, ARGB(255, 255, 0, 0));
      m_sppenBorder->create_solid(5.0, ARGB(255, 0, 0, 0));
      m_sppenMarker->create_solid(1.0, ARGB(255, 255, 255, 255));
      m_spbrushBk->create_solid(ARGB(255, 60, 60, 60));
      m_spbrushDotMarker->create_solid(ARGB(255, 255, 255, 255));


      m_spbrushAxisShadow->create_solid(ARGB(255, 11, 11, 11));
      m_spbrushAxis->create_solid(ARGB(255, 80, 80, 80));
      m_spbrushAxisHighlight->create_solid(ARGB(255, 140, 140, 140));
      m_spbrushHand->create_solid(ARGB(255, 255, 0, 0));

      m_fontTitle->create_pixel_font("Verdana", 36.0, FW_BOLD);
      m_fontTitle1->create_pixel_font("Verdana", 42.0, FW_BOLD);
      m_fontInternal->create_pixel_font("Verdana", 18.0, FW_BOLD);
      m_fontScale->create_pixel_font("Verdana", 12.0);

      m_dibRim.load_from_matter("base.png");

      m_bDigital = false;

   }


   gauge::~gauge()
   {


   }


   void gauge::create(group * pgroup)
   {

      m_pgroup = pgroup;

   }

   void gauge::on_draw(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_pgroup == NULL || m_pgroup->m_pflight == NULL)
      {

         return;

      }

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      if (m_bDigital)
      {

         on_draw_digital(pgraphics);

      }
      else
      {

         on_draw_analog(pgraphics);

      }


   }

   void gauge::on_draw_digital(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_pgroup == NULL || m_pgroup->m_pflight == NULL)
      {

         return;

      }


      rect rDisplay;

      rDisplay = m_rect;

      //rDisplay.deflate(rDisplay.width() / 6, 0);

      rDisplay.top += rDisplay.height()  / 2;

      rect rLabel;

      rLabel = m_rect;

      rLabel.bottom = rDisplay.top;

      synch_lock sl(mutex());

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillRect(rDisplay, m_spbrushBk);

      pgraphics->set_text_color(ARGB(205, 255, 255, 255));

      pgraphics->selectFont(m_fontTitle);

      index iTime = (index)((((double)get_tick_count() - m_pgroup->m_pflight->m_dStart) / (double)(1000.0 / m_pgroup->m_pflight->m_iTimeDivisor)));

      bool bEnd = false;

      if (iTime > m_pointda.get_size() || (m_pgroup->m_pflight.m_dNow == 0.0 && !m_pgroup.m_pflight.m_bStart))
      {


         bEnd = true;

         iTime = m_pointda.get_upper_bound();

      }
      else if (!m_pgroup->m_pflight->m_bStart && m_pgroup->m_pflight->m_dNow > 0)
      {

         iTime = (int)m_pgroup->m_pflight->m_dNow;

      }
      else if (iTime < 0)
      {

         iTime = 0;

      }

      m_pgroup->m_iTime = iTime;

      pgraphics->draw_text(m_strTitle1, rLabel, DT_HVCENTER);

      pgraphics->set_text_color(ARGB(255, 255, 0, 0));

      m_fontTitle1->m_dFontSize = 54.0;

      m_fontTitle1->m_bUpdated = false;

      pgraphics->selectFont(m_fontTitle1);

      if ((bEnd || m_pointda.is_empty()) && !m_bControl)
         //if (m_pointda.is_empty() && !m_bControl)
      {

         pgraphics->draw_text(m_strTitle, rDisplay, DT_HVCENTER);

      }
      if (m_pointda.has_elements())
      {

         char sz[256];
         ZERO(sz);
         if (bEnd && !m_bControl)
            // if (!m_bControl)
         {
         }
         else if (m_etype == type_egt)
         {

            sprintf(sz, unitext("%0.1f°C"), m_pointda[iTime].y);

         }
         else if (m_etype == type_rpm)
         {

            sprintf(sz, unitext("RPM"), (int)m_pointda[iTime].y);

         }
         else if (m_etype == type_throttle)
         {

            sprintf(sz, unitext("%0.1f%%"), m_pointda[iTime].y);

         }

         if (strlen(sz) > 0)
         {

            pgraphics->draw_text(sz, rDisplay, DT_HVCENTER);

         }

         //double dMax = -100000.0;

         //double dMin = 100000.0;

         double dMin = m_dMin;

         double dMax = m_dMax;

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



      }
      pgraphics->DrawRect(rDisplay, m_sppenBorder);


   }


   void gauge::on_draw_analog(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_pgroup == NULL || m_pgroup->m_pflight == NULL)
      {

         return;

      }

      synch_lock sl(mutex());

      rect rectGauge(m_rect);

      rectGauge.bottom = rectGauge.top + m_rect.width();


      m_ptCenter = rectGauge.center();

      size szCenter(m_rect.width() / 55, 0);

      m_ptCenter.offset(szCenter);

      pgraphics->StretchBlt(rectGauge.left, rectGauge.top, rectGauge.width(), rectGauge.height(),
                            m_dibRim->get_graphics(),
                            0, 0, m_dibRim->m_size.cx, m_dibRim.m_size.cy, SRCCOPY);

      {

         rect rTitle1;

         rTitle1 = m_rect;

         rTitle1.top = m_rect.top + m_rect.height() / 4;
         rTitle1.bottom = m_rect.top + m_rect.height() / 2;

         rTitle1.offset(szCenter);

         pgraphics->set_text_color(ARGB(185, 255, 0, 0));

         m_fontInternal->m_dFontSize = 20.0 * m_rect.width() / 200.0;

         m_fontInternal->m_bUpdated = false;

         pgraphics->selectFont(m_fontInternal);

         pgraphics->draw_text(m_strTitle1, rTitle1, DT_HVCENTER);

      }
      double dAxis = m_rect.width() / 20;

      rect rAxis(
      (i64) (m_ptCenter.x - dAxis),
      (i64)(m_ptCenter.y - dAxis),
      (i64)(m_ptCenter.x + dAxis),
      (i64)(m_ptCenter.y + dAxis));

      rect rAxisShadow(rAxis);

      rAxisShadow.offset(m_rect.width() / 175, m_rect.height() / 175);

      pgraphics->SelectObject(m_spbrushAxisShadow);

      pgraphics->FillEllipse(rAxisShadow);

      rect rAxisHighlight(rAxis);

      rAxisHighlight.offset(-m_rect.width() / 175, -m_rect.height() / 175);

      pgraphics->SelectObject(m_spbrushAxisHighlight);

      pgraphics->FillEllipse(rAxisHighlight);


      double d = m_dScaleMin;

      double dStep = (m_dScaleMax - m_dScaleMin) / (m_iCount);

      stringa straLabel;

      straLabel.set_size(m_iCount + 1);

      int iSize = 0;

      m_fontScale->m_dFontSize = m_rect.width() / 30;

      m_fontScale->m_bUpdated = false;

      pgraphics->selectFont(m_fontScale);

      for (int i = 0; i <= m_iCount; i++, d += dStep)
      {

         if (d == 0.0)
         {

            straLabel[i] = "0";

         }
         else if (m_bKilo && fabs(d) > 1000.0)
         {

            straLabel[i].Format("%dk", (int) (d / 1000.0));

         }
         else
         {

            straLabel[i].Format("%d", (int)(d));

         }

         sized s = pgraphics->GetTextExtent(straLabel[i]);

         iSize = (int)(MAX(MAX(s.cx, s.cy), iSize));

      }

      iSize = (int) (iSize * 1.2);

      d = m_dScaleMin;

      dStep = (m_dScaleMax - m_dScaleMin) / (m_iCount * 5);

      double dAngle;
      for (int i = 0; i <= m_iCount * 5; i++, d+= dStep)
      {
         dAngle = 180.0 - (270.0 *(d- m_dScaleMin)/ (m_dScaleMax - m_dScaleMin));

         if (i % 5 == 0)
         {

            draw_marker(pgraphics, dAngle);

            draw_label(pgraphics, dAngle, straLabel[i / 5], iSize);

         }
         else
         {
            draw_dot_marker(pgraphics, dAngle);

         }

      }


      //pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      //pgraphics->FillRect(m_rect, m_spbrushBk);

      pgraphics->set_text_color(ARGB(205, 255, 255, 255));

      pgraphics->selectFont(m_fontTitle);

      index iTime = (index)((((double)get_tick_count() - m_pgroup->m_pflight->m_dStart) / (double)(1000.0 / m_pgroup->m_pflight->m_iTimeDivisor)));

      bool bEnd = false;





      if (iTime > m_pointda.get_size() || (m_pgroup->m_pflight.m_dNow == 0.0 && !m_pgroup.m_pflight.m_bStart))
      {


         bEnd = true;

         iTime = m_pointda.get_upper_bound();

      }
      else if (!m_pgroup->m_pflight->m_bStart && m_pgroup->m_pflight->m_dNow > 0)
      {

         iTime = (int)m_pgroup->m_pflight->m_dNow;

      }
      else if (iTime < 0)
      {

         iTime = 0;

      }
      int iH = (int)(m_fontTitle->m_dFontSize * 0.4);

      rect rText;

      rText = m_rect;

      rText.top = rectGauge.bottom + iH;
      rText.bottom = (LONG) (m_rect.top + m_fontTitle.m_dFontSize * 1.5);

      if (m_pointda.is_empty())
      {

         pgraphics->draw_text(m_strTitle, rText, DT_TOPCENTER);

      }

      if (m_pointda.has_elements())
      {

         dAngle = 180.0 - (270.0 *(m_pointda[iTime].y - m_dScaleMin) / (m_dScaleMax - m_dScaleMin));

      }
      else
      {

         dAngle = 180.0;

      }

      draw_hand(pgraphics, dAngle);

      pgraphics->SelectObject(m_spbrushAxis);

      pgraphics->FillEllipse(rAxis);

      if (m_pointda.has_elements())
      {

         char sz[256];
         ZERO(sz);
         if (m_etype == type_egt)
         {

            sprintf(sz, unitext("EGT %0.2f°C"), m_pointda[iTime].y);

         }
         else if (m_etype == type_rpm)
         {

            sprintf(sz, unitext("%d RPM"), (int)m_pointda[iTime].y);

         }
         else if (m_etype == type_throttle)
         {

            sprintf(sz, unitext("%0.1f%% Throttle"), m_pointda[iTime].y);

         }
         else if (m_etype == type_volt)
         {

            sprintf(sz, unitext("%0.2fV"), m_pointda[iTime].y);

         }

         if (strlen(sz) > 0)
         {

            pgraphics->set_text_color(ARGB(205, 255, 255, 255));

            pgraphics->draw_text(sz, rText, DT_TOPCENTER);

         }

         //double dMax = -100000.0;

         //double dMin = 100000.0;

         double dMin = m_dMin;

         double dMax = m_dMax;

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

         //double dSpan = dMax - dMin;

         //dSpan *= 1.2;

         //dMin -= dSpan * 0.1;

         ////double dWSpan = m_pointda.last().x - m_pointda.first().x;

         //double dWSpan = m_pgroup->m_pflight->m_dEnd;

         //pgraphics->SelectObject(m_sppen);

         //pgraphics->MoveTo((double)m_rect.left, m_rect.bottom - m_rect.height() * (m_pointda[0].y - dMin) / dSpan);


         //for (index i = 1; i < iTime; i++)
         //{

         //   pgraphics->LineTo(m_rect.left + m_rect.width() * m_pointda[i].x / dWSpan, m_rect.bottom - m_rect.height() * (m_pointda[i].y - dMin) / dSpan);

         //}

         //::draw2d::pen_pointer pen(allocer());

         //pen->create_solid(1.0, ARGB(155, 255, 255, 255));

         //pgraphics->SelectObject(pen);

         //pgraphics->selectFont(m_fontScale);

         //rect r1;

         //r1.bottom = (LONG)(m_rect.bottom - m_rect.height() * (m_dMax - dMin) / dSpan - 1);
         //r1.top = m_rect.top;
         //r1.right = m_rect.left - 1;
         //r1.left = r1.right - 200;

         //pgraphics->set_smooth_mode(::draw2d::smooth_mode_none);
         //pgraphics->MoveTo((int)m_rect.left - 1, (int)(m_rect.bottom - m_rect.height() * (m_dMax - dMin) / dSpan));
         //pgraphics->LineTo((int)m_rect.left - 20, (int)(m_rect.bottom - m_rect.height() * (m_dMax - dMin) / dSpan));
         //pgraphics->draw_text(m_strMax, r1, DT_RIGHT | DT_BOTTOM);

         //rect r2;

         //r2.top = (LONG)(m_rect.bottom - m_rect.height() * (m_dMin - dMin) / dSpan + 1);
         //r2.bottom = m_rect.bottom;
         //r2.right = m_rect.left - 1;
         //r2.left = r2.right - 200;

         //pgraphics->set_smooth_mode(::draw2d::smooth_mode_none);
         //pgraphics->MoveTo((int)m_rect.left - 1, (int)(m_rect.bottom - m_rect.height() * (m_dMin - dMin) / dSpan));
         //pgraphics->LineTo((int)m_rect.left - 20, (int)(m_rect.bottom - m_rect.height() * (m_dMin - dMin) / dSpan));
         //pgraphics->draw_text(m_strMin, r2, DT_RIGHT | DT_TOP);

      }
      else
      {


      }
      //pgraphics->DrawRect(m_rect, m_sppenBorder);

   }

   void gauge::draw_marker(::draw2d::graphics_pointer & pgraphics, double d)
   {

      double dR = d * System.math().GetPi() / 180.0;

      point ptCenter = m_ptCenter;

      double rOuter = (m_rect.width() / 2.0) * 0.75;

      double rInner = (m_rect.width() / 2.0) * 0.65;

      point ptOuter;

      ptOuter.x = (LONG) (ptCenter.x + rOuter * cos(dR));
      ptOuter.y = (LONG) (ptCenter.y - rOuter * sin(dR));

      point ptInner;

      ptInner.x = (LONG)(ptCenter.x + rInner * cos(dR));
      ptInner.y = (LONG)(ptCenter.y - rInner * sin(dR));

      pgraphics->SelectObject(m_sppenMarker);

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      pgraphics->MoveTo(ptInner);

      pgraphics->LineTo(ptOuter);

   }

   void gauge::draw_dot_marker(::draw2d::graphics_pointer & pgraphics, double d)
   {

      double dR = d * System.math().GetPi() / 180.0;

      point ptCenter = m_ptCenter;

      double r = (m_rect.width() / 2.0) * 0.75;

      ::point point;

      point.x = (LONG)(ptCenter.x + r * cos(dR));
      point.y = (LONG)(ptCenter.y - r * sin(dR));

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      pgraphics->SelectObject(m_spbrushDotMarker);

      pgraphics->Ellipse(point.x - 1, point.y - 1, point.x + 1, point.y + 1);

   }


   void gauge::draw_label(::draw2d::graphics_pointer & pgraphics, double d, string strLabel, int iSize)
   {

      double dR = d * System.math().GetPi() / 180.0;

      point ptCenter = m_ptCenter;

      double r = (m_rect.width() / 2.0) * 0.60;

      ::point point;

      point.x = (LONG)(ptCenter.x + r * cos(dR));
      point.y = (LONG)(ptCenter.y - r * sin(dR));

      rect rc;

      rc.left = point.x - iSize / 2;
      rc.top = point.y - iSize / 2;
      rc.right = point.x + iSize / 2;
      rc.bottom = point.y + iSize / 2;

      pgraphics->set_text_color(ARGB(255, 255, 255, 255));

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      pgraphics->draw_text(strLabel, rc, DT_HVCENTER);


   }

   void gauge::draw_hand(::draw2d::graphics_pointer & pgraphics, double d)
   {

      pointd pa[3];

      double dR = d * System.math().GetPi() / 180.0;

      double d90 = 90.0 * System.math().GetPi() / 180.0;

      point ptCenter = m_ptCenter;

      double rOuter = (m_rect.width() / 2.0) * 0.75;

      double rInner = m_rect.width() / 40;

      pa[0].x = (LONG)(ptCenter.x + rInner * cos(dR - d90));
      pa[0].y = (LONG)(ptCenter.y - rInner * sin(dR - d90));

      pa[1].x = (LONG)(ptCenter.x + rOuter * cos(dR));
      pa[1].y = (LONG)(ptCenter.y - rOuter * sin(dR));

      pa[2].x = (LONG)(ptCenter.x + rInner * cos(dR + d90));
      pa[2].y = (LONG)(ptCenter.y - rInner * sin(dR + d90));

      pgraphics->SelectObject(m_spbrushHand);

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      pgraphics->fill_polygon(pa, 3);

   }


   void gauge::rate(double d)
   {

      m_pointda.set_size(1);

      m_pgroup->m_iTime = 0;

      m_pointda[0].x = 0;

      m_b = (byte) (d * 254.0);

      if (m_etype == type_throttle)
      {

         m_pointda[0].y = m_b /2.0;

      }
      else if (m_etype == type_rpm)
      {

         m_pointda[0].y = m_b * 500.0;

      }
      else if (m_etype == type_egt)
      {

         m_pointda[0].y = m_b * 4.6 - 50.0;

      }
      else if (m_etype == type_volt)
      {

         m_pointda[0].y = 7.0 + (m_b * 9.03/ 255.0);

      }

   }


} // namespace gauge_prober