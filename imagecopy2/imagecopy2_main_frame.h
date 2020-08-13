#pragma once


namespace imagecopy2
{


   class CLASS_DECL_APP_CA2_IMAGECOPY2 main_frame :
      virtual public simple_frame_window
   {
      public:


         bool                       m_bExplicitTranslucency;
         ::user::e_translucency   m_etranslucency;

         ::draw2d::font_pointer          m_font;
         main_frame(::aura::application * papp);
         virtual ~main_frame();


         virtual void install_message_routing(::channel * pdispatch);

         virtual void assert_valid() const;
         virtual void dump(dump_context & dumpcontext) const;

         virtual bool on_create_bars();
         DECL_GEN_SIGNAL(_001OnCreate);
         DECL_GEN_SIGNAL(_001OnDestroy);

         virtual bool get_font(::draw2d::font_pointer & sp, ::user::e_font efont, ::user::interaction * pinteraction) override;
         virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pinteraction) override;
         virtual __pointer(::user::wndfrm::frame::frame) experience_get_frame();

         virtual bool has_pending_graphical_update();

         virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction) override;
   };


} // namespace imagecopy2












