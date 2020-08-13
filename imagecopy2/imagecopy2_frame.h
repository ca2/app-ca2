#pragma once


namespace imagecopy2
{


   class CLASS_DECL_APP_CA2_IMAGECOPY2 frame :
      virtual public simple_frame_window
   {
      public:


         frame(::aura::application * papp);
         virtual ~frame();


         bool  on_create_bars();


         virtual void assert_valid() const;
         virtual void dump(dump_context & dumpcontext) const;


         virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pinteraction) override;


         virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction);

   };


} // namespace imagecopy2












