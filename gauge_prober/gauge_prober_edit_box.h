#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER edit_box :
      virtual public ::user::plain_edit
   {
      public:


         ::draw2d::pen_pointer                    m_pen;


         edit_box(::aura::application * papp);
         virtual ~edit_box();


         virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

   };



} // namespace gauge_prober




