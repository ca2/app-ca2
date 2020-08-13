#include "framework.h"



namespace grbl_merge_tool
{


   top_pane_view::top_pane_view(::aura::application * papp) :
      object(papp),
      ::user::tab(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp),
      m_progress(papp)
   {

      m_bNoTabs = true;

   }


   top_pane_view::~top_pane_view()
   {

   }


   void top_pane_view::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void top_pane_view::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }


   void top_pane_view::install_message_routing(::channel * pchannel)
   {

      ::userex::pane_tab_view::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &top_pane_view::_001OnCreate);

   }
   void top_pane_view::on_create_impact(::user::impact_creator_data * pdata)
   {

      if (pdata->m_id == "merge_tool")
      {

         Application.m_ptemplateGrblMergeToolMain->create_subdocument(pdata);


      }
      else
      {

         ::userex::pane_tab_view::on_create_impact(pdata);

      }

   }

   void top_pane_view::_001OnCreate(::message::message * pobj)
   {

      if (pobj->previous())
         return;

      Application.m_ptoppaneview = this;

      set_tab("main", "form_main");

      set_tab("Merge Tool", "merge_tool");

      set_cur_tab_by_id("form_main");

      Application.open_serial_port();

   }

   void top_pane_view::on_show_view()
   {

      ::userex::pane_tab_view::on_show_view();

      Application.update_all_views(update_open_file);

      string str;
      strsize iBeg;
      strsize iEnd;
      if (m_pviewdata->m_id == "form_main")
      {

         if (Application.m_pedit2 != NULL)
         {
            Application.m_pedit2->_001GetText(str);
            Application.m_pedit2->_001GetSel(iBeg, iEnd);
            Application.m_pedit1->_001SetText(str, ::source_sync);
            Application.m_pedit1->_001SetSel(iBeg, iEnd);

         }
         Application.m_pedit = Application.m_pedit1;

      }
      else
      {
         string str;
         Application.m_pedit1->_001GetText(str);
         Application.m_pedit1->_001GetSel(iBeg, iEnd);
         Application.m_pedit2->_001SetText(str, ::source_sync);
         Application.m_pedit2->_001SetSel(iBeg, iEnd);
         Application.m_pedit = Application.m_pedit2;
      }

      Application.m_pedit->SetFocus();

   }


   void top_pane_view::_001OnFormSize(::message::message * pmessage)
   {

      pmessage->all_previous();


   }


   void top_pane_view::on_layout()
   {

      ::userex::pane_tab_view::on_layout();

   }

   void top_pane_view::update_serial_ports()
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


   bool top_pane_view::on_control_event(::user::form_window * pview, ::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_layout)
      {

         auto pform = Application.m_pmainview;
         if (pevent->m_puie == pform)
         {

            __pointer(::user::plain_edit)puiEdit = pform->get_child_by_id("barcode");


            rect r;
            puiEdit->GetWindowRect(&r);

            r.top += 32;
            r.bottom += 32;

            pform->ScreenToClient(r);

            m_progress.SetPlacement(r);

            puiEdit->SetFocus();

         }

      }
      else if (pevent->m_eevent == ::user::event_button_clicked)
      {

         if (pevent->m_puie->m_id == "button_go")
         {

            if (Application.update_commands())
            {

               Application.on_start();

            }

         }
         else if (pevent->m_puie->m_id == "settings")
         {

            set_cur_tab_by_id("merge_tool");

         }

      }
      else if (pevent->m_eevent == ::user::event_after_change_text)
      {

         if (pevent->m_context.has(::source_user))
         {

            if (pevent->m_puie->m_id == "barcode")
            {


               Application.update_commands();

            }

         }

      }
      else if (pevent->m_eevent == ::user::event_enter_key)
      {

         if (pevent->m_puie->m_id == "barcode")
         {

            if (Application.update_commands())
            {

               Application.on_start();

            }

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


   document * top_pane_view::get_document()
   {

      return dynamic_cast <document *> (::userex::pane_tab_view::get_document());

   }

   void top_pane_view::prepare_form(id id, ::html_document * pdoc)
   {

      if (id == "form_main")
      {

         pdoc->open_document("matter://main.html");

         //update_serial_ports();

         html_form * pform = dynamic_cast < html_form * > (pdoc->get_view().m_p);

         Application.m_pmainview = pform;

         IGUI_MSG_LINK(WM_SIZE, pform->m_pimpl, this, &top_pane_view::_001OnFormSize);



         __pointer(::user::plain_edit)puiEdit = pform->get_child_by_id("barcode");
         Application.m_pedit1 = puiEdit;
         Application.m_pedit = puiEdit;
         m_progress.create_window(pform, "progress");
         m_progress.m_pscalar = &Application.m_scalar;
         set_need_layout();
         pform->set_need_layout();

      }


   }

} // namespace grbl_merge_tool





