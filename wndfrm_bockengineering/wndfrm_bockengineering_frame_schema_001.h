#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_bockengineering
         {


            class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING FrameSchemaHardCoded001 :
               public frame_schema
            {
            public:

               
               FrameSchemaHardCoded001(::aura::application * papp);
               virtual ~FrameSchemaHardCoded001();

               virtual void on_style_change();

               void on_draw_frame(::draw2d::graphics_pointer & pgraphics);
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






