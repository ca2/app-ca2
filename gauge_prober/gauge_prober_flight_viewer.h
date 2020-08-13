#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER flight_viewer :
      virtual public flight
   {
      public:




         flight_viewer(::aura::application * papp);
         virtual ~flight_viewer();


         virtual void assert_valid() const;

         virtual void dump(dump_context & dumpcontext) const;


         virtual void install_message_routing(::channel * pchannel) override;


         void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

         DECL_GEN_SIGNAL(_001OnCreate);


         virtual void on_layout() override;

         virtual void update_file();

         virtual bool on_control_event(::user::control_event * pevent) override;

         virtual void start();

         virtual void stop();

   };


} // namespace gauge_prober




