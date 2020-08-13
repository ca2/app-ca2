#pragma once


namespace gauge_prober
{



   class CLASS_DECL_APP_CA2_GAUGE_PROBER line_chart :
      virtual public ::object
   {
      public:

         enum e_type
         {

            type_egt,
            type_rpm,
            type_throttle,

         };

         group *              m_pgroup;


         e_type               m_etype;
         string               m_strTitle;
         pointd_array         m_pointda;
         rect                 m_rect;
         ::draw2d::font_pointer    m_fontTitle;
         ::draw2d::font_pointer    m_fontScale;
         ::draw2d::pen_pointer     m_sppenRun;
         ::draw2d::pen_pointer     m_sppenMouse;
         ::draw2d::pen_pointer     m_sppen;
         ::draw2d::pen_pointer     m_sppenBorder;
         ::draw2d::brush_pointer   m_spbrushBk;
         double               m_dMin;
         double               m_dMax;
         string               m_strMin;
         string               m_strMax;



         line_chart(::aura::application * papp);
         virtual ~line_chart();


         virtual void create(group * pgroup);

         virtual void on_draw(::draw2d::graphics_pointer & pgraphics);



   };

} // namespace gauge_prober