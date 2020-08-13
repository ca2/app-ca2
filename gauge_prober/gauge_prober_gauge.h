#pragma once


namespace gauge_prober
{



   class CLASS_DECL_APP_CA2_GAUGE_PROBER gauge :
      virtual public ::object
   {
      public:

         enum e_type
         {

            type_egt,
            type_rpm,
            type_throttle,
            type_volt,
         };

         byte                 m_b;
         bool                 m_bControl;
         bool                 m_bDigital;
         bool                 m_bMilli;
         bool                 m_bKilo;
         group *              m_pgroup;

         point                m_ptCenter;

         ::visual::dib_pointer     m_dibRim;
         e_type               m_etype;
         string               m_strTitle1;
         string               m_strTitle;
         pointd_array         m_pointda;
         rect                 m_rect;
         ::draw2d::font_pointer    m_fontTitle1;
         ::draw2d::font_pointer    m_fontTitle;
         ::draw2d::font_pointer    m_fontInternal;
         ::draw2d::font_pointer    m_fontScale;
         ::draw2d::pen_pointer     m_sppen;
         ::draw2d::pen_pointer     m_sppenBorder;
         ::draw2d::brush_pointer   m_spbrushBk;
         ::draw2d::brush_pointer   m_spbrushDotMarker;
         ::draw2d::brush_pointer   m_spbrushAxis;
         ::draw2d::brush_pointer   m_spbrushAxisHighlight;
         ::draw2d::brush_pointer   m_spbrushAxisShadow;
         ::draw2d::brush_pointer   m_spbrushHand;
         double               m_dMin;
         double               m_dMax;
         double               m_dScaleMin;
         double               m_dScaleMax;
         int                  m_iCount;

         ::draw2d::pen_pointer     m_sppenMarker;

         gauge(::aura::application * papp);
         virtual ~gauge();


         virtual void create(group * pgroup);

         virtual void on_draw(::draw2d::graphics_pointer & pgraphics);

         virtual void on_draw_analog(::draw2d::graphics_pointer & pgraphics);

         virtual void on_draw_digital(::draw2d::graphics_pointer & pgraphics);

         virtual void draw_marker(::draw2d::graphics_pointer & pgraphics, double d); // °
         virtual void draw_dot_marker(::draw2d::graphics_pointer & pgraphics, double d); // °
         virtual void draw_label(::draw2d::graphics_pointer & pgraphics, double d, string strText, int iSize); // °
         virtual void draw_hand(::draw2d::graphics_pointer & pgraphics, double d);

         virtual void rate(double d);

   };

} // namespace gauge_prober