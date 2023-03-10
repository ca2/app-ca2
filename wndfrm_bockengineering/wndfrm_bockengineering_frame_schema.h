#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_bockengineering
         {


            class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING frame_schema :
               public ::user::wndfrm::frame::frame
            {
               public:


                  enum e_style
                  {
                     StyleLightBlue,
                     StyleBlackBorder,
                     StyleLightGreen,
                     StyleRedOrange,
                     StyleRouge,
                     StyleBlueRedPurple,
                     StyleEveningSun,
                     StyleTranslucidLightBlue,
                     StyleTranslucidLightGreen,
                     StyleTranslucidWarmGray,
                     StyleTranslucidWarmLiteGray,
                     StyleDarkWarmBlue,
                  };




                  ::draw2d::font_pointer                m_fontEdit;
                  ::draw2d::font_pointer                m_fontList;
                  ::draw2d::font_pointer                m_fontButton;
                  e_style                          m_estyle;
                  //::user::front_end_schema         m_schema;
                  COLORREF                         m_crFrameBorder;
                  COLORREF                         m_crMoveableBorderHilight;
                  COLORREF                         m_crMoveableBorderDkShadow;
                  COLORREF                         m_crCaptionTextBk;
                  COLORREF                         m_crActiveCaptionTextBk;
                  COLORREF                         m_crCaptionText;
                  ::draw2d::pen_pointer                 m_penText1;
                  ::draw2d::pen_pointer                 m_penFace1;
                  ::draw2d::pen_pointer                 m_penHilight1;
                  ::draw2d::pen_pointer                 m_penShadow1;
                  ::draw2d::pen_pointer                 m_penDkShadow1;
                  COLORREF                         m_crDkShadow;
                  size                             m_minSize;

                  ::draw2d::pen_pointer                 m_penHollow1;
                  ::draw2d::pen_pointer                 m_penHollow2;
                  ::draw2d::pen_pointer                 m_penHollow3;
                  ::draw2d::pen_pointer                 m_penHollow4;
                  ::draw2d::pen_pointer                 m_penHollow5;



                  frame_schema(::aura::application * papp);

                  virtual ~frame_schema();

                  virtual COLORREF get_border_main_body_color();

                  virtual bool is_translucid_style(e_style estyle);

                  virtual void set_moveable_border_color(COLORREF cr);

                  virtual void set_button_color_schema_001(COLORREF crMoveableBorder);

                  virtual void set_frame_schema_color_system_default_001();

                  virtual void on_style_change_001_and_002();

                  virtual COLORREF get_style_moveable_border_color(e_style estyle);

                  virtual void OnMove(__pointer(::user::interaction) pwnd);

                  virtual void set_style(e_style estyle);

                  virtual void set_style(const char * pszStyle);

                  virtual void on_style_change();

                  virtual void _000OnBeforeMove(const RECT & lpcrectWindow);

                  virtual void _000OnBeforeSize(const RECT & lpcrectWindow);

                  virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

                  virtual bool get_element_rect(LPRECT lprect, e_element eelement);

                  virtual bool hit_test(const POINT &point, e_element &eelementParam);

                  virtual void Glass(::draw2d::graphics_pointer & pgraphics, const RECT & lprect);

                  virtual void ColorGlass(::draw2d::graphics_pointer & pgraphics, const RECT & lprect, COLORREF cr, BYTE bAlpha);

                  //virtual ::user::front_end_schema * get_user_front_end_schema();

                  virtual void Draw3dRectSide(::draw2d::graphics_pointer & pgraphics, const RECT & rect, EBorder eborder, COLORREF crTopLeft, COLORREF crBottomRight);

                  virtual void on_draw_frame(::draw2d::graphics_pointer & pgraphics);

                  virtual bool is_control_box_moveable() override;


                  virtual bool get_font(::draw2d::font_pointer & sp, e_font efont, ::user::interaction * pinteraction) override;

                  virtual void GetFrameRect(rect * lprect);

            };


            frame_schema::e_element operator++(frame_schema::e_element & eelement, i32 i);


         } // namespace wndfrm_bockengineering


      } // namespace frame


   } // namespace wndfrm


} // namespace user




