#pragma once


namespace image_display
{


   class window :

   virtual public ::user::interaction
   {
   public:

      ::draw2d::dib_pointer m_dib;
      int m_cx;
      int m_cy;

      point m_pt;

      window(::aura::application * papp);
      virtual ~window();


      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;


   };


} // namespace image_display



