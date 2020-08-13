#pragma once


namespace user
{


   class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING meta_button :
      virtual public ::user::wndfrm::frame::control_box_button
   {
   public:


      ::draw2d::region_pointer                 m_spregion;
      ::draw2d::pen_pointer                    m_pen;
      ::draw2d::brush_pointer                  m_brush;


      meta_button(::aura::application * papp);
      virtual ~meta_button();


      virtual index hit_test(const ::point & point, ::user::e_element & eelement);

      virtual void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

      virtual void install_message_routing(::channel * pchannel);

      virtual void _001OnClip(::draw2d::graphics_pointer & pgraphics);

      DECL_GEN_SIGNAL(_001OnShowWindow);
      virtual void on_layout();



      virtual bool keyboard_focus_is_focusable();

   };



} // namespace user




