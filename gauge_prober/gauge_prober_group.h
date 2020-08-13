#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER group :
      virtual public object
   {
   public:


      int                     m_iTimeMouse;

      double                  m_dX;
      e_engine                m_eengine;

      flight *                m_pflight;

      index                   m_iIndex;
      string                  m_strIndex;
      rect                    m_rect;
      ::user::plain_edit *    m_peditBrowse;
      ::user::button *        m_pbuttonBrowse;

      ::user::combo_box *     m_pcomboBaud;
      int                     m_iTime;
      property_set            m_setFile;

      line_chart              m_chartEgt;
      line_chart              m_chartRpm;
      line_chart              m_chartThrottle;
      //memory                  m_memData1;

      string                  m_strError;

      gauge                   m_gaugeRpm;
      gauge                   m_gaugeEgt;
      gauge                   m_gaugeVout;
      gauge                   m_gaugeVbatt;
      gauge                   m_gaugeThrottle;

      throttle                m_throttle;

      double                  m_dEnd;

      memory                  m_mem;

      size                    m_sizeBorder;

      rect                    m_rectMessage;


      group(::aura::application * papp);
      virtual ~group();


      virtual void create(flight * pflight, index iIndex);

      virtual void update_file();

      virtual bool on_control_event(::user::control_event * pevent);

      //virtual void serial_run(string strSerial);

      virtual void on_draw(::draw2d::graphics_pointer & pgraphics);

      virtual void on_mouse_move();

   };


} // namespace gauge_prober



