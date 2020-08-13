#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_bockengineering
         {


            class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING FrameSchemaHardCoded008 :
               public frame_schema
            {
               public:


                  bool     m_bHollow;


                  FrameSchemaHardCoded008(::aura::application * papp);
                  virtual ~FrameSchemaHardCoded008();


                  virtual void on_style_change();

                  void on_draw_frame(::draw2d::graphics_pointer & pgraphics);
                  void DrawBorder(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient);
                  void DrawBorderSide(::draw2d::graphics_pointer & pgraphics, const RECT & lpcrectClient, EBorder eside);
                  void GetBorderRect(const RECT & lpcrectClient, LPRECT lprect, EBorder eside);

                  virtual EHitTest _000HitTest(const ::point & point);
                  COLORREF get_border_main_body_color();

                  bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction) override;

            };


         } // namespace wndfrm_bockengineering


      } // namespace frame


   } // namespace wndfrm


} // namespace user






