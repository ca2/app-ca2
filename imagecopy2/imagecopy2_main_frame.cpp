#include "framework.h"


namespace imagecopy2
{


   main_frame::main_frame(::aura::application * papp) :
      object(papp),
      simple_frame_window(papp),
      m_font(allocer())

   {
      m_font->create_point_font("Tahoma", 12, 400);
      m_bDefaultNotifyIcon = false;

      WfiEnableFullScreen(false);

      m_bWindowFrame = !Application.handler()->has_topic_query("client_only");


      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_notify_icon);
      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_transparent_frame);
      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_dock);

      m_etranslucency = ::user::translucency_present;

      m_bExplicitTranslucency = false;


   }


   main_frame::~main_frame()
   {

   }


   void main_frame::install_message_routing(::channel * pdispatch)
   {

      simple_frame_window::install_message_routing(pdispatch);

      IGUI_MSG_LINK(WM_CREATE, pdispatch, this, &main_frame::_001OnCreate);
      IGUI_MSG_LINK(WM_DESTROY, pdispatch, this, &main_frame::_001OnDestroy);

   }

   void main_frame::_001OnCreate(message::message * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      set_window_text("Image Copy");

   }


   bool main_frame::get_font(::draw2d::font_pointer & sp, ::user::e_font efont, ::user::interaction * pinteraction)
   {

      sp = m_font;

      return true;

   }


   void main_frame::_001OnDestroy(message::message * pobj)
   {

      pobj->previous();

   }

   void main_frame::assert_valid() const
   {

      simple_frame_window::assert_valid();

   }


   void main_frame::dump(dump_context & dumpcontext) const
   {

      simple_frame_window::dump(dumpcontext);

   }


   __pointer(::user::wndfrm::frame::frame) main_frame::experience_get_frame()
   {

      __pointer(::user::wndfrm::frame::frame) pschema = Application.wndfrm()->get_frame_schema(NULL, "013");

      pschema->set_style("LightBlue");

      return pschema;

   }
   bool  main_frame::on_create_bars()
   {

      //if (!LoadToolBar("main", "main_toolbar.xml"))
      //{

      //   return false;

      //}

      //__pointer(simple_toolbar) p = m_toolbarmap["main"];

      //if (p.is_set())
      //{

      //   p->m_sizeSpacing.cx = 8;

      //   p->m_rectItemPad.set(9, 9);

      //   p->m_rectBorder.set(9, 9);

      //}

      return true;

   }

   bool main_frame::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return simple_frame_window::get_color(cr, ecolor, pinteraction);

   }


   bool main_frame::has_pending_graphical_update()
   {

      return IsWindowVisible();

   }


   bool main_frame::get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pinteraction)
   {

      if (m_bExplicitTranslucency)
      {

         etranslucency = m_etranslucency;

         return true;

      }

      //if (m_puserschemaSchema != NULL)
      //{

      //   if (m_puserschemaSchema->_001GetMainFrameTranslucency(etranslucency))
      //      return true;

      //}
      etranslucency = ::user::translucency_present;

      //      etranslucency = ::user::TranslucencyNone;

      return true;

   }


} // namespace imagecopy2







