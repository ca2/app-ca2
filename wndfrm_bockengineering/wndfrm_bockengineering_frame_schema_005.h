#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_bockengineering
         {


            class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING FrameSchemaHardCoded005 :
               public frame_schema
            {
            public:


               ::draw2d::brush_pointer                m_brushGrip;
               ::draw2d::brush_pointer                m_brushTextCaption;


               FrameSchemaHardCoded005(::aura::application * papp);
               virtual ~FrameSchemaHardCoded005();


               virtual void on_style_change();


               void on_draw_frame(::draw2d::graphics_pointer & pgraphics);
               void Draw3dRectSide(::draw2d::graphics_pointer & pgraphics, const RECT & rect, EBorder eborder, COLORREF crTopLeft, COLORREF crBottomRight);
               void DrawBorder(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient);
               void DrawBorderSide(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient, EBorder eside);
               void GetBorderRect(const RECT & lpcrectClient, LPRECT lprect, EBorder eside);
               void DrawGripSet(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient);
               void DrawGrip(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient, EGrip egrip);
               void DrawRectGrip(::draw2d::graphics_pointer & pgraphics, const RECT & rect);

               virtual EHitTest _000HitTest(const ::point & point);


            };


         } // namespace wndfrm_bockengineering


      } // namespace frame


   } // namespace wndfrm


} // namespace user








