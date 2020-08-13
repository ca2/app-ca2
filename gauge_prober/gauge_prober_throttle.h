#pragma once


namespace gauge_prober
{



   class CLASS_DECL_APP_CA2_GAUGE_PROBER throttle :
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

         bool                 m_bDigital;
         bool                 m_bMilli;
         bool                 m_bKilo;
         group *              m_pgroup;

         point                m_ptCenter;

         ::visual::dib_pointer     m_dibButton;
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
         ::draw2d::pen_pointer     m_sppenShadow1;
         ::draw2d::pen_pointer     m_sppenShadow2;
         ::draw2d::pen_pointer     m_sppenShadow3;
         ::draw2d::pen_pointer     m_sppenShadow4;
         ::draw2d::brush_pointer   m_spbrushHand;
         double               m_dMin;
         double               m_dMax;
         double               m_dScaleMin;
         double               m_dScaleMax;
         int                  m_iCount;
         double               m_d;
         ::draw2d::pen_pointer     m_sppenMarker;
         bool                 m_bDrag;
         double               m_dMarkerBeg;
         double               m_dMarkerEnd;
         double               m_dMarkerSize;


         throttle(::aura::application * papp);
         virtual ~throttle();


         virtual void on_marker(int y);


         virtual void create(group * pgroup);

         virtual void on_draw(::draw2d::graphics_pointer & pgraphics);

         virtual void draw_marker(::draw2d::graphics_pointer & pgraphics, double d);

         virtual void on_mouse_down(const ::point & point);

         virtual void on_mouse_move(const ::point & point);

         virtual void on_mouse_up(const ::point & point);

   };

} // namespace gauge_prober