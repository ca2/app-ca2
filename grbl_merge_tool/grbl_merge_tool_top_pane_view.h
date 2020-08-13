#pragma once


namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL top_pane_view :
      virtual public ::userex::pane_tab_view,
      virtual public ::user::form_callback
   {
   public:


      ::user::progress  m_progress;

      top_pane_view(::aura::application * papp);
      virtual ~top_pane_view();

      virtual void on_layout();
      virtual void on_show_view() override;

      virtual void install_message_routing(::channel * pchannel);

      virtual void assert_valid() const;

      document * get_document();

      virtual void dump(dump_context & dumpcontext) const;

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnFormSize);
      bool on_control_event(::user::form_window * pview, ::user::control_event * pevent);

      virtual void on_create_impact(::user::impact_creator_data * pdata) override;

      virtual void prepare_form(id id, ::html_document * pdocument) override;

      virtual void update_serial_ports();

   };


} // namespace grbl_merge_tool













