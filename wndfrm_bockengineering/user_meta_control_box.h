#pragma once


class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING MetaControlBox :
   virtual public ::user::wndfrm::frame::control_box
{
public:


   MetaControlBox(::aura::application * papp);
   virtual ~MetaControlBox();


   virtual void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
   virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

   virtual void install_message_routing(::channel * pchannel);


   DECL_GEN_SIGNAL(_001OnShowWindow);
   DECL_GEN_SIGNAL(_001OnLButtonDown);
   DECL_GEN_SIGNAL(_001OnLButtonUp);
   void _001OnTimer(::timer * ptimer);


};



