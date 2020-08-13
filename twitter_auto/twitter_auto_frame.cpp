#include "framework.h"


namespace twitter_auto
{


   frame::frame(::aura::application * papp) :
      object(papp),
      simple_frame_window(papp)
   {


   }


   frame::~frame()
   {

   }


   void frame::assert_valid() const
   {

      simple_frame_window::assert_valid();

   }


   void frame::dump(dump_context & dumpcontext) const
   {

      simple_frame_window::dump(dumpcontext);

   }


   bool frame::get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pinteraction)
   {

      etranslucency = ::user::translucency_present;

      return true;

   }


   bool  frame::on_create_bars()
   {

      if (!LoadToolBar("main", "main_toolbar.xml"))
      {

         return false;

      }

      return true;

   }

   bool frame::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return simple_frame_window::get_color(cr, ecolor, pinteraction);

   }


} // namespace twitter_auto







