#include "framework.h" // from "base/user/user.h"
#include "core/user/menu/user_menu_central.h"
// pgraphics->GetTextExtent("->:<-"); // oh no!! omg!! The size is the size of the alien!!
#define MAGIC_PALACE_TAB_SPLT "->:<-"
#define MAGIC_PALACE_TAB_SIZE "-/-"
#define MAGIC_PALACE_TAB_TEXT "/"





namespace wndfrm_bockengineering
{


   user_style::user_style(::aura::application * papp):
      object(papp),
      ::user::style(papp)
   {

      defer_create_user_schema(::user::schema_default);

      userstyle()->m_mapTranslucency[::user::element_none] = ::user::translucency_present;

      create_point_font(::user::font_default,"MS Sans Serif", 20.0, 800);

      userstyle()->m_mapFont[::user::font_default]->m_etextrendering = ::draw2d::text_rendering_anti_alias_grid_fit;

   }


   user_style::~user_style()
   {

   }


   bool user_style::simple_ui_draw_focus_rect(::user::interaction * pinteraction, ::draw2d::graphics_pointer & pgraphics)
   {
      //
      //      if (m_puserstyle != NULL && m_puserstyle != this)
      //      {
      //
      //         if (m_puserstyle->simple_ui_draw_focus_rect(pinteraction, pgraphics))
      //            return true;
      //
      //      }



      bool bError = pinteraction->m_ptooltip.is_set()
                    && pinteraction->m_ptooltip->IsWindowVisible()
                    && pinteraction->get_tooltip()->m_bError;

      rect rectClient;

      pinteraction->::user::interaction::GetClientRect(rectClient);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      i32 iStyle = 1;

      rectClient.left--;
      rectClient.top--;

      if (m_pauraapp != NULL && (Session.get_keyboard_focus() == pinteraction || pinteraction->has_action_hover()))
      {

         ::draw2d::brush_pointer brush(allocer());

         if (pinteraction->has_action_hover() && !pinteraction->has_text_input())
         {

            brush->create_solid(ARGB(184, 230, 180, 180));

            pgraphics->SelectObject(brush);

            pgraphics->FillRectangle(rectClient);

         }

         if (iStyle == 1)
         {

            bool bHover = pinteraction->has_action_hover();

            if (pinteraction->has_action_hover())
            {

               ::draw2d::pen_pointer pen(pgraphics, 1.0, pinteraction->get_action_hover_border_color());

               pgraphics->DrawRect(rectClient, pen);

            }
            else
            {

               //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(255, 108, 149, 255));
               ::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(184, 90, 80, 77));

               pgraphics->DrawRect(rectClient, pen);

            }


            if (Session.get_keyboard_focus() == pinteraction)
            {

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(255, 108, 149, 255));
                  ::draw2d::pen_pointer pen(pgraphics, 1.0, bError ? ARGB(195, 255, 130, 120) : bHover ? ARGB(220, 255, 110, 110) : ARGB(200, 235, 77, 77));

                  pgraphics->DrawRect(rectClient, pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  /*::draw2d::path_pointer pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);*/

                  //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(84, 108, 149, 255));
                  ::draw2d::pen_pointer pen(pgraphics, 1.0, bError ? ARGB(155, 255, 150, 140) : bHover ? ARGB(200, 255, 110, 110) : ARGB(190, 235, 89, 89));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_pointer pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(72, 108, 149, 255));
                  ::draw2d::pen_pointer pen(pgraphics, 1.0, bError ? ARGB(135, 255, 170, 160) : bHover ? ARGB(160, 255, 125, 125) : ARGB(140, 255, 100, 100));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }

               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_pointer pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(60, 108, 149, 255));
                  ::draw2d::pen_pointer pen(pgraphics, 1.0, bError ? ARGB(105, 255, 190, 180) : bHover ? ARGB(120, 255, 140, 140) : ARGB(90, 235, 120, 120));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }


               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_pointer pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(48, 108, 149, 255));
                  ::draw2d::pen_pointer pen(pgraphics, 1.0, bError ? ARGB(75, 255, 210, 200) : bHover ? ARGB(80, 255, 150, 150) : ARGB(40, 235, 140, 140));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }



               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

                  ::draw2d::path_pointer pathRound(allocer());

                  pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

                  pathRound->add_round_rect(rectClient, 1 * 2);

                  pathRound->end_figure(true);

                  //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(36, 108, 149, 255));
                  ::draw2d::pen_pointer pen(pgraphics, 1.0, bError ? ARGB(45, 255, 230, 220) : bHover ? ARGB(40, 255, 160, 160) : ARGB(10, 235, 160, 160));

                  pgraphics->SelectObject(pen);

                  pgraphics->DrawRect(rectClient, pen);

               }




               /*
               rectClient.left--;
               rectClient.right++;
               rectClient.top--;
               rectClient.bottom++;

               {

               ::draw2d::path_pointer pathRound(allocer());

               pathRound->begin_figure(true, ::draw2d::fill_mode_winding);

               pathRound->add_round_rect(rectClient, 1 * 2);

               pathRound->end_figure(true);

               //::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(24, 108, 149, 255));

               ::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(25, 240, 250, 255));

               pgraphics->SelectObject(pen);

               pgraphics->DrawRect(rectClient, pen);

               }
               */

            }

         }
         else
         {

            ::draw2d::pen_pointer pen(pgraphics, 3.0, ARGB(255, 255, 90, 80));

            pgraphics->DrawRect(rectClient, pen);

         }

      }
      else
      {

         ::draw2d::pen_pointer pen(pgraphics, 1.0, ARGB(255, 149, 149, 123));

         pgraphics->DrawRect(rectClient, pen);

      }

      return true;

   }


   bool user_style::_001TabOnDrawSchema01(::draw2d::graphics_pointer & pgraphics,::user::tab * ptab)
   {

      class rect rect;
      class rect rectBorder;
      class rect rectText;
      class rect rectClient;
      class rect rectIcon;
      class rect rectClose;

      class rect r1;
      ptab->GetClientRect(r1);

      pgraphics->FillSolidRect(r1, ARGB(255, 255, 255, 255));


      ptab->get_data()->m_pen->create_solid(1,RGB(32,32,32));

      pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      ::rect rcTab;

      rcTab = ptab->get_data()->m_rectTab;

      ::rect rcTabs(rcTab);

      ::rect rcClient;

      int iPen = (int)  ptab->get_data()->m_penBorderSel->m_dWidth;


      rcClient = ptab->get_data()->m_rectTabClient;

      int iTabHeight = ptab->get_data()->m_iTabHeight;

      int iB = rcClient.top;

      rcTabs.bottom = rcTabs.top + iTabHeight - ptab->get_data()->m_rectBorder.bottom + iPen;

      rcClient.top = rcTabs.bottom;

      COLORREF crbk;

      ptab->style_color(crbk, ::user::color_tab_layout_background);
      pgraphics->FillSolidRect(rcTabs, crbk);


      ptab->style_color(crbk, ::user::color_tab_client_background);
      pgraphics->FillSolidRect(rcClient, crbk);

      ::draw2d::brush_pointer brushText;


      for (int i = 0; i < 2; i++)
      {
         i32 iTab = -1;


         for (i32 iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & pane = ptab->get_data()->m_panea(iPane);

            if (!pane.m_bTabPaneVisible)
               continue;

            iTab++;

            if (!ptab->get_element_rect(iTab, rect, ::user::element_tab))
               continue;

            if (!ptab->get_element_rect(iTab, rectBorder, ::user::element_border))
               continue;

            if (!ptab->get_element_rect(iTab, rectClient, ::user::element_client))
               continue;

            if (ptab->get_data()->m_bVertical)
            {

               if (ptab->get_element_rect(iTab, rectIcon, ::user::element_icon))
               {

                  pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
                  pane.m_dib->bitmap_blend(pgraphics, rectIcon);

               }

               ::draw2d::path_pointer path(allocer());

               if (true)
               {

                  if (ptab->get_data()->m_idaSel.contains(pane.m_id))
                  {

                     //path->start_figure();

                     path->add_line(rectBorder.right, rectBorder.bottom, rectBorder.left + 1, rectBorder.bottom);
                     //path->add_line(rectClient.right, rectBorder.top);
                     path->add_line(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
                     path->add_line(rectClient.left, rectBorder.top);
                     path->add_line(rectBorder.right, rectBorder.top);

                     path->end_figure(false);

                     pane.m_brushFillSel->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 235, 235, 230), ARGB(250, 255, 255, 250));

                     pgraphics->SelectObject(pane.m_brushFillSel);

                     pgraphics->fill_path(path);

                     pgraphics->SelectObject(ptab->get_data()->m_penBorderSel);

                     pgraphics->draw_path(path);

                     pgraphics->set_font(ptab->get_data()->m_font);

                     brushText = ptab->get_data()->m_brushTextSel;

                  }
                  else
                  {

                     //path->start_figure();

                     path->add_line(rectBorder.right, rectBorder.bottom, rectBorder.left + 1, rectBorder.bottom);
                     path->add_line(rectBorder.left, rectBorder.top - (rectBorder.left - rectClient.left));
                     path->add_line(rectText.left, rectBorder.top);
                     path->add_line(rectBorder.right, rectBorder.top);
                     path->add_line(rectBorder.right, rectBorder.bottom);

                     path->end_figure(true);

                     if (iTab == ptab->m_iHover && ptab->m_eelementHover != ::user::element_close_tab_button && ptab->m_eelementHover < ::user::element_split && ptab->m_eelementHover >(::user::element_split + 100))
                     {

                        pane.m_brushFillHover->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 215, 215, 210), ARGB(250, 235, 235, 230));

                        pgraphics->SelectObject(pane.m_brushFillHover);

                        pgraphics->fill_path(path);

                        pgraphics->SelectObject(ptab->get_data()->m_penBorderHover);

                        pgraphics->draw_path(path);

                        pgraphics->set_font(ptab->get_data()->m_fontUnderline);

                        brushText = ptab->get_data()->m_brushTextHover;

                     }
                     else
                     {

                        pane.m_brushFill->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 175, 175, 170), ARGB(250, 195, 195, 190));

                        pgraphics->SelectObject(pane.m_brushFill);

                        pgraphics->fill_path(path);

                        pgraphics->SelectObject(ptab->get_data()->m_penBorder);

                        pgraphics->draw_path(path);

                        pgraphics->set_font(ptab->get_data()->m_font);

                        brushText = ptab->get_data()->m_brushText;

                     }

                  }

               }

            } // Horizontal
            else
            {

               if (ptab->get_element_rect(iTab, rectIcon, ::user::element_icon))
               {

                  pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

                  pane.m_dib->bitmap_blend(pgraphics, rectIcon);

               }
               if (iPane == 0)
               {

                  if (rcTab.left < rectBorder.left)
                  {

                     pgraphics->SelectObject(ptab->get_data()->m_penBorder);

                     //pgraphics->DrawLine(rcTab.left, rectClient.bottom, rectBorder.left, rectClient.bottom);

                  }

               }

               if (true)
               {

                  ::draw2d::path_pointer path(allocer());

                  if (ptab->get_data()->m_idaSel.contains(pane.m_id))
                  {

                     if (i == 0)
                     {

                        continue;

                     }



                     //if (iPane > 0)
                     //{

                     //   path->add_line(rect.left, rectClient.bottom, rectBorder.left, rectClient.bottom);

                     //}

                     path->add_line(rcTabs.left, rectClient.bottom + iPen, rectBorder.left, rectClient.bottom + iPen);


                     path->add_line(rectBorder.left, rectClient.bottom + iPen, rectBorder.left, rectBorder.top);

                     int dSize = (int)(ptab->get_data()->m_font->m_dFontSize * 2);

                     //path->add_line(rectClient.right - dSize / 2 - 1, rectBorder.top);

                     path->add_line(rectBorder.right - 1, rectBorder.top);

                     //path->add_line(rectBorder.right,rectBorder.top + (rectBorder.right - rectClient.right));
                     //path->add_line(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));
                     //::rect rArc1;
                     //rArc1.left = rectBorder.right - dSize - 1;
                     //rArc1.top = rectBorder.top;
                     //rArc1.right = rectBorder.right - 1;
                     //rArc1.bottom = rectBorder.top + dSize;
                     //path->add_arc(rArc1, -90, 90);

                     //                  path->add_line(rectBorder.right - 1, rectBorder.top + dSize);

                     path->add_line(rectBorder.right - 1, rectClient.bottom + iPen);

                     path->add_line(rectBorder.right - 1, rectClient.bottom + iPen, rcTabs.right, rectClient.bottom + iPen);

                     path->end_figure(false);

                     //pane.m_brushFillSel->CreateLinearGradientBrush(rectBorder.top_left(),rectBorder.bottom_left(),ARGB(230,235,235,230),ARGB(250,255,255,250));

                     ptab->style_color(crbk, ::user::color_tab_client_background);
                     //pgraphics->FillSolidRect(rcClient, crbk);

                     point p1;

                     point p2;

                     p1.x = 0;
                     p2.x = 0;
                     p1.y = rectBorder.top;
                     p2.y = rectClient.bottom + 2;

                     //pane.m_brushFillSel->CreateLinearGradientBrush(p1, p2, ARGB(255,255, 255, 255),crbk);
                     pane.m_brushFillSel->create_solid(crbk);

                     pgraphics->SelectObject(pane.m_brushFillSel);

                     pgraphics->fill_path(path);

                     //ptab->get_data()->m_penBorderSel->create_solid(1.0,ARGB(255,0,0,0));

                     pgraphics->SelectObject(ptab->get_data()->m_penBorderSel);

                     pgraphics->draw_path(path);

                     pgraphics->set_font(ptab->get_data()->m_font);

                     brushText = ptab->get_data()->m_brushTextSel;

                  }
                  else
                  {

                     if (i == 1)
                     {

                        continue;

                     }



                     //path->begin_figure(true, ::draw2d::fill_mode_winding);

                     //if (iPane > 0)
                     //{


                     //}

                     //path->add_line(rectBorder.left,rectClient.bottom,rectBorder.left,rectBorder.top);

                     //path->add_line(rectClient.right,rectBorder.top);

                     //path->add_line(rectBorder.right,rectBorder.top + (rectBorder.right - rectClient.right));

                     path->add_line(rectBorder.left, rectClient.bottom + iPen, rectBorder.left, rectBorder.top);

                     int dSize = (int)(ptab->get_data()->m_font->m_dFontSize * 2);

                     //path->add_line(rectClient.right - dSize / 2, rectBorder.top);

                     path->add_line(rectBorder.right - 1, rectBorder.top);

                     //path->add_line(rectBorder.right,rectBorder.top + (rectBorder.right - rectClient.right));
                     //path->add_line(rectBorder.right, rectBorder.top + (rectBorder.right - rectClient.right));
                     /*::rect rArc1;
                     rArc1.left = rectBorder.right - dSize;
                     rArc1.top = rectBorder.top;
                     rArc1.right = rectBorder.right;
                     rArc1.bottom = rectBorder.top + dSize;
                     path->add_arc(rArc1, -90, 90);

                     path->add_line(rectBorder.right - 1, rectClient.bottom);*/

                     path->add_line(rectBorder.right - 1, rectClient.bottom + iPen);

                     path->end_figure(true);

                     if (iTab == ptab->m_iHover && ptab->m_eelementHover != ::user::element_close_tab_button && (ptab->m_eelementHover < ::user::element_split || ptab->m_eelementHover >(::user::element_split + 100)))
                     {

                        pane.m_brushFillHover->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 215, 215, 210), ARGB(250, 235, 235, 230));

                        pgraphics->SelectObject(pane.m_brushFillHover);

                        pgraphics->fill_path(path);

                        pgraphics->SelectObject(ptab->get_data()->m_penBorderHover);

                        pgraphics->draw_path(path);

                        pgraphics->set_font(ptab->get_data()->m_fontUnderline);

                        brushText = ptab->get_data()->m_brushTextHover;

                     }
                     else
                     {

                        pane.m_brushFill->CreateLinearGradientBrush(rectBorder.top_left(), rectBorder.bottom_left(), ARGB(230, 175, 175, 170), ARGB(250, 195, 195, 190));

                        pgraphics->SelectObject(pane.m_brushFill);

                        pgraphics->fill_path(path);

                        pgraphics->SelectObject(ptab->get_data()->m_penBorder);

                        pgraphics->draw_path(path);

                        pgraphics->set_font(ptab->get_data()->m_font);

                        brushText = ptab->get_data()->m_brushText;

                     }

                  }

               }

               if (iPane == ptab->get_data()->m_panea.get_upper_bound())
               {

                  if (rectBorder.right - 1 < rcTab.right)
                  {

                     pgraphics->SelectObject(ptab->get_data()->m_penBorder);

                     //pgraphics->DrawLine(rectBorder.right - 1, rectClient.bottom, rcTab.right, rectClient.bottom);

                  }

               }


            }

            if (true)
            {

               if (ptab->get_element_rect(iTab, rectText, ::user::element_text))
               {

                  _001OnTabPaneDrawTitle(pane, ptab, pgraphics, rectText, brushText);

               }

            }

            if (true)
            {

               if (ptab->get_element_rect(iTab, rectClose, ::user::element_close_tab_button))
               {

                  pgraphics->set_font(ptab->get_data()->m_fontBold);

                  if (iTab == ptab->m_iHover && ptab->m_eelementHover == ::user::element_close_tab_button)
                  {

                     brushText = ptab->get_data()->m_brushCloseHover;

                  }
                  else
                  {

                     brushText = ptab->get_data()->m_brushClose;

                  }

                  pgraphics->SelectObject(brushText);

                  pgraphics->draw_text("x", rectClose, DT_CENTER | DT_VCENTER);

               }

            }

         }

      }

      return true;

   }


   void user_style::_001OnTabPaneDrawTitle(::user::tab_pane & pane,::user::tab * ptab,::draw2d::graphics_pointer & pgraphics,LPCRECT lpcrect,::draw2d::brush_pointer & brushText)
   {

      stringa & straTitle = pane.m_straTitle;

      pgraphics->SelectObject(brushText);
      ::rect rectText(lpcrect);


      ::draw2d::font_pointer font;
      font = pgraphics->get_current_font();
      rectText.offset((int) (font->m_dFontSize * 0.15), -(int) (font->m_dFontSize * 0.3));

      if(straTitle.get_count() <= 1)
      {

         pgraphics->_DrawText(pane.get_title(), rectText, e_align_bottom_left, e_draw_text_no_prefix);

      }
      else
      {

         size sSep = ptab->get_data()->m_sizeSep;
         ::rect rectEmp;
         for(index i = 0; i < straTitle.get_size(); i++)
         {
            string str = straTitle[i];
            size s = pane.m_sizeaText[i];
            rectText.right =rectText.left + s.cx;
            pgraphics->_DrawText(str,rectText, e_align_bottom_left, e_draw_text_no_prefix);
            rectText.left += s.cx;
            if(i < straTitle.get_upper_bound())
            {
               rectText.right = rectText.left + sSep.cx;
               rectEmp = rectText;
               rectEmp.deflate(1,1);
               ::draw2d::e_alpha_mode emode = pgraphics->m_ealphamode;
               pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
               if(ptab->m_eelementHover == (int)::user::element_split + i)
               {
                  pgraphics->FillSolidRect(rectEmp,ARGB(128,149,184,255));
                  pgraphics->SelectObject(ptab->get_data()->m_brushTextHover);
               }
               else
               {
                  //pgraphics->FillSolidRect(rectEmp,ARGB(128,208,223,233));
                  pgraphics->SelectObject(ptab->get_data()->m_brushText);
               }
               pgraphics->set_font(ptab->get_data()->m_fontBigBold);
               pgraphics->set_alpha_mode(emode);
               pgraphics->_DrawText(MAGIC_PALACE_TAB_TEXT,rectText, e_align_center, e_draw_text_no_prefix);
               rectText.left += sSep.cx;
               pgraphics->selectFont(font);
               pgraphics->SelectObject(brushText);
            }
         }

      }

   }



   bool user_style::_001OnTabLayout(::user::tab * ptab)
   {


      {

         //         DWORD dwTime2 = ::get_tick_count();

         //TRACE("message_handler call time0= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
         //TRACE("usertab::on_layout call time1= %d ms",dwTime2 - t_time1.operator DWORD_PTR());
      }
      if(!ptab->get_data()->m_bCreated)
         return false;

      ptab->m_bDrawTabAtBackground = true;

      ptab->defer_handle_auto_hide_tabs(false);

      ::draw2d::memory_graphics pgraphics(allocer());
      pgraphics->SelectObject(ptab->get_data()->m_fontBold);

      ptab->m_dcextension.GetTextExtent(pgraphics,MAGIC_PALACE_TAB_SIZE,ptab->get_data()->m_sizeSep);



      if(ptab->get_data()->m_bVertical)
      {
         i32 iTabWidth = 16;
         i32 iTabHeight = 8;
         i32 cx;
         i32 cy;
         for(i32 iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = ptab->get_data()->m_panea(iPane);

            if(!tab_pane.m_bTabPaneVisible)
               continue;

            string str = tab_pane.get_title();

            tab_pane.do_split_layout(ptab->m_dcextension,graphics);

            ::size size;

            ptab->m_dcextension.GetTextExtent(pgraphics,str,size);



            if(tab_pane.m_dib.is_set())
            {
               size.cx += tab_pane.m_dib->m_size.cx + 2;
               size.cy = MAX(size.cy,tab_pane.m_dib->m_size.cy);
            }
            cx = size.cx + 2;

            if(!tab_pane.m_bPermanent)
            {
               cx += 2 + 16 + 2;
            }

            if(cx > iTabWidth)
            {
               iTabWidth = cx;
            }
            cy = size.cy + 2;
            if(cy > iTabHeight)
            {
               iTabHeight = cy;
            }
         }

         // close tab button
         cy = 2 + 16 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabWidth += ptab->get_data()->m_rectBorder.left + ptab->get_data().m_rectBorder.right +
                      ptab->get_data()->m_rectMargin.left + ptab->get_data().m_rectMargin.right +
                      ptab->get_data()->m_rectTextMargin.left + ptab->get_data().m_rectTextMargin.right;

         ptab->get_data()->m_iTabWidth = iTabWidth;

         iTabHeight += ptab->get_data()->m_rectBorder.top + ptab->get_data().m_rectBorder.bottom +
                       ptab->get_data()->m_rectMargin.top + ptab->get_data().m_rectMargin.bottom +
                       ptab->get_data()->m_rectTextMargin.top + ptab->get_data().m_rectTextMargin.bottom;

         ptab->get_data()->m_iTabHeight = iTabHeight;

         rect rectClient;
         ptab->GetClientRect(rectClient);

         ptab->get_data()->m_rectTab.left       = rectClient.left;
         ptab->get_data()->m_rectTab.top        = rectClient.top;
         ptab->get_data()->m_rectTab.right      = ptab->get_data()->m_rectTab.left + ptab->get_data().m_iTabWidth;
         ptab->get_data()->m_rectTab.bottom     = rectClient.bottom;

         /*      m_pui->SetWindowPos(
         ZORDER_TOP,
         m_rectTab.left,
         m_rectTab.top,
         m_rectTab.width(),
         m_rectTab.height(),
         0);*/

         ptab->get_data()->m_rectTabClient.left       = ptab->m_bShowTabs ? ptab->get_data().m_rectTab.right : rectClient.left;
         ptab->get_data()->m_rectTabClient.top        = ptab->get_data().m_rectTab.top;
         ptab->get_data()->m_rectTabClient.right      = rectClient.right;
         ptab->get_data()->m_rectTabClient.bottom     = ptab->get_data().m_rectTab.bottom;

      }
      else
      {
         i32 iTabHeight = 16;
         i32 cy;
         ::draw2d::graphics_pointer graphics(allocer());
         pgraphics->CreateCompatibleDC(NULL);
         ::draw2d::graphics_pointer & pgraphics = graphics;
         pgraphics->SelectObject(ptab->get_data()->m_fontBold);

         rect rectClient;
         ptab->GetClientRect(rectClient);
         int x = rectClient.left;

         i32 ixAdd;
         for(i32 iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = ptab->get_data()->m_panea(iPane);

            if(!tab_pane.m_bTabPaneVisible)
               return false;

            string str = tab_pane.get_title();

            tab_pane.do_split_layout(ptab->m_dcextension,graphics);

            size size;

            ptab->m_dcextension.GetTextExtent(pgraphics,str,size);

            if(tab_pane.m_dib.m_p != NULL)
            {
               size.cy = MAX(size.cy,tab_pane.m_dib->size().cy);
            }
            cy = size.cy + 2;

            if(cy > iTabHeight)
            {
               iTabHeight = cy;
            }

            tab_pane.m_pt.x = x;
            tab_pane.m_pt.y = rectClient.top;


            //            string str = tab_pane.get_title();

            //            size size;

            ixAdd = 5;

            if(tab_pane.m_dib.is_set())
            {
               //::image_list::info ii;
               ixAdd += tab_pane.m_dib->m_size.cx + 2;
            }

            if(!tab_pane.m_bPermanent)
            {
               ixAdd += 2 + 16 + 2;
            }




            tab_pane.m_size.cx = size.cx + ixAdd
                                 + ptab->get_data()->m_rectBorder.left + ptab->get_data().m_rectBorder.right
                                 + ptab->get_data()->m_rectMargin.left + ptab->get_data().m_rectMargin.right
                                 + ptab->get_data()->m_rectTextMargin.left + ptab->get_data().m_rectTextMargin.right;

            x += tab_pane.m_size.cx;
         }

         // close tab button
         cy = 2 + 8 + 2;
         if(cy > iTabHeight)
         {
            iTabHeight = cy;
         }

         iTabHeight += ptab->get_data()->m_rectBorder.top + ptab->get_data().m_rectBorder.bottom +
                       ptab->get_data()->m_rectMargin.top + ptab->get_data()->m_rectMargin.bottom + ptab->get_data().m_iHeightAddUp;

         iTabHeight += 8;

         ptab->get_data()->m_iTabHeight = iTabHeight;

         for(i32 iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
         {

            ::user::tab_pane & tab_pane = ptab->get_data()->m_panea(iPane);

            tab_pane.m_size.cy = iTabHeight;

         }




         ptab->get_data()->m_rectTab.left       = rectClient.left;
         ptab->get_data()->m_rectTab.top        = rectClient.top;
         ptab->get_data()->m_rectTab.right      = rectClient.right;
         ptab->get_data()->m_rectTab.bottom     = ptab->get_data()->m_rectTab.top + ptab->get_data().m_iTabHeight;

         /*      SetWindowPos(
         ZORDER_TOP,
         m_rectTab.left,
         m_rectTab.top,
         m_rectTab.width(),
         m_rectTab.height(),
         0);*/

         rect & rectTabClient = ptab->get_data()->m_rectTabClient;

         rectTabClient.left       = ptab->get_data()->m_rectTab.left;
         rectTabClient.top        = ptab->m_bShowTabs ? ptab->get_data()->m_rectTab.bottom : rectClient.top;
         rectTabClient.right      = ptab->get_data()->m_rectTab.right;
         rectTabClient.bottom     = rectClient.bottom;

         //TRACE0("rectTabClient");

      }

      for(i32 iPane = 0; iPane < ptab->get_data()->m_panea.get_size(); iPane++)
      {

         if(iPane != ptab->_001GetSel())
         {

            ptab->layout_pane(iPane);

         }

      }

      ptab->layout_pane(ptab->_001GetSel(), ptab->IsWindowVisible());

      return true;

   }


   bool user_style::on_ui_event(::user::e_event eevent, ::user::e_object eobject, ::user::interaction * pinteraction)
   {

      if (eevent == ::user::event_calc_item_height)
      {

         pinteraction->m_iItemHeight += 8;

         return true;

      }

      return false;

   }


   bool user_style::_001DrawToolbarItem(::draw2d::graphics_pointer & pgraphics, i32 iItem, ::user::toolbar * ptoolbar)
   {

      if (1)
      {

         _001DrawAnthillToolbarItem(pgraphics, iItem, ptoolbar);

      }
      else
      {

         _001DrawSimpleToolbarItem(pgraphics, iItem, ptoolbar);

      }

      return true;

   }


   void user_style::_001DrawSimpleToolbarItem(::draw2d::graphics_pointer & pgraphics, i32 iItem, ::user::toolbar * ptoolbar)
   {

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      rect rectItem;

      rect rectImage;

      ptoolbar->select_font(pgraphics, ::user::font_toolbar);

      ::user::toolbar_item & item = ptoolbar->m_itema(iItem);

      UINT nStyle = ptoolbar->GetButtonStyle(iItem);

      bool bHover = iItem == ptoolbar->_001GetHoverItem();

      BaseMenuCentral * pmenucentral = BaseMenuCentral::GetMenuCentral(get_context_application());

      UINT uiImage = pmenucentral->CommandToImage(item.m_id);

      ::user::toolbar::e_element eelement = ::user::toolbar::element_item;
      ::user::toolbar::e_element eelementImage = ::user::toolbar::element_image;
      ::user::toolbar::e_element eelementText = ::user::toolbar::element_text;
      if ((nStyle & TBBS_SEPARATOR) == 0)
      {
         if ((nStyle & TBBS_DISABLED) == 0)
         {
            // item is enabled
            if (ptoolbar->m_iButtonPressItem >= 0)
            {
               if (iItem == ptoolbar->m_iButtonPressItem)
               {
                  if (bHover)
                  {
                     eelement = ::user::toolbar::element_item_press;
                     eelementImage = ::user::toolbar::element_image_press;
                     eelementText = ::user::toolbar::element_text_press;
                  }
                  else
                  {
                     eelement = ::user::toolbar::element_item_hover;
                     eelementImage = ::user::toolbar::element_image_hover;
                     eelementText = ::user::toolbar::element_text_hover;
                  }
               }
            }
            else if (bHover)
            {
               eelement = ::user::toolbar::element_item_hover;
               eelementImage = ::user::toolbar::element_image_hover;
               eelementText = ::user::toolbar::element_text_hover;
            }
         }
         else
         {
            // item is disabled
            eelement = ::user::toolbar::element_item;
            eelementImage = ::user::toolbar::element_image;
            eelementText = ::user::toolbar::element_text;
         }
      }
      else
      {
         eelement = ::user::toolbar::element_item;
         eelementImage = ::user::toolbar::element_image;
         eelementText = ::user::toolbar::element_text;
      }


      int iOffsetX = 0;
      int iOffsetY = 0;

      ptoolbar->_001GetElementRect(iItem, rectItem, eelement);

      ptoolbar->_001GetElementRect(iItem, rectImage, eelementImage);

      if ((nStyle & TBBS_SEPARATOR) != 0)
      {
         rect rectSeparator;
         rectSeparator.left = (rectImage.left + rectImage.right) / 2 - 1;
         rectSeparator.right = rectSeparator.left + 2;
         rectSeparator.top = rectImage.top;
         rectSeparator.bottom = rectImage.bottom;
         pgraphics->Draw3dRect(rectSeparator, ARGB(255, 92, 92, 92), ARGB(255, 255, 255, 255));
      }
      else
      {
         if (eelement == ::user::toolbar::element_item_hover)
         {
            if ((nStyle & TBBS_CHECKED) != 0)
            {

               ptoolbar->_001GetElementRect(iItem, rectItem, ::user::toolbar::element_item);

               ptoolbar->_001GetElementRect(iItem, rectImage, ::user::toolbar::element_image);

               if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
               {
                  System.imaging().color_blend(
                     pgraphics,
                     rectItem.left,
                     rectItem.top,
                     rectItem.width(),
                     rectItem.height(),
                     RGB(255, 255, 250), 208);

                  pgraphics->Draw3dRect(rectItem, ARGB(255, 127, 127, 127), ARGB(255, 255, 255, 255));
               }

               if (uiImage != 0xffffffffu)
               {

                  if ((nStyle & TBBS_DISABLED) == 0)
                  {

                     // button is enabled
                     pmenucentral->MenuV033GetImageListBlend()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

                  }
                  else
                  {

                     // button is disabled
                     pmenucentral->MenuV033GetImageListHueLight()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

                  }

               }

            }
            else
            {

               rect rectShadow;

               ptoolbar->_001GetElementRect(iItem, rectShadow, ::user::toolbar::element_item_hover);

               if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
               {

                  ::draw2d::pen_pointer pen(pgraphics, 1, ARGB(184, 92, 184, 92));
                  ::draw2d::brush_pointer brush(allocer(), ARGB(123, 177, 184, 255));
                  ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
                  ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
                  pgraphics->Rectangle(rectItem);
                  pgraphics->SelectObject(ppenOld);
                  pgraphics->SelectObject(pbrushOld);

               }

               if (item.m_spdib.is_set())
               {

                  rect rect;

                  ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_image_hover);

                  System.imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 0.84);

               }
               else if (uiImage != 0xffffffffu)
               {

                  rect rect;

                  ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_item_hover);

                  pmenucentral->MenuV033GetImageListHue()->draw(pgraphics, uiImage, rect.top_left(), 0);

                  pmenucentral->MenuV033GetImageList()->draw(pgraphics, uiImage, rectImage.top_left(), 0);
               }

            }

         }
         else if (eelement == ::user::toolbar::element_item_press)
         {

            if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
            {

               ::draw2d::pen_pointer pen(pgraphics, 1, ARGB(255, 92, 92, 92));
               ::draw2d::brush_pointer brush(allocer(), ARGB(255, 255, 255, 255));
               ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
               ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
               pgraphics->Rectangle(rectItem);
               pgraphics->SelectObject(ppenOld);
               pgraphics->SelectObject(pbrushOld);

            }

            if (item.m_spdib.is_set())
            {

               rect rect;

               ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_image_press);

               System.imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 1.0);

            }
            else if (uiImage != 0xffffffff)
            {

               pmenucentral->MenuV033GetImageList()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

            }

         }
         else
         {

            if ((nStyle & TBBS_DISABLED) == 0)
            {

               ptoolbar->_001GetElementRect(iItem, rectItem, ::user::toolbar::element_item);

               pgraphics->FillSolidRect(rectItem, ARGB(184, 255, 255, 255));

            }

            if ((nStyle & TBBS_CHECKED) != 0)
            {

               pgraphics->Draw3dRect(rectItem, ARGB(255, 127, 127, 127), ARGB(255, 255, 255, 255));

            }

            if (item.m_spdib.is_set())
            {

               rect rect;

               ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_image);

               System.imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 0.23);

            }
            else if (uiImage != 0xffffffff)
            {

               if ((nStyle & TBBS_DISABLED) == 0)
               {

                  pmenucentral->MenuV033GetImageListBlend()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

               }
               else
               {

                  pmenucentral->MenuV033GetImageListHueLight()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

               }

            }

         }

      }

      if (item.m_str.has_char())
      {

         ptoolbar->select_font(pgraphics, ::user::font_toolbar);

         rect rectText;

         ::draw2d::brush_pointer brushText(allocer());

         if ((nStyle & TBBS_DISABLED) == 0)
         {

            brushText->create_solid(ARGB(255, 0, 0, 0));

         }
         else
         {

            brushText->create_solid(ARGB(255, 123, 123, 118));

         }

         pgraphics->SelectObject(brushText);

         if (ptoolbar->_001GetElementRect(iItem, rectText, eelementText) && rectText.right > 0)
         {

            pgraphics->_DrawText(item.m_str, item.m_str.get_length(), rectText, e_align_bottom_left, e_draw_text_no_prefix);

         }

      }

   }

   void user_style::_001DrawAnthillToolbarItem(::draw2d::graphics_pointer & pgraphics, i32 iItem, ::user::toolbar * ptoolbar)
   {

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      rect rectItem;

      rect rectImage;

      ptoolbar->select_font(pgraphics, ::user::font_toolbar);

      ::user::toolbar_item & item = ptoolbar->m_itema(iItem);

      //UINT nStyle = ptoolbar->GetButtonStyle(iItem);
      UINT nStyle = 0;

      bool bHover = iItem == ptoolbar->_001GetHoverItem();

      BaseMenuCentral * pmenucentral = BaseMenuCentral::GetMenuCentral(get_context_application());

      UINT uiImage = pmenucentral->CommandToImage(item.m_id);

      ::user::toolbar::e_element eelement = ::user::toolbar::element_item;
      ::user::toolbar::e_element eelementImage = ::user::toolbar::element_image;
      ::user::toolbar::e_element eelementText = ::user::toolbar::element_text;
      if ((nStyle & TBBS_SEPARATOR) == 0)
      {
         if ((nStyle & TBBS_DISABLED) == 0)
         {
            // item is enabled
            if (ptoolbar->m_iButtonPressItem >= 0)
            {
               if (iItem == ptoolbar->m_iButtonPressItem)
               {
                  if (bHover)
                  {
                     eelement = ::user::toolbar::element_item_press;
                     eelementImage = ::user::toolbar::element_image_press;
                     eelementText = ::user::toolbar::element_text_press;
                  }
                  else
                  {
                     eelement = ::user::toolbar::element_item_hover;
                     eelementImage = ::user::toolbar::element_image_hover;
                     eelementText = ::user::toolbar::element_text_hover;
                  }
               }
            }
            else if (bHover)
            {
               eelement = ::user::toolbar::element_item_hover;
               eelementImage = ::user::toolbar::element_image_hover;
               eelementText = ::user::toolbar::element_text_hover;
            }
         }
         else
         {
            // item is disabled
            eelement = ::user::toolbar::element_item;
            eelementImage = ::user::toolbar::element_image;
            eelementText = ::user::toolbar::element_text;
         }
      }
      else
      {
         eelement = ::user::toolbar::element_item;
         eelementImage = ::user::toolbar::element_image;
         eelementText = ::user::toolbar::element_text;
      }


      int iOffsetX = 0;
      int iOffsetY = 0;

      ptoolbar->_001GetElementRect(iItem, rectItem, eelement);

      ptoolbar->_001GetElementRect(iItem, rectImage, eelementImage);

      if (item.m_id.compare_ci("separator") == 0)
      {
         /*rect rectSeparator;
         rectSeparator.left = (rectImage.left + rectImage.right) / 2 - 1;
         rectSeparator.right = rectSeparator.left + 2;
         rectSeparator.top = rectImage.top;
         rectSeparator.bottom = rectImage.bottom;*/
         //pgraphics->Draw3dRect(rectSeparator, ARGB(255, 92, 92, 92), ARGB(255, 255, 255, 255));
      }
      else
      {
         if (eelement == ::user::toolbar::element_item_hover)
         {
            if ((nStyle & TBBS_CHECKED) != 0)
            {

               ptoolbar->_001GetElementRect(iItem, rectItem, ::user::toolbar::element_item);

               ptoolbar->_001GetElementRect(iItem, rectImage, ::user::toolbar::element_image);

               if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
               {
                  System.imaging().color_blend(
                     pgraphics,
                     rectItem.left,
                     rectItem.top,
                     rectItem.width(),
                     rectItem.height(),
                     RGB(255, 255, 250), 208);

                  pgraphics->Draw3dRect(rectItem, ARGB(255, 127, 127, 127), ARGB(255, 255, 255, 255));
               }

               if (uiImage != 0xffffffffu)
               {

                  if ((nStyle & TBBS_DISABLED) == 0)
                  {

                     // button is enabled
                     pmenucentral->MenuV033GetImageListBlend()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

                  }
                  else
                  {

                     // button is disabled
                     pmenucentral->MenuV033GetImageListHueLight()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

                  }

               }

            }
            else
            {

               rect rectShadow;

               ptoolbar->_001GetElementRect(iItem, rectShadow, ::user::toolbar::element_item_hover);

               if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
               {

                  ::draw2d::pen_pointer pen(pgraphics, 1, ptoolbar->_001GetColor(::user::color_button_background_hover));
                  ::draw2d::brush_pointer brush(allocer(), ptoolbar->_001GetColor(::user::color_button_background_hover));
                  ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
                  ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
                  pgraphics->Rectangle(rectItem);
                  pgraphics->SelectObject(ppenOld);
                  pgraphics->SelectObject(pbrushOld);

               }

               if (item.m_spdib.is_set())
               {

                  rect rect;

                  ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_image_hover);

                  System.imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 0.84);

               }
               else if (uiImage != 0xffffffffu)
               {

                  rect rect;

                  ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_item_hover);

                  pmenucentral->MenuV033GetImageListHue()->draw(pgraphics, uiImage, rect.top_left(), 0);

                  pmenucentral->MenuV033GetImageList()->draw(pgraphics, uiImage, rectImage.top_left(), 0);
               }

            }

         }
         else if (eelement == ::user::toolbar::element_item_press)
         {

            if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
            {

               ::draw2d::pen_pointer pen(pgraphics, 1, ptoolbar->_001GetColor(::user::color_button_background_press));
               ::draw2d::brush_pointer brush(allocer(), ptoolbar->_001GetColor(::user::color_button_background_press));
               ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
               ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
               pgraphics->Rectangle(rectItem);
               pgraphics->SelectObject(ppenOld);
               pgraphics->SelectObject(pbrushOld);

            }

            if (item.m_spdib.is_set())
            {

               rect rect;

               ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_image_press);

               System.imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 1.0);

            }
            else if (uiImage != 0xffffffff)
            {

               pmenucentral->MenuV033GetImageList()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

            }

         }
         else
         {

            if ((nStyle & TBBS_DISABLED) == 0)
            {

               ptoolbar->_001GetElementRect(iItem, rectItem, ::user::toolbar::element_item);

               pgraphics->FillSolidRect(rectItem, ARGB(184, 255, 255, 255));

            }

            if ((nStyle & TBBS_CHECKED) != 0)
            {

               pgraphics->Draw3dRect(rectItem, ARGB(255, 127, 127, 127), ARGB(255, 255, 255, 255));

            }

            if ((ptoolbar->m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
            {

               ::draw2d::pen_pointer pen(pgraphics, 1, ptoolbar->_001GetColor(::user::color_background_normal));
               ::draw2d::brush_pointer brush(allocer(), ptoolbar->_001GetColor(::user::color_background_normal));
               ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
               ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
               pgraphics->Rectangle(rectItem);
               pgraphics->SelectObject(ppenOld);
               pgraphics->SelectObject(pbrushOld);

            }

            if (item.m_spdib.is_set())
            {

               rect rect;

               ptoolbar->_001GetElementRect(iItem, rect, ::user::toolbar::element_image);

               System.imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 0.23);

            }
            else if (uiImage != 0xffffffff)
            {

               if ((nStyle & TBBS_DISABLED) == 0)
               {

                  pmenucentral->MenuV033GetImageListBlend()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

               }
               else
               {

                  pmenucentral->MenuV033GetImageListHueLight()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

               }

            }

         }

      }

      if (item.m_str.has_char())
      {

         ptoolbar->select_font(pgraphics, ::user::font_toolbar);

         rect rectText;

         ::draw2d::brush_pointer brushText(allocer());

         if ((nStyle & TBBS_DISABLED) == 0)
         {

            brushText->create_solid(ARGB(255, 0, 0, 0));

            pgraphics->set_text_color(ARGB(255, 0, 0, 0));

         }
         else
         {

            brushText->create_solid(ARGB(255, 123, 123, 118));

            pgraphics->set_text_color(ARGB(255, 123, 123, 118));

         }

         if (ptoolbar->_001GetElementRect(iItem, rectText, eelementText) && rectText.right > 0)
         {

            pgraphics->_DrawText(item.m_str, item.m_str.get_length(), rectText, e_align_bottom_left, e_draw_text_no_prefix);

         }

      }

   }



   bool user_style::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 192, 192, 192);

         return true;

      }
      else if (ecolor == ::user::color_tab_layout_background)
      {

         cr = ARGB(255, 192, 192, 192);

         return true;

      }
      else if (ecolor == ::user::color_tab_client_background)
      {

         cr = ARGB(255, 74, 74,74);

         return true;

      }
      else if (ecolor == ::user::color_background_hover)
      {

         cr = ARGB(120, 110, 100, 200);

         return true;

      }
      else if (ecolor == ::user::color_split_layout_background)
      {

         cr = ARGB(0, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_list_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_edit_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_edit_background_selected)
      {

         cr = ARGB(120, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_text)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_text_selected)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_text_highlight)
      {

         cr = ARGB(255, 50, 150, 150);

         return true;

      }
      else if (ecolor == ::user::color_text_selected_highlight)
      {

         cr = ARGB(255, 20, 90, 100);

         return true;

      }
      else if (ecolor == ::user::color_edit_text)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_edit_text_selected)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_tree_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_view_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_scrollbar_background)
      {

         cr = ARGB(80, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_background_selected)
      {

         cr = ARGB(255, 155, 185, 255);

         return true;

      }
      else if (ecolor == ::user::color_list_header)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_list_header_background)
      {

         cr = ARGB(255, 0x58, 0x5C, 0x5D);

         return true;

      }
      else if (ecolor == ::user::color_list_item_background)
      {

         cr = ARGB(30, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_list_header_separator)
      {

         cr = 0;

         return true;

      }
      else if (ecolor == ::user::color_toolbar_background)
      {

         cr = ARGB(80, 0xF3, 0XF5, 0xF5);

         return true;


      }
      else if (ecolor == ::user::color_button_background_disabled)
      {

         cr = ARGB(255, 128, 128, 128);

         return true;


      }
      else if (ecolor == ::user::color_button_background_hover)
      {

         cr = ARGB(255, 0xC2, 0x54, 0x55);

         return true;

      }
      else if (ecolor == ::user::color_button_background_normal)
      {

         cr = ARGB(255, 0x75, 0x75, 0x75);

         return true;
      }
      else if (ecolor == ::user::color_button_background_press)
      {

         cr = ARGB(255, 0x91, 0x06, 0x07);

         return true;
      }
      else if (ecolor == ::user::color_button_text_disabled)
      {

         cr = ARGB(255, 0x80, 0x80, 0x80);

         return true;

      }
      else if (ecolor == ::user::color_button_text_hover)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_button_text_normal)
      {

         cr = ARGB(255, 0, 0, 0);

         return true;

      }
      else if (ecolor == ::user::color_button_text_press)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }
      else if (ecolor == ::user::color_action_hover_border_color)
      {

         cr = ARGB(255, 235, 89, 89);

         return true;

      }


      bool bOk = m_mapColor.Lookup(ecolor, cr);

      if (!bOk)
      {

         output_debug_string("\nUnknown color: code=" + ::str::from((int)ecolor));

      }

      return bOk;

   }


} // namespace wndfrm_bockengineering









