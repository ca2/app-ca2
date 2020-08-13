#include "framework.h"


namespace grbl_merge_tool
{

   split_view1::split_view1(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {

      m_bSplitBar = false;

   }


   split_view1::~split_view1()
   {

   }


   void split_view1::install_message_routing(::channel * pchannel)
   {

      ::user::split_view::install_message_routing(pchannel);

   }


   void split_view1::assert_valid() const
   {

      ::user::split_view::assert_valid();

   }


   void split_view1::dump(dump_context & dumpcontext) const
   {

      ::user::split_view::dump(dumpcontext);

   }


   void split_view1::update(::update * pupdate)
   {

      ::user::split_view::on_update(pSender, eupdate, pupdate);

   }


   void split_view1::on_create_impacts()
   {

      SetPaneCount(2);

      SetSplitOrientation(orientation_vertical);

      initialize_split_layout();

      SIZE sz;

      sz.cx = 100;
      sz.cy = 32;

      set_position_rate(0, 0.65);

      m_plist = create_pane_view < main_form_list >(0);

      if (m_plist == NULL)
      {

         System.simple_message_box(NULL, "Could not create list view");

      }

      Application.m_plist = m_plist;

      m_pcontrol = create_pane_view < control_view >(1);

      if (m_pcontrol == NULL)
      {

         System.simple_message_box(NULL, "Could not create control view");

      }

      Application.m_pcontrolview = m_pcontrol;


   }



   main_view::main_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {

   }


   main_view::~main_view()
   {

   }


   void main_view::install_message_routing(::channel * pchannel)
   {

      ::user::split_view::install_message_routing(pchannel);

   }


   void main_view::assert_valid() const
   {

      ::user::split_view::assert_valid();

   }


   void main_view::dump(dump_context & dumpcontext) const
   {

      ::user::split_view::dump(dumpcontext);

   }


   void main_view::update(::update * pupdate)
   {

      ::user::split_view::on_update(pSender, eupdate, pupdate);

   }


   void main_view::on_create_impacts()
   {

      SetPaneCount(3);

      SetSplitOrientation(orientation_horizontal);

      initialize_split_layout();

      SIZE sz;
      sz.cx = 100;
      sz.cy = 32;

      set_position(0, 32);
      SetPaneFixedSize(0, &sz);

      m_ptext = create_pane_view < ::user::simple_view >(0);

      if (m_ptext == NULL)
      {

         System.simple_message_box(NULL, "Could not create edit view");

      }

      ::user::simple_view::text text;

      text.m_strText = "Please scan item barcode";
      text.m_color.set_COLORREF(ARGB(255, 0, 0, 0));
      text.m_point = point(2, 2);
      text.m_font.alloc(allocer());
      text.m_font->create_point_font("Tahoma", 14, 800);

      m_ptext->m_texta.add(text);

      set_position(1, 64);
      SetPaneFixedSize(0, &sz);

      m_pedit = create_pane_view < ::userex::top_edit_view > (1);

      if (m_pedit == NULL)
      {

         System.simple_message_box(NULL, "Could not create edit view");

      }

      m_pedit->m_id = "top_edit";

      Application.m_pedit2 = m_pedit;
      Application.m_pedit = m_pedit;

      m_pedit->m_puserstyleSelect = m_pedit;

      m_pedit->create_point_font(::user::font_plain_edit, "Tahoma", 16, 800);

      m_pedit->create_color(::user::color_text, ARGB(255, 0, 0, 0));

      auto emode = Application.m_emode;

      m_pedit->enable_window(emode == mode_library);

      m_psplit = create_pane_view < split_view1 >(2);

      if (m_psplit == NULL)
      {

         System.simple_message_box(NULL, "Could not create split view");

      }

   }


} // namespace grbl_merge_tool




