#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER flight :
      virtual public ::user::impact
   {
      public:

         array < __pointer(serial) >    m_serialptra;

         int                     m_iTimeDivisor;

         e_impact                  m_eview;
         group                   m_group1;
         group                   m_group2;

         bool                    m_bStart;
         double                  m_dStart;
         double                  m_dNow;
         double                  m_dEnd;


         ::draw2d::font_pointer       m_fontTime;
         ::draw2d::pen_pointer        m_penTime;

         ::user::button *        m_pbuttonRun;
         ::user::button *        m_pbuttonStop;

         int                     m_iT2;
         int                     m_iB2;
         double                  m_iX;

         bool                    m_bIn;


         flight(::aura::application * papp, e_impact eview);
         virtual ~flight();


         virtual void install_message_routing(::channel * pchannel) override;

         DECL_GEN_SIGNAL(_001OnCreate);
         DECL_GEN_SIGNAL(_001OnMouseMove);
         DECL_GEN_SIGNAL(_001OnMouseLeave);


         virtual void serial_start();

         virtual void serial_stop();



         virtual void update_file();

         void draw_timer(::draw2d::graphics_pointer & pgraphics);

         virtual void on_layout();

         string get_group_error_message(group * pgroup);

   };


} // namespace gauge_prober




