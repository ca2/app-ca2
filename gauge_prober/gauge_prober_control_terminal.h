#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER control_terminal :
      virtual public flight
   {
      public:


         ::draw2d::font_pointer       m_fontTime;

         ::user::button *        m_pbuttonRun;
         ::user::button *        m_pbuttonStop;



         control_terminal(::aura::application * papp);
         virtual ~control_terminal();


         virtual void assert_valid() const;

         virtual void dump(dump_context & dumpcontext) const;


         virtual void install_message_routing(::channel * pchannel) override;


         void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

         DECL_GEN_SIGNAL(_001OnCreate);
         DECL_GEN_SIGNAL(_001OnLButtonDown);
         DECL_GEN_SIGNAL(_001OnMouseMove);
         DECL_GEN_SIGNAL(_001OnLButtonUp);


         virtual void on_layout() override;

         virtual void update_file();

         virtual bool on_control_event(::user::control_event * pevent) override;

         virtual void start();

         virtual void stop();

   };


} // namespace gauge_prober




