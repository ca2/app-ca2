#pragma once


namespace twitter_automator
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR main_frame :
      virtual public simple_frame_window
   {
   public:


      bool                       m_bExplicitTranslucency;
      ::user::e_translucency     m_etranslucency;


      main_frame(::aura::application * papp);
      virtual ~main_frame();


      virtual void install_message_routing(::channel * pdispatch);

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      DECL_GEN_SIGNAL(_001OnCreate);


      virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::style_context * pcontext) override;
      virtual __pointer(::user::wndfrm::frame::frame) experience_get_frame();

      virtual bool has_pending_graphical_update();

      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::style_context * pcontext) override;


   };


} // namespace twitter_automator












