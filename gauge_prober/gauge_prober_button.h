#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER button :
      virtual public ::user::button
   {
      public:


         ::draw2d::pen_pointer                    m_pen;


         button(::aura::application * papp);
         virtual ~button();


         virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);


   };


} // namespace gauge_prober




