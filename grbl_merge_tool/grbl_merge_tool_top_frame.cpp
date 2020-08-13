#include "framework.h"


namespace grbl_merge_tool
{


   top_frame::top_frame(::aura::application * papp) :
      object(papp),
      simple_frame_window(papp)
   {

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


   top_frame::~top_frame()
   {

   }


   void top_frame::install_message_routing(::channel * pdispatch)
   {

      simple_frame_window::install_message_routing(pdispatch);

      IGUI_MSG_LINK(WM_CREATE, pdispatch, this, &top_frame::_001OnCreate);
      IGUI_MSG_LINK(WM_DESTROY, pdispatch, this, &top_frame::_001OnDestroy);
      IGUI_MSG_LINK(WM_ACTIVATE, pdispatch, this, &top_frame::_001OnActivate);

   }



   void top_frame::_001OnCreate(message::message * pobj)
   {

      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      set_window_text("MDW Serial Plate");

   }


   void top_frame::_001OnDestroy(message::message * pobj)
   {

      pobj->previous();

   }

   void top_frame::_001OnActivate(message::message * pobj)
   {
      SCAST_PTR(::message::activate, pactivate, pobj);
      pobj->previous();

      if (pactivate->m_nState == WA_ACTIVE)
      {


         if (Context.dir().is(Application.m_filepath))
         {

            Application.m_pedit->SetFocus();

         }
      }

   }

   void top_frame::assert_valid() const
   {

      simple_frame_window::assert_valid();

   }


   void top_frame::dump(dump_context & dumpcontext) const
   {

      simple_frame_window::dump(dumpcontext);

   }


   __pointer(::user::wndfrm::frame::frame) top_frame::experience_get_frame()
   {

      __pointer(::user::wndfrm::frame::frame) pschema = Application.wndfrm()->get_frame_schema(NULL, "013");

      pschema->set_style("LightBlue");

      return pschema;

   }


   bool  top_frame::on_create_bars()
   {


      return true;

   }


   bool top_frame::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return simple_frame_window::get_color(cr, ecolor, pinteraction);

   }


   bool top_frame::has_pending_graphical_update()
   {

      return IsWindowVisible();

   }


   bool top_frame::get_translucency(::user::e_translucency & etranslucency, ::user::e_element eelement, ::user::interaction * pinteraction)
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


} // namespace grbl_merge_tool







