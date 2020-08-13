#include "framework.h"



namespace grbl_merge_tool
{


   pane_view::pane_view(::aura::application * papp) :
      object(papp),
      ::user::tab(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {

   }


   pane_view::~pane_view()
   {

   }


   void pane_view::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void pane_view::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }


   void pane_view::install_message_routing(::channel * pchannel)
   {

      ::userex::pane_tab_view::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &pane_view::_001OnCreate);

   }


   void pane_view::_001OnCreate(::message::message * pobj)
   {

      if (pobj->previous())
         return;

      Application.m_ppaneview = this;

      set_tab("Options", "form_options");

      set_tab("Merge Tool", pane_main);

      set_cur_tab_by_id(pane_main);

      Application.open_serial_port();

   }



   void pane_view::update_serial_ports()
   {

      Application.list_serial_ports();

      auto & pform = m_mapForm["form_options"]->get_typed_view < ::html_form >();

      {

         __pointer(::user::combo_box) pcombo = pform->get_child_by_id("serial_port");

         pcombo->ResetContent();

         pcombo->m_puserstyleSelect = pcombo;
         pcombo->create_color(::user::color_text, ARGB(255, 0, 0, 0));
         pcombo->create_color(::user::color_text_disabled, ARGB(255, 80, 80, 80));

         stringa straPort;

         for (auto & info : Application.m_portinfoa)
         {

            straPort.add_unique_ci(info.port);

         }

         for (auto & strPort : straPort)
         {

            pcombo->AddString(strPort, strPort);

         }

         string str;

         Application.data_get("serial_port", str);

         pcombo->set_current_item_by_string_value(str, ::source_sync);

      }

      {

         __pointer(::user::combo_box) pcombo = pform->get_child_by_id("baud_rate");

         int iBaud;

         if (!Application.data_get("baud_rate", iBaud))
         {

            iBaud = 115200;

         }

         pcombo->m_puserstyleSelect = pcombo;

         pcombo->create_color(::user::color_text, ARGB(255, 0, 0, 0));

         pcombo->create_color(::user::color_text_disabled, ARGB(255, 80, 80, 80));

         pcombo->set_current_item_by_string_value(::str::from(iBaud), ::source_sync);

      }

   }


   bool pane_view::on_control_event(::user::form_window * pview,::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_button_clicked)
      {

         if (pevent->m_puie->m_id == "button_refresh")
         {

            update_serial_ports();

         }

      }
      else if (pevent->m_eevent == ::user::event_after_change_text)
      {

         if (!pevent->m_context.has(::source_sync))
         {

            if (pevent->m_puie->m_id == "top_edit")
            {



            }

         }

         string str;
         pevent->m_puie->_001GetText(str);
         if (str.has_char() && Application.m_emode != mode_library)
         {

            MessageBox(get_safe_handle(), "There is not library open", "Error", MB_ICONEXCLAMATION);
         }

      }
      else if (pevent->m_eevent == ::user::event_after_change_cur_sel)
      {

         if (!pevent->m_context.has(::source_sync))
         {

            if (pevent->m_puie->m_id == "serial_port")
            {

               __pointer(::user::combo_box) pcombo = pevent->m_puie;

               string str = pcombo->_001GetCurSelStringValue();

               Application.open_serial_port(str);

            }
            else if (pevent->m_puie->m_id == "baud_rate")
            {

               __pointer(::user::combo_box) pcombo = pevent->m_puie;

               int iBaud = atoi(pcombo->_001GetCurSelStringValue());

               Application.open_serial_port("", iBaud);

            }

         }


      }

      return false;
   }



   void pane_view::prepare_form(id id, ::html_document * pdoc)
   {

      if (id == "form_options")
      {

         pdoc->open_document("matter://menu.html");

         update_serial_ports();

      }


   }

} // namespace grbl_merge_tool





