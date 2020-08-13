#pragma once


namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL top_frame :
      virtual public simple_frame_window
   {
   public:


      bool                          m_bExplicitTranslucency;
      ::user::e_translucency        m_etranslucency;


      top_frame(::aura::application * papp);
      virtual ~top_frame();


      virtual void install_message_routing(::channel * pdispatch);

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      virtual bool on_create_bars();
      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnDestroy);
      DECL_GEN_SIGNAL(_001OnActivate);


      virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pinteraction) override;
      virtual __pointer(::user::wndfrm::frame::frame) experience_get_frame();

      virtual bool has_pending_graphical_update();

      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction) override;


   };


} // namespace grbl_merge_tool












