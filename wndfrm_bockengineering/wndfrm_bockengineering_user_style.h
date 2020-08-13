#pragma once


namespace wndfrm_bockengineering
{


   class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING user_style :
      virtual public ::user::style
   {
      public:



         user_style(::aura::application * papp);
         virtual ~user_style();

         virtual i64 add_ref()
         {

            return ::object::add_ref();

         }

         virtual i64 dec_ref()
         {

            return ::object::dec_ref();

         }


         //      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor) override;
         //    virtual bool get_font(::draw2d::font_pointer & font, ::user::e_font efont) override;
         //  virtual bool get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement) override;


         virtual bool on_ui_event(::user::e_event eevent, ::user::e_object eobject, ::user::interaction * pinteraction) override;

         bool _001TabOnDrawSchema01(::draw2d::graphics_pointer & pgraphics, ::user::tab * ptab);
         void _001OnTabPaneDrawTitle(::user::tab_pane & pane, ::user::tab * ptab, ::draw2d::graphics_pointer & pgraphics, LPCRECT lpcrect, ::draw2d::brush_pointer & brushText);
         bool _001OnTabLayout(::user::tab * ptab);

         virtual bool _001DrawToolbarItem(::draw2d::graphics_pointer & pgraphics, i32 iItem, ::user::toolbar * ptoolbar) override;
         virtual void _001DrawSimpleToolbarItem(::draw2d::graphics_pointer & pgraphics, i32 iItem, ::user::toolbar * ptoolbar);
         virtual void _001DrawAnthillToolbarItem(::draw2d::graphics_pointer & pgraphics, i32 iItem, ::user::toolbar * ptoolbar);
         virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction) override;

         virtual bool simple_ui_draw_focus_rect(::user::interaction * pinteraction, ::draw2d::graphics_pointer & pgraphics) override;


   };


} // namespace wndfrm_rootkiller


