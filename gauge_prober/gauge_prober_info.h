#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER info :
      virtual public ::user::impact
   {
   public:


      string                  m_strVersion;

      ::draw2d::font_pointer       m_font;

      info(::aura::application * papp);
      virtual ~info();


      DECL_GEN_SIGNAL(_001OnCreate);

      virtual void install_message_routing(::channel * pchannel) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

   };


} // namespace gauge_prober





