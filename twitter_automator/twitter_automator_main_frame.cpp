#include "framework.h"


namespace twitter_automator
{


   main_frame::main_frame(::aura::application * papp) :
      object(papp),
      simple_frame_window(papp)
   {

      m_bDefaultNotifyIcon = true;

      WfiEnableFullScreen(false);

      m_bWindowFrame = !Application.handler()->has_topic_query("client_only");


      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_notify_icon);
      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_transparent_frame);
      m_workset.m_ebuttonaHide.add(::user::wndfrm::frame::button_dock);

      if (Application.handler()->has_topic_query("opaque"))
      {

         m_bExplicitTranslucency = true;

         m_etranslucency = ::user::translucency_none;

      }
      else
      {

         m_bExplicitTranslucency = false;

      }



   }


   main_frame::~main_frame()
   {

   }


   void main_frame::install_message_routing(::channel * pdispatch)
   {

      simple_frame_window::install_message_routing(pdispatch);

      IGUI_MSG_LINK(WM_CREATE, pdispatch, this, &main_frame::_001OnCreate);

   }

   void main_frame::_001OnCreate(message::message * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      set_window_text("Twitter Automator");

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

      pschema->set_style("LightGreen");

      return pschema;

   }


   bool main_frame::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::style_context * pcontext)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return simple_frame_window::get_color(cr, ecolor, pcontext);

   }

   bool main_frame::has_pending_graphical_update()
   {

      return IsWindowVisible();

   }


   bool main_frame::get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::style_context * pcontext)
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


} // namespace twitter_automator







