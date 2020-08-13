#pragma once


namespace twitter_auto
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTO pane_view :
      virtual public ::userex::pane_tab_view,
      virtual public ::user::form_callback
   {
   public:


      document * m_pdocMain;
      

      pane_view(::aura::application * papp);
	   virtual ~pane_view();


      virtual void on_create_impact(::user::impact_creator_data * pcreatordata) override;

      virtual void install_message_routing(::channel * pchannel);

	   virtual void assert_valid() const;

      virtual void dump(dump_context & dumpcontext) const;

	   DECL_GEN_SIGNAL(_001OnCreate);
      bool on_control_event(::user::form_window * pview,::user::control_event * pevent);
      void on_show_view();


   };


} // namespace twitter_auto













