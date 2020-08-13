#include "framework.h"


namespace gauge_prober
{


   main_frame::main_frame(::aura::application * papp) :
      object(papp),
      simple_frame_window(papp),
      m_font(allocer())

   {
      m_font->create_pixel_font("Verdana", 30, 800);
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


   void main_frame::install_message_routing(::channel * pchannel)
   {

      simple_frame_window::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &main_frame::_001OnCreate);
      IGUI_MSG_LINK(WM_DESTROY, pchannel, this, &main_frame::_001OnDestroy);

   }

   void main_frame::_001OnCreate(::message::message * pmessage)
   {

      SCAST_PTR(::message::create, pcreate, pmessage);

      pcreate->previous();

      set_window_text("Gauge Prober");

   }


   bool main_frame::get_font(::draw2d::font_pointer & sp, ::user::e_font efont, ::user::interaction * pinteraction)
   {

      sp = m_font;

      return true;

   }

   void main_frame::_001OnDestroy(::message::message * pmessage)
   {

      pmessage->previous();

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

      pschema->set_style("Rouge");

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

   void main_frame::_000OnDraw(::draw2d::graphics_pointer & pgraphics)
   {
      defer_check_layout();

      defer_check_zorder();

      if (!IsWindowVisible() || WfiIsIconic())
         return;

      rect rectClient;

      GetWindowRect(rectClient);

      rectClient -= rectClient.top_left();

      bool bDib = false;

      double dAlpha = get_alpha();

      ::draw2d::graphics_pointer & pgraphics = pgraphicsParam;

      if (dAlpha == 0.0)
      {

         output_debug_string("Alpha is Zero\n");

      }

      if (rectClient.area() > 0 && dAlpha > 0.0 && dAlpha < 1.0 && GetExStyle() & WS_EX_LAYERED)
      {

         if (m_dibAlpha.is_null())
         {

            m_dibAlpha.alloc(allocer());

         }

         m_dibAlpha->create(rectClient.size());

         m_dibAlpha->Fill(0, 0, 0, 0);

         pgraphics = m_dibAlpha->get_graphics();

         pgraphics->SetViewportOrg(pgraphics->GetViewportOrg());

         bDib = true;

      }

      if (dAlpha > 0.0)
      {

         {

#if TEST

            pgraphics->FillSolidRect(60, 10, 50, 50, ARGB(128, 184, 180, 90));

#endif

            _001DrawThis(pgraphics);

            _001DrawChildren(pgraphics);

            _008CallOnDraw(pgraphics);

#if TEST

            pgraphics->FillSolidRect(10, 60, 50, 50, ARGB(128, 184, 180, 90));

#endif

         }

      }

      if (bDib)
      {

         pgraphicsParam->set_alpha_mode(::draw2d::alpha_mode_blend);

         pgraphicsParam->alpha_blend(null_point(), rectClient.size(), pgraphics, null_point(), dAlpha);

      }

   }


} // namespace gauge_prober







