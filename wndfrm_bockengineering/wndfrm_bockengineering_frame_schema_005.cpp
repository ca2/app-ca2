#include "framework.h"


#define GRIP_CORNER_LARGE_CX 16
#define GRIP_CORNER_SMALL_CX 5
#define GRIP_CORNER_LARGE_CY 16
#define GRIP_CORNER_SMALL_CY 5


#define GRIP_CENTER_LARGE_CX 36
#define GRIP_CENTER_SMALL_CX 7
#define GRIP_CENTER_LARGE_CY 36
#define GRIP_CENTER_SMALL_CY 7


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_bockengineering
         {


            FrameSchemaHardCoded005::FrameSchemaHardCoded005(::aura::application * papp) :
               object(papp),
               frame_schema(papp),
               m_brushGrip(allocer()),
               m_brushTextCaption(allocer())
            {

               set_style(StyleLightGreen);

               //      SetStyle(StyleLightBlue);

               m_brushGrip->create_solid(ARGB(0xff, 0x60, 0x65, 0x55));
               m_brushTextCaption->create_solid(Session.get_default_color(COLOR_CAPTIONTEXT) | 0xff << 24);
            }

            FrameSchemaHardCoded005::~FrameSchemaHardCoded005()
            {

            }

            EHitTest FrameSchemaHardCoded005::_000HitTest(point ptCursor)
            {
               EHitTest etest = hittest_client;
               {
                  //      m_pframewindow->GetEventWindow()->ScreenToClient(point);
                  rect rectEvent;
                  m_pframewindow->GetWindowRect(rectEvent);
                  rect rect;
                  point ptCenter = rectEvent.center();
                  EGrip egrip = m_pframewindow->size_manager()->GetGripMask();

                  point ptHitTest = ptCursor;

                  if(rectEvent.left < 0)
                     ptHitTest.x -= rectEvent.left;
                  if(rectEvent.top < 0)
                     ptHitTest.y -= rectEvent.top;

                  if(egrip & GripTopLeft)
                  {
                     rect = rectEvent;
                     rect.right = rect.left + 16;
                     rect.bottom = rect.top + 5;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_top_left;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.right = rect.left + 5;
                     rect.bottom = rect.top + 16;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_top_left;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripTopRight)
                  {
                     rect = rectEvent;
                     rect.left = rect.right - 16;
                     rect.bottom = rect.top + 5;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_top_right;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.left = rect.right - 5;
                     rect.bottom = rect.top + 16;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_top_right;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripBottomRight)
                  {
                     rect = rectEvent;
                     rect.left = rect.right - 16;
                     rect.top = rect.bottom - 5;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_bottom_right;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.left = rect.right - 5;
                     rect.top = rect.bottom - 16;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_bottom_right;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripBottomLeft)
                  {
                     rect = rectEvent;
                     rect.right = rect.left + 16;
                     rect.top = rect.bottom - 5;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_bottom_left;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.right = rect.left + 5;
                     rect.top = rect.bottom - 16;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_bottom_left;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterTop)
                  {
                     rect.top = rectEvent.top;
                     rect.left = ptCenter.x - 8;
                     rect.right = ptCenter.x + 8;
                     rect.bottom = rectEvent.top + 5;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_top;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterBottom)
                  {
                     rect.top = rectEvent.bottom - 5;
                     rect.left = ptCenter.x - 8;
                     rect.right = ptCenter.x + 8;
                     rect.bottom = rectEvent.bottom;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_bottom;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterLeft)
                  {
                     rect.top = ptCenter.y - 8;
                     rect.left = rectEvent.left;
                     rect.right = rectEvent.left + 5;
                     rect.bottom = ptCenter.y + 8;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_left;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterRight)
                  {
                     rect.top = ptCenter.y - 8;
                     rect.left = rectEvent.right - 5;
                     rect.right = rectEvent.right;
                     rect.bottom = ptCenter.y + 8;
                     if(rect.contains(ptHitTest))
                     {
                        etest = hittest_sizing_right;
                        goto SizingSuccess;
                     }
                  }
                  goto SizingNone;
               SizingSuccess:
                  return etest;
               SizingNone:;
               }
               return hittest_client;
            }




            void FrameSchemaHardCoded005::DrawBorderSide(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient,
               EBorder eside)
            {

               appearance * pappearance = get_appearance();

               if(pappearance == NULL)
                  return;

               COLORREF    crMoveableBorder;
               COLORREF    crMoveableBorderHilight;
               COLORREF    crMoveableBorderShadow;

               if(pappearance->m_fActive)
               {
                  crMoveableBorder = m_crMoveableBorder;
                  crMoveableBorderHilight = m_crMoveableBorderHilight;
                  crMoveableBorderShadow = m_crMoveableBorderShadow;
               }
               else
               {
                  crMoveableBorder = Session.get_default_color(COLOR_BTNFACE);
                  crMoveableBorderHilight = Session.get_default_color(COLOR_BTNHILIGHT);
                  crMoveableBorderShadow = Session.get_default_color(COLOR_BTNSHADOW);
               }

               EDock edock = m_pframewindow->dock_manager()->GetDockState();
               rect rectA(lpcrectClient);

               if(m_estyle == StyleTranslucidWarmGray
                  || m_estyle == StyleTranslucidLightBlue
                  || m_estyle == StyleTranslucidLightGreen)
               {

                  rect rect;

                  GetBorderRect(lpcrectClient, rect, eside);

                  ColorGlass(pgraphics, rect, crMoveableBorder, 127);

               }
               else if(m_estyle == StyleLightBlue)
               {
                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, 0);//m_crMoveableBorderDkShadow);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorderHilight, crMoveableBorderShadow);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, crMoveableBorder);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, crMoveableBorder);

                  rect rect;
                  GetBorderRect(lpcrectClient, rect, eside);
                  class imaging & imaging = System.imaging();
                  imaging.color_blend(pgraphics,
                     rect,
                     crMoveableBorder,
                     127);
               }
               else
               {
                  rect rect;
                  GetBorderRect(lpcrectClient, rect, eside);

                  ColorGlass(pgraphics, rect, crMoveableBorder, 230);

                  class rect rectClientB = rectA;

                  rectClientB.bottom--;
                  rectClientB.right--;

                  rectA.top++;
                  rectA.bottom--;
                  rectA.left++;
                  rectA.right--;
                  if(edock == DockNone)
                  {
                     Draw3dRectSide(pgraphics, rectA, eside, m_crDkShadow, m_crDkShadow);
                  }

                  rectA.top++;
                  rectA.bottom--;
                  rectA.left++;
                  rectA.right--;
                  Draw3dRectSide(pgraphics, rectA, eside, m_crDkShadow, m_crDkShadow);

                  rectA.top++;
                  rectA.bottom--;
                  rectA.left++;
                  rectA.right--;
                  if(edock == DockNone)
                  {
                     Draw3dRectSide(pgraphics, rectA, eside, m_crDkShadow, m_crDkShadow);
                  }
               }

            }

            void FrameSchemaHardCoded005::on_draw_frame(::draw2d::graphics_pointer & pgraphics)
            {

               __pointer(::user::interaction) pwndDraw = get_draw_window();

               if(pwndDraw == NULL)
                  return;

               appearance * pappearance = get_appearance();

               if(pappearance == NULL)
                  return;

               if(!pappearance->IsEnabled())
                  return;

               __pointer(::user::interaction) pwnd = get_window();

               bool bZoomed = pappearance->IsZoomed();

               //    CVMSApp * pApp = (CVMSApp *) &System;
               //::aura::savings & savings = Session.savings();


               rect rectClient;
               pwndDraw->GetClientRect(rectClient);


               string str;


               rect rectNClient;

               pwndDraw->GetWindowRect(rectNClient);

               rectNClient -= rectNClient.top_left();

               ////////////////////
               //
               // Routine:
               // draw frame border
               //
               ////////////////////


               if(!pappearance->IsFullScreen())
               {
                  DrawBorder(pgraphics, rectNClient);
               }


               ///////////////////////
               //
               // Routine:
               // draw resize grip set
               //
               ///////////////////////

               if(!bZoomed && !pappearance->IsFullScreen())
               {
                  DrawGripSet(pgraphics, rectNClient);
               }

               //          pgraphics->SetBkMode(iOriginalBkMode);
               //            pgraphics->set_text_color(crOriginalTextColor);

            }


            void FrameSchemaHardCoded005::DrawBorder(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient)
            {
               MoveManager * pwmm = m_pframewindow->GetMovingManager();

               EBorder eborder = pwmm->GetBorderMask();

               if(get_appearance()->IsZoomed())
               {
                  eborder = (EBorder)
                     (eborder &
                     ~(BorderRight
                     | BorderBottom
                     | BorderLeft));
               }

               if(eborder & BorderTop)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderTop);
               }
               if(eborder & BorderRight)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderRight);
               }
               if(eborder & BorderBottom)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderBottom);
               }
               if(eborder & BorderLeft)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderLeft);
               }

            }

            void FrameSchemaHardCoded005::GetBorderRect(
               const RECT & lpcrectClient,
               LPRECT lprect,
               EBorder eside)
            {
               rect rectBig(lpcrectClient);
               rect rectSmall;
               get_window_client_rect(rectSmall);
               rect rect;
               if(eside == BorderTop)
               {
                  rect.left = rectBig.left;
                  rect.right = rectBig.right;
                  rect.top = rectBig.top;
                  rect.bottom = rectSmall.top;
               }
               else if(eside == BorderLeft)
               {

                  rect.left = rectBig.left;
                  rect.right = rectSmall.left;
                  rect.top = rectSmall.top;
                  rect.bottom = rectSmall.bottom;

               }
               else if(eside == BorderRight)
               {

                  rect.left = rectSmall.right;
                  rect.right = rectBig.right;
                  rect.top = rectSmall.top;
                  rect.bottom = rectSmall.bottom;

               }
               else if(eside == BorderBottom)
               {

                  rect.left = rectBig.left;
                  rect.right = rectBig.right;
                  rect.top = rectSmall.bottom;
                  rect.bottom = rectBig.bottom;

               }

               *lprect = rect;

            }


            void FrameSchemaHardCoded005::Draw3dRectSide(::draw2d::graphics_pointer & pgraphics,const RECT & rectParam,EBorder eborder,COLORREF crTopLeft,COLORREF crBottomRight)
            {

               rect rect(rectParam);
               i32 x = rect.left;
               i32 y = rect.top;
               i32 cx = rect.width();
               i32 cy = rect.height();

               ::rect r;

               if(eborder & BorderTop)
               {

                  r = ::rect_dim(x, y, cx, 1);

                  if(Session.savings().is_trying_to_save(::aura::resource_processing))
                  {

                     pgraphics->FillSolidRect(r, crTopLeft);

                  }
                  else
                  {
                     ColorGlass(pgraphics, r, crTopLeft, 127);

                  }

               }

               if(eborder & BorderLeft)
               {

                  r = ::rect_dim(x, y + 1, 1, cy - 2);

                  if(Session.savings().is_trying_to_save(::aura::resource_processing))
                  {

                     pgraphics->FillSolidRect(r, crTopLeft);

                  }
                  else
                  {

                     ColorGlass(pgraphics, r, crTopLeft, 127);

                  }

               }

               if(eborder & BorderRight)
               {

                  r = ::rect_dim(x + cx - 1, y + 1, 1, cy - 2);

                  if(Session.savings().is_trying_to_save(::aura::resource_processing))
                  {

                     pgraphics->FillSolidRect(r, crBottomRight);

                  }
                  else
                  {

                     ColorGlass(pgraphics, r, crBottomRight, 127);

                  }

               }

               if(eborder & BorderBottom)
               {

                  r = ::rect_dim(x, y + cy - 1, cx, 1);

                  if(Session.savings().is_trying_to_save(::aura::resource_processing))
                  {

                     pgraphics->FillSolidRect(r, crBottomRight);

                  }
                  else
                  {
                     ColorGlass(pgraphics, r, crBottomRight, 127);
                  }

               }

            }

            void FrameSchemaHardCoded005::on_style_change()
            {

               on_style_change_001_and_002();


            }

#define BEVEL_SMALL 2
#define BEVEL_LARGE 5
#define GRIP_SMALL 8
#define GRIP_LARGE 30

            void FrameSchemaHardCoded005::DrawGrip(
               ::draw2d::graphics_pointer & pgraphics,
               const RECT & lpcrectClient,
               EGrip egrip)
            {

               

               const i32 size = 16;

               rect rectClient(lpcrectClient);
               //            rect rectClientB(rectClient);
               rect rectA;
               point ptA;
               point ptB;
               point ptC;
               //            rect rect(rectClient);
               switch(egrip)
               {
               case GripTopLeft:
               {

                                  rectA = rectClient;

                                  rectA.right = 4;
                                  rectA.bottom = size;

                                  pgraphics->FillRect(rectA, m_brushGrip);

                                  rectA.right = size;
                                  rectA.bottom = 4;

                                  pgraphics->FillRect(rectA, m_brushGrip);

               }
                  break;
               case GripTopRight:
               {
                                   rectA = rectClient;

                                   rectA.left = rectA.right - 4;
                                   rectA.bottom = size;

                                   pgraphics->FillRect(rectA, m_brushGrip);

                                   rectA.left = rectA.right - size;
                                   rectA.bottom = 4;

                                   pgraphics->FillRect(rectA, m_brushGrip);
               }
                  break;
               case GripBottomLeft:
               {
                                     rectA = rectClient;

                                     rectA.right = 4;
                                     rectA.top = rectA.bottom - size;

                                     pgraphics->FillRect(rectA, m_brushGrip);

                                     rectA.right = size;
                                     rectA.top = rectA.bottom - 4;

                                     pgraphics->FillRect(rectA, m_brushGrip);
               }
                  break;
               case GripBottomRight:
               {
                                      rectA = rectClient;

                                      rectA.left = rectA.right - 4;
                                      rectA.top = rectA.bottom - size;

                                      pgraphics->FillRect(rectA, m_brushGrip);

                                      rectA.left = rectA.right - size;
                                      rectA.top = rectA.bottom - 4;

                                      pgraphics->FillRect(rectA, m_brushGrip);
               }
                  break;
               case GripCenterTop:
               {
                                    rectA = rectClient;

                                    rectA.left = rectA.left + rectA.width() / 2 - size / 2;
                                    rectA.right = rectA.left + size;
                                    rectA.bottom = 4;

                                    pgraphics->FillRect(rectA, m_brushGrip);

               }
                  break;
               case GripCenterBottom:
               {
                                       rectA = rectClient;

                                       rectA.left = rectA.left + rectA.width() / 2 - size / 2;
                                       rectA.right = rectA.left + size;
                                       rectA.top = rectA.bottom - 4;

                                       pgraphics->FillRect(rectA, m_brushGrip);
               }
                  break;
               case GripCenterLeft:
               {
                                     rectA = rectClient;

                                     rectA.right = 4;
                                     rectA.top = rectA.top + rectA.height() / 2 - size / 2;
                                     rectA.bottom = rectA.top + size;

                                     pgraphics->FillRect(rectA, m_brushGrip);
               }
                  break;
               case GripCenterRight:
               {
                                      rectA = rectClient;

                                      rectA.left = rectA.right - 4;
                                      rectA.top = rectA.top + rectA.height() / 2 - size / 2;
                                      rectA.bottom = rectA.top + size;

                                      pgraphics->FillRect(rectA, m_brushGrip);
               }
                  break;
               default:
                  break;
               }


            }


            void FrameSchemaHardCoded005::DrawRectGrip(::draw2d::graphics_pointer & pgraphics,const RECT & rectParam)
            {

               

               rect rect(rectParam);

               pgraphics->Draw3dRect(rect, Session.get_default_color(COLOR_BTNFACE), Session.get_default_color(COLOR_3DDKSHADOW));

               rect.top++;
               rect.bottom--;
               rect.left++;
               rect.right--;

               pgraphics->Draw3dRect(rect, Session.get_default_color(COLOR_BTNHILIGHT), Session.get_default_color(COLOR_BTNSHADOW));

               rect.top++;
               rect.bottom--;
               rect.left++;
               rect.right--;

               pgraphics->FillSolidRect(rect, Session.get_default_color(COLOR_BTNFACE));

            }


            void FrameSchemaHardCoded005::DrawGripSet(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient)
            {

               SizeManager * pwsm = m_pframework;

               if(pwsm == NULL)
                  return;

               EGrip egrip = pwsm->GetGripMask();

               if(egrip & GripCenterTop)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterTop);
               }
               if(egrip & GripTopRight)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripTopRight);
               }
               if(egrip & GripRight)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterRight);
               }
               if(egrip & GripBottomRight)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripBottomRight);
               }
               if(egrip & GripCenterBottom)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterBottom);
               }
               if(egrip & GripBottomLeft)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripBottomLeft);
               }
               if(egrip & GripCenterLeft)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterLeft);
               }
               if(egrip & GripTopLeft)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripTopLeft);
               }

            }


         } // namespace wndfrm_bockengineering


      } // namespace frame


   } // namespace wndfrm


} // namespace user



















































