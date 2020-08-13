#include "framework.h"


namespace windows
{

   ::file::path get_known_folder(REFKNOWNFOLDERID kfid);

}


namespace grbl_merge_tool
{



   main_form_list::main_form_list(::aura::application * papp) :
      ::object(papp),
      simple_form_list_view(papp),
      m_piconUndefined(allocer()),
      m_piconRunning(allocer()),
      m_piconError(allocer()),
      m_piconFullFail(allocer()),
      m_i(get_context_application())
   {
      m_iImageSpacing = 7;
      //m_bMorePlain = true;
      m_i.create(24, 24);

      m_bSortEnable = false;
      //m_ptaska->m_pformlist = this;
      m_iItemHeight = 24;
      m_iLeftMargin = 49;
      m_iTopMargin = 33;

      //connect_command_probe("edit_undo", &main_form_list::_001OnUpdateEditUndo);
      //connect_command("edit_undo", &main_form_list::_001OnEditUndo);
      //connect_command_probe("edit_redo", &main_form_list::_001OnUpdateEditRedo);
      //connect_command("edit_redo", &main_form_list::_001OnEditRedo);
      //connect_command_probe("media_transfer", &main_form_list::_001OnUpdateMediaTransfer);
      //connect_command("media_transfer", &main_form_list::_001OnMediaTransfer);


      //connect_command_probe("task_add", &main_form_list::_001OnUpdateAddTask);
      //connect_command("task_add", &main_form_list::_001OnAddTask);
      //connect_command_probe("task_delete", &main_form_list::_001OnUpdateDeleteTask);
      //connect_command("task_delete", &main_form_list::_001OnDeleteTask);


      //connect_command_probe("file_import", &main_form_list::_001OnUpdateImport);
      //connect_command("file_import", &main_form_list::_001OnImport);
      //connect_command_probe("file_export", &main_form_list::_001OnUpdateExport);
      //connect_command("file_export", &main_form_list::_001OnExport);


      //connect_command_probe("task_start_all", &main_form_list::_001OnUpdateStartAllTasks);
      //connect_command("task_start_all", &main_form_list::_001OnStartAllTasks);
      //connect_command_probe("task_stop_all", &main_form_list::_001OnUpdateStopAllTasks);
      //connect_command("task_stop_all", &main_form_list::_001OnStopAllTasks);

      m_piconUndefined->load_matter("main/undefined.ico");
      m_piconRunning->load_matter("main/valid.ico");
      m_piconError->load_matter("main/error.ico");
      m_piconFullFail->load_matter("main/full_fail.ico");

      m_i.add_file("matter://twittericon.png");

   }

   main_form_list::~main_form_list()
   {

   }


   void main_form_list::_001InsertColumns()
   {

      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 30;

         pcolumn->m_iSubItem = subitem_spacing;

         pcolumn->m_uiText = "";

         _001AddColumn(column);

      }


      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 60;

         pcolumn->m_iSubItem = subitem_line;

         pcolumn->m_uiText = "LINE";

         _001AddColumn(column);

      }

      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 180;

         pcolumn->m_iSubItem = subitem_command;

         pcolumn->m_uiText = "COMMAND";

         _001AddColumn(column);

      }

      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 100;

         pcolumn->m_iSubItem = subitem_state;

         pcolumn->m_uiText = "STATE";

         _001AddColumn(column);

      }

      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 100;

         pcolumn->m_iSubItem = subitem_response;

         pcolumn->m_uiText = "RESPONSE";

         _001AddColumn(column);

      }



   }

   bool main_form_list::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return simple_form_list_view::get_color(cr, ecolor, pinteraction);

   }


   void main_form_list::_001OnShowControl(__pointer(control) pcontrol)
   {
      if (pcontrol->descriptor().m_id == 1001)
      {

         __pointer(::user::combo_box) pcombobox = pcontrol;

         if (pcombobox.is_set())
         {

            pcombobox->ShowDropDown();

         }
      }

   }


   void main_form_list::_001OnInitializeForm(__pointer(control) pcontrol)
   {

      if (pcontrol->descriptor().m_id == "status_check_box")
      {
         __pointer(::user::check_box) pcheckbox = pcheckbox;

         if (pcheckbox.is_set())
         {

            pcheckbox->m_estyle = ::user::check_box::style_red_green_circle;

         }
      }

   }


   void main_form_list::on_update(::user::impact * pSender, LPARAM lHint, ::object * pupdate)
   {

      simple_form_list_view::on_update(pSender, lHint, pupdate);

      if (lHint == 0)
      {
         _001OnUpdateItemCount();

      }
      else if (lHint == update_open_file)
      {

         _001OnUpdateItemCount();

      }
      else if (lHint == update_open_library)
      {


      }
      else if (lHint == update_reset)
      {

         reset_items();

      }

      if (pupdate != NULL)
      {

         __pointer(::impact_update) pupdate = pupdate;

         if (pupdate.is_set())
         {

            if (pupdate->m_ehint == ::impact_update::update_after_change_text_delayed)
            {

               if (pupdate->m_pui->m_id == "top_edit")
               {

                  __pointer(::user::edit_text) ptext = pupdate->m_pui;

                  ptext->_001GetText(Application.m_strCode);

                  bool bEnterKeyPressed = pupdate->m_bEnterKeyPressed;

                  fork([this, bEnterKeyPressed]()
                  {

                     if (Application.update_commands())
                     {

                        if (bEnterKeyPressed)
                        {

                           Application.on_start();

                        }

                     }

                  });

               }

            }

         }

      }



   }


   //void main_form_list::_001OnContextMenu(::message::message * pobj)
   //{

   //   track_popup_xml_matter_menu("devedge_contextmenu.xml", 0, pobj);

   //}
   //void main_form_list::_001OnUpdateImport(::message::message * pobj)
   //{

   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(true);

   //}


   //void main_form_list::_001OnImport(::message::message * pobj)
   //{

   //   pobj->m_bRet = true;

   //   ::fork(get_context_application(), [&]()
   //   {

   //      string str;

   //      data_get("last_import_folder", str);

   //      if (str.is_empty())
   //      {

   //         str = Context.dir().home() / "GRBL Merge Tool";

   //         if (!Context.dir().is(str))
   //         {

   //            str = ::windows::get_known_folder(FOLDERID_Desktop);

   //            if (!Context.dir().is(str))
   //            {

   //               str = "";

   //            }

   //         }


   //      }

   //      property_set set;

   //      set["file_filter_specs"] = "*.csv";
   //      set["file_filter_names"] = "Image Files";
   //      set["folder"] = str;

   //      if (!Context.os().browse_file_open(get_handle(), set))
   //      {

   //         return;
   //         //::grbl_merge_tool::get_file_open(GetTopLevelFrame(), str);

   //      }

   //      file::path path = set["file_name"];

   //      if (path.is_empty())
   //      {

   //         return;

   //      }

   //      data_set("last_import_folder", path.folder());

   //      get_document()->m_filepath = path;

   //      get_document()->update_all_views(NULL, document::update_import);

   //   });

   //}

   //void main_form_list::_001OnUpdateExport(::message::message * pobj)
   //{

   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(true);

   //}


   //void main_form_list::_001OnExport(::message::message * pobj)
   //{

   //   pobj->m_bRet = true;

   //   ::fork(get_context_application(), [&]()
   //   {

   //      string str;

   //      data_get("last_export_folder", str);

   //      if (str.is_empty())
   //      {

   //         str = ::windows::get_known_folder(FOLDERID_Desktop);

   //         if (!Context.dir().is(str))
   //         {

   //            str = "";

   //         }

   //      }
   //      property_set set;

   //      set["file_filter_specs"] = "*.csv";
   //      set["file_filter_names"] = "Image Files";
   //      set["folder"] = str;
   //      set["default_file_extension"] = "csv";

   //      if (!Context.os().browse_file_save(get_handle(), set))
   //      {

   //         return;
   //         //file::path path = ::grbl_merge_tool::get_file_save(GetTopLevelFrame(), str);


   //      }

   //      file::path path = set["file_name"];

   //      if (path.is_empty())
   //      {

   //         return;

   //      }

   //      if (path.extension().is_empty())
   //      {

   //         path += ".csv";

   //      }

   //      data_set("last_export_folder", path.folder());

   //      get_document()->m_filepath = path;

   //      get_document()->update_all_views(NULL, document::update_export);

   //   });

   //}

   //void main_form_list::_001OnUpdateStartAllTasks(::message::message * pobj)
   //{

   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(true);

   //}


   //void main_form_list::_001OnStartAllTasks(::message::message * pobj)
   //{


   //}


   //void main_form_list::_001OnUpdateStopAllTasks(::message::message * pobj)
   //{

   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(true);

   //}


   //void main_form_list::_001OnStopAllTasks(::message::message * pobj)
   //{
   //}

   //void main_form_list::_001OnUpdateAddTask(::message::message * pobj)
   //{

   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(true);

   //}


   //void main_form_list::_001OnAddTask(::message::message * pobj)
   //{

   //   _001OnUpdateItemCount();

   //   pobj->m_bRet = true;

   //}


   //void main_form_list::_001OnUpdateDeleteTask(::message::message * pobj)
   //{
   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(_001GetSelectedItemCount() > 0);
   //}
   //void main_form_list::_001OnDeleteTask(::message::message * pobj)
   //{
   //   pobj->m_bRet = true;
   //}


   //void main_form_list::_001OnUpdateEditUndo(::message::message * pobj)
   //{
   //   SCAST_PTR(::user::command, pcmdui, pobj);
   //   //pcmdui->Enable(m_ptree->m_editfile.CanUndo());
   //}

   //void main_form_list::_001OnUpdateEditRedo(::message::message * pobj)
   //{
   //   SCAST_PTR(::user::command, pcmdui, pobj);
   //   //pcmdui->Enable(m_ptree->m_editfile.GetRedoBranchCount() > 0);
   //}

   //void main_form_list::_001OnEditUndo(::message::message * pobj)
   //{


   //   UNREFERENCED_PARAMETER(pobj);

   //   //Undo();


   //}


   //void main_form_list::_001OnEditRedo(::message::message * pobj)
   //{


   //   UNREFERENCED_PARAMETER(pobj);

   //   //Redo();


   //}

   /*   void main_form_list::_001GetItemText(::user::mesh_item * pitem)
      {
      }*/

   void main_form_list::_001GetItemImage(::user::mesh_item * pitem)
   {

      if (pitem->m_iSubItem == 1)
      {

         pitem->m_bOk = true;

         pitem->m_iImage = 0;

         return;

      }
      else if (pitem->m_iSubItem == 2)
      {

         pitem->m_bOk = true;

         pitem->m_iImage = 0;

         return;

      }

      pitem->m_bOk = false;

   }



   void main_form_list::_001OnAfterChangeText(const ::action_context & context)
   {


      //::user::scroll_view::_001OnAfterChangeText(context);

      if (get_document() != NULL)
      {

         get_document()->set_modified_flag(TRUE);

      }


   }


   void main_form_list::install_message_routing(::channel * pchannel)
   {


      simple_form_list_view::install_message_routing(pchannel);


      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &main_form_list::_001OnCreate);
//      IGUI_MSG_LINK(WM_CONTEXTMENU, pchannel, this, &main_form_list::_001OnContextMenu);


   }


   void main_form_list::_001OnCreate(::message::message * pobj)
   {


      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      if (pcreate->m_bRet)
         return;


      m_puserstyleSelect = this;

      create_point_font(::user::font_list_item, "Tahoma", 11, 400);
      create_point_font(::user::font_list_hover, "Tahoma", 11, 400);


      _001UpdateColumns();
      _001OnUpdateItemCount();

      SetTimer(1234, 250, NULL);

      m_plistheader->m_iImageSpacing = 16;

   }

   void main_form_list::_001OnTimer(timer * ptimer)
   {

      if (ptimer->m_nIDEvent == 1234)
      {
         //return;

         if (GetTypedParent<main_frame>() == NULL)
         {

            return;

         }
         if (GetTypedParent<main_frame>()->m_pnotifyicon == NULL)
         {

            return;

         }
         try
         {

            __pointer(visual::icon) piconNew;


            if (piconNew.is_set())
            {

               GetTypedParent<main_frame>()->m_pnotifyicon->ModifyIcon(piconNew);
               //GetTypedParent<main_frame>()->m_workset.m_pappearance->m_picon = piconNew;

               //void * p = piconNew->m_iconmap[::size(256, 256)];
               //if (p == NULL)
               //{

               //   p = piconNew->m_picon;

               //}

               //GetTypedParent<main_frame>()->send_message(WM_SETICON, ICON_BIG, (LPARAM)p);
               //GetTypedParent<main_frame>()->send_message(WM_SETICON, ICON_SMALL, (LPARAM)p);

            }

         }
         catch (...)
         {

         }

      }


   }



   //void main_form_list::_001OnUpdateMediaTransfer(::message::message * pobj)
   //{


   //   SCAST_PTR(::user::command, pcmdui, pobj);

   //   pcmdui->Enable(TRUE);


   //}


   //void main_form_list::_001OnMediaTransfer(::message::message * pobj)
   //{


   //   UNREFERENCED_PARAMETER(pobj);


   //}


   bool main_form_list::keyboard_focus_OnSetFocus()
   {

      return true;

   }


   ::count main_form_list::_001GetItemCount()
   {

      synch_lock sl(get_document()->mutex());

      return commanda()->get_count();

   }



   array < command > * main_form_list::commanda()
   {

      return &Application.m_commanda;

   }


   void main_form_list::_001GetItemColor(::user::mesh_item * pitem)
   {

      simple_form_list_view::_001GetItemColor(pitem);

   }


   i32 main_form_list::_001GetDrawTextFlags(::user::list::EView eview)
   {

      if (m_eview == impact_icon)
      {

         return DT_TOP | DT_CENTER | DT_END_ELLIPSIS | DT_WORDBREAK | DT_NOPREFIX;

      }
      else
      {

         return DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX;

      }

   }

   void main_form_list::_001GetItemText(::user::mesh_item * pitem)
   {

      pitem->m_crText = ARGB(255, 0x07, 0x6d, 0x91);

      synch_lock sl(get_document()->mutex());

      if (pitem->m_iItem < 0 || pitem->m_iItem >= commanda()->get_count())
      {

         pitem->m_bOk = false;

         return;

      }

      if (pitem->m_iSubItem == subitem_line)
      {

         pitem->m_strText.Format("%d", commanda()->element_at(pitem->m_iItem).m_iIndex);

      }
      else if (pitem->m_iSubItem == subitem_command)
      {

         pitem->m_strText = commanda()->element_at(pitem->m_iItem).m_strCommand;

      }
      else if (pitem->m_iSubItem == subitem_state)
      {

         pitem->m_strText = commanda()->element_at(pitem->m_iItem).get_state_string();

      }
      else if (pitem->m_iSubItem == subitem_response)
      {

         pitem->m_strText = commanda()->element_at(pitem->m_iItem).m_strResponse;

      }
      else
      {

         pitem->m_strText.Empty();

      }

      pitem->m_bOk = true;

      return;

   }


   void main_form_list::_001SetItemText(::user::mesh_item * pitem)
   {


      pitem->m_bOk = true;

   }


   bool main_form_list::on_click(const ::user::item & item)
   {

      m_rangeSelection.clear();

      ::user::mesh::item_range r;

      r.m_iLowerBound = iItem;

      r.m_iUpperBound = iItem;

      m_rangeSelection.add_item(r);

      _001HideEditingControls();

      if (iSubItem == 1000)
      {

         if (m_pdialog.is_set() && m_pdialog->m_pframe.is_set() && m_pdialog->m_pframe->IsWindowVisible())
         {

            m_pdialog->m_iTask = iItem;

            m_pdialog->show();

         }

         return true;

      }

      if (iSubItem == 2000)
      {

         if (m_pdialog.is_null())
         {

            m_pdialog = canew(task_dialog(get_context_application()));

         }

         m_pdialog->m_iTask = iItem;

         m_pdialog->show();

         return true;

      }

      return false;

   }


   void   main_form_list::_001DrawSubItem(::user::draw_list_item * pdrawitem)
   {

      if (pdrawitem->m_iSubItem == subitem_spacing)
      {

         ::draw2d::savedc savedc(pdrawitem->m_pgraphics);

         pdrawitem->m_iListItem = -1;

         _001GetElementRect(pdrawitem, ::user::list::element_text);


         if (pdrawitem->m_bOk)
         {

            rect rScreen = pdrawitem->m_rectSubItem;

            ClientToScreen(rScreen);

            if (rScreen.contains(Session.m_ptCursor))
            {

               COLORREF crBackHover = _001GetColor(::user::color_background_hover, ARGB(40, 0, 0, 0));

               pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectSubItem, crBackHover);

            }


            COLORREF crPen = ARGB(255, 80, 80, 75);

            COLORREF crBrush;

            rect rect;

            rect = pdrawitem->m_rectSubItem;

            ::rect rAlign;

            rAlign = rect;

            rAlign.left++;

            int iMin = MAX(rect.size().get_minimum_dimension() / 3, 1);

            rect.left = 0;
            rect.top = 0;
            rect.right = iMin;
            rect.bottom = iMin;

            rect.offset(0, iMin / 3);

            rect.Align(align_left_center, rAlign);

            synch_lock sl(get_document()->mutex());

            if (pdrawitem->m_iItem < 0 || pdrawitem->m_iItem >= commanda()->get_count())
            {

               pdrawitem->m_bOk = false;

               return;

            }

            if (commanda()->element_at(pdrawitem->m_iItem).m_estate == command::state_skipped)
            {

               crBrush = ARGB(200, 100, 120, 255);

            }
            else if (commanda()->element_at(pdrawitem->m_iItem).m_estate == command::state_processed)
            {

               crBrush = ARGB(200, 100, 220, 120);

            }
            else if (commanda()->element_at(pdrawitem->m_iItem).m_estate == command::state_sent)
            {

               crBrush = ARGB(200, 220, 190, 120);

            }
            else
            {

               crBrush = ARGB(200, 220, 140, 130);

            }

            ::draw2d::pen_pointer pen(allocer());

            pen->create_solid(1.0, crPen);

            pdrawitem->m_pgraphics->SelectObject(pen);

            ::draw2d::brush_pointer brush(allocer());

            brush->create_solid(crBrush);

            pdrawitem->m_pgraphics->SelectObject(brush);

            pdrawitem->m_pgraphics->FillEllipse(rect);

         }

         return;

      }

      simple_form_list_view::_001DrawSubItem(pdrawitem);

   }

   i32 main_form_list::_001CalcItemHeight(int iBaseHeight)
   {

      return simple_form_list_view::_001CalcItemHeight(iBaseHeight) * 3/2;

   }

   void main_form_list::reset_items()
   {

      synch_lock slList(get_document()->mutex());

      auto & commanda = *this->commanda();

      for (auto & command : commanda)
      {

         command.m_estate = grbl_merge_tool::command::state_queue;

         command.m_strResponse.Empty();

      }

      _001EnsureVisible(0);

   }



   void main_form_list::_001DrawItem(::user::draw_list_item * pdrawitem)
   {

      simple_form_list_view::_001DrawItem(pdrawitem);

      ::draw2d::pen_pointer p(allocer());

      p->create_solid(4.0, ARGB(255, 0xf3, 0xf5, 0xf5));

      pdrawitem->m_pgraphics->SelectObject(p);

      rect rectClient;
      GetClientRect(rectClient);


      pdrawitem->m_pgraphics->MoveTo(rectClient.left, pdrawitem->m_rectItem.bottom-2);
      pdrawitem->m_pgraphics->LineTo(rectClient.right, pdrawitem->m_rectItem.bottom-2);


   }


} // namespace grbl_merge_tool





