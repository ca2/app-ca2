#pragma once


namespace twitter_automator
{


   class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR frame :
      virtual public simple_frame_window
   {
   public:


      frame(::aura::application * papp);
      virtual ~frame();


      bool  on_create_bars();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::style_context * pcontext) override;


      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::style_context * pcontext) override;


   };


} // namespace twitter_automator












