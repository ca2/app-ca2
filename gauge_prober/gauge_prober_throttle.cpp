#include "framework.h"
#include <stdio.h>


namespace gauge_prober
{

   throttle::throttle(::aura::application * papp) :
      ::object(papp),
      m_dibButton(allocer()),
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
      m_sppenShadow1(allocer()),
      m_sppenShadow2(allocer()),
      m_sppenShadow3(allocer()),
      m_sppenShadow4(allocer()),
      m_spbrushHand(allocer()),
      m_fontTitle1(allocer())
   {

      m_bDrag = false;
      m_pgroup = NULL;

      m_strTitle = "Title";

      defer_create_mutex();

      m_bMilli = true;
      m_bKilo = true;

      m_sppen->create_solid(2.0, ARGB(255, 255, 0, 0));
      m_sppenBorder->create_solid(20.0, ARGB(255, 128, 128, 128));
      m_sppenMarker->create_solid(2.0, ARGB(128, 128, 128, 128));
      m_spbrushBk->create_solid(ARGB(255, 70, 70, 70));
      m_spbrushDotMarker->create_solid(ARGB(255, 255, 255, 255));

      m_d = 0.0;

      m_sppenShadow1->create_solid(1.0, ARGB(255, 67, 67, 67));
      m_sppenShadow2->create_solid(1.0, ARGB(255, 65, 65, 65));
      m_sppenShadow3->create_solid(1.0, ARGB(255, 63, 63, 63));
      m_sppenShadow4->create_solid(1.0, ARGB(255, 60, 60, 60));
      m_spbrushAxis->create_solid(ARGB(255, 80, 80, 80));
      m_spbrushAxisHighlight->create_solid(ARGB(255, 140, 140, 140));
      m_spbrushHand->create_solid(ARGB(255, 255, 0, 0));

      m_fontTitle->create_pixel_font("Verdana", 18.0, FW_BOLD);
      m_fontInternal->create_pixel_font("Verdana", 18.0, FW_BOLD);
      m_fontScale->create_pixel_font("Verdana", 12.0);

      m_dibButton.load_from_matter("throttle.png");

      m_bDigital = false;

   }


   throttle::~throttle()
   {


   }


   void throttle::create(group * pgroup)
   {

      m_pgroup = pgroup;

   }

   void throttle::on_draw(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_pgroup == NULL || m_pgroup->m_pflight == NULL)
      {

         return;

      }

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      if (m_pgroup == NULL || m_pgroup->m_pflight == NULL)
      {

         return;

      }


      rect rDisplay;

      rDisplay = m_rect;

      rect rBk(rDisplay);

      rBk.inflate(6, 6);

      synch_lock sl(mutex());

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      pgraphics->FillRect(rBk, m_spbrushBk);

      class ::rect r;

      int iH = m_dibButton->m_size.cy * m_rect.width() / m_dibButton.m_size.cx;

      r.left = m_rect.left;
      r.right = m_rect.right;
      r.top = m_rect.top;
      r.bottom = r.top + iH;

      rect rShadow(rBk);

      rShadow.inflate(1, 1);

      pgraphics->DrawRect(rShadow, m_sppenShadow1);

      rShadow.inflate(1, 1);

      pgraphics->DrawRect(rShadow, m_sppenShadow2);

      rShadow.inflate(1, 1);

      pgraphics->DrawRect(rShadow, m_sppenShadow3);

      rShadow.inflate(1, 1);

      pgraphics->DrawRect(rShadow, m_sppenShadow4);


      class ::rect rBorder;

      rBorder = rShadow;

      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 108, 108, 108), ARGB(255, 230, 230, 230));
      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 120, 120, 120), ARGB(255, 230, 230, 230));
      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 132, 132, 132), ARGB(255, 200, 200, 200));
      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 142, 142, 142), ARGB(255, 180, 180, 180));

      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 150, 150, 150), ARGB(255, 150, 150, 150));
      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 150, 150, 150), ARGB(255, 150, 150, 150));
      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 150, 150, 150), ARGB(255, 150, 150, 150));
      rBorder.inflate(1, 1);

      pgraphics->Draw3dRect(rBorder, ARGB(255, 150, 150, 150), ARGB(255, 150, 150, 150));

      rBorder.inflate(1, 1);
      pgraphics->Draw3dRect(rBorder, ARGB(255, 180, 180, 180), ARGB(255, 142, 142, 142));

      rBorder.inflate(1, 1);
      pgraphics->Draw3dRect(rBorder, ARGB(255, 200, 200, 200), ARGB(255, 132, 132, 132));

      rBorder.inflate(1, 1);
      pgraphics->Draw3dRect(rBorder, ARGB(255, 230, 230, 230), ARGB(255, 120, 120, 120));

      rBorder.inflate(1, 1);
      pgraphics->Draw3dRect(rBorder, ARGB(255, 230, 230, 230), ARGB(255, 108, 108, 108));

      rBorder.inflate(1, 1);
      pgraphics->Draw3dRect(rBorder, ARGB(255, 120, 120, 120), ARGB(255, 108,108, 108));

      rBorder.inflate(1, 1);
      pgraphics->Draw3dRect(rBorder, ARGB(255, 162, 162, 162), ARGB(255, 108, 108, 108));

      m_dMarkerBeg = m_rect.top + iH / 2;
      m_dMarkerEnd = m_rect.bottom - iH / 2;
      m_dMarkerSize = m_dMarkerEnd - m_dMarkerBeg;
      double dMarkerStep = m_dMarkerSize / 20.0;
      double dMarker = m_dMarkerBeg;

      for (int i = 0; i <= 20; i++, dMarker += dMarkerStep)
      {

         draw_marker(pgraphics, dMarker);

      }

      dMarker = (1.0 - m_d) * m_dMarkerSize;

      r.top = (LONG) (m_rect.top + dMarker);
      r.bottom = r.top + iH;

      pgraphics->StretchBlt(r.left, r.top, r.width(), r.height(), m_dibButton->get_graphics(),
                            0, 0, m_dibButton->m_size.cx, m_dibButton.m_size.cy, SRCCOPY);



   }


   void throttle::draw_marker(::draw2d::graphics_pointer & pgraphics, double d)
   {

      int iM = (m_rect.right + m_rect.left) / 2;

      point ptL;

      ptL.x = m_rect.left + m_rect.width() / 10;
      ptL.y = (LONG) d;

      point ptML;

      ptML.x = iM - m_rect.width() / 8;
      ptML.y = ptL.y;

      point ptMR;

      ptMR.x = iM + m_rect.width() / 8;
      ptMR.y = ptL.y;

      point ptR;

      ptR.x = m_rect.right - m_rect.width() / 10;
      ptR.y = ptL.y;

      pgraphics->SelectObject(m_sppenMarker);

      pgraphics->set_smooth_mode(::draw2d::smooth_mode_high);

      pgraphics->MoveTo(ptL);

      pgraphics->LineTo(ptML);

      pgraphics->MoveTo(ptMR);

      pgraphics->LineTo(ptR);

   }

   void throttle::on_marker(int y)
   {

      if ((m_dMarkerEnd - m_dMarkerBeg) <= 0)
      {

         m_d = 0;

         return;

      }

      m_d = (y - m_dMarkerBeg) / (m_dMarkerEnd - m_dMarkerBeg);

      if (m_d < 0.0)
      {

         m_d = 0.0;

      }
      else if (m_d > 1.0)
      {

         m_d = 1.0;

      }

      m_d = 1.0 - m_d;

      m_pgroup->m_gaugeThrottle.rate(m_d);
      m_pgroup->m_gaugeRpm.rate(m_d);
      m_pgroup->m_gaugeEgt.rate(m_d);
      m_pgroup->m_gaugeVbatt.rate(m_d);
      m_pgroup->m_iTime = 0;

   }

   void throttle::on_mouse_down(const ::point & point)
   {

      m_bDrag = true;

      on_marker(point.y);

   }

   void throttle::on_mouse_move(const ::point & point)
   {

      on_marker(point.y);
   }

   void throttle::on_mouse_up(const ::point & point)
   {

      m_bDrag = false;

      on_marker(point.y);
   }


} // namespace gauge_prober