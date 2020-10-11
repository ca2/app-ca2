#include "framework.h"


namespace windows
{

   ::file::path get_known_folder(REFKNOWNFOLDERID kfid);

}


namespace twitter_auto
{


   main_form_list::main_form_list(::aura::application * papp) :
      ::object(papp),
      ::twitter_auto::container(papp),
      simple_form_list_view(papp),
      m_piconUndefined(allocer()),
      m_piconRunning(allocer()),
      m_piconError(allocer()),
      m_piconFullFail(allocer()),
      m_i(get_context_application()),
      m_fontUsername(allocer())
   {
      m_iImageSpacing = 7;
      //m_bMorePlain = true;
      m_i.create(24, 24);

      m_bSortEnable = false;
      //m_ptaska->m_pformlist = this;
      m_iItemHeight = 24;
      m_iLeftMargin = 49;
      m_iTopMargin = 33;

      connect_command_probe("edit_undo", &main_form_list::_001OnUpdateEditUndo);
      connect_command("edit_undo", &main_form_list::_001OnEditUndo);
      connect_command_probe("edit_redo", &main_form_list::_001OnUpdateEditRedo);
      connect_command("edit_redo", &main_form_list::_001OnEditRedo);
      connect_command_probe("media_transfer", &main_form_list::_001OnUpdateMediaTransfer);
      connect_command("media_transfer", &main_form_list::_001OnMediaTransfer);


      connect_command_probe("task_add", &main_form_list::_001OnUpdateAddTask);
      connect_command("task_add", &main_form_list::_001OnAddTask);
      connect_command_probe("task_delete", &main_form_list::_001OnUpdateDeleteTask);
      connect_command("task_delete", &main_form_list::_001OnDeleteTask);


      connect_command_probe("file_import", &main_form_list::_001OnUpdateImport);
      connect_command("file_import", &main_form_list::_001OnImport);
      connect_command_probe("file_export", &main_form_list::_001OnUpdateExport);
      connect_command("file_export", &main_form_list::_001OnExport);


      connect_command_probe("task_start_all", &main_form_list::_001OnUpdateStartAllTasks);
      connect_command("task_start_all", &main_form_list::_001OnStartAllTasks);
      connect_command_probe("task_stop_all", &main_form_list::_001OnUpdateStopAllTasks);
      connect_command("task_stop_all", &main_form_list::_001OnStopAllTasks);

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

      //{


      //   /*      class ::user::control::descriptor control;

      //   control.m_bTransparent = true;
      //   control.m_typeinfo = System.type_info < ::user::check_box >();
      //   control.m_id = "check_box";
      //   control.m_iSubItem = 0;
      //   control.add_function(::user::control::function_check_box);
      //   control.set_type(::user::control_type_check_box);
      //   control.m_setValue[::check_checked] = "on";
      //   control.m_setValue[::check_unchecked] = "off";
      //   */
      //   auto pcolumn = new_list_column();

      //   pcolumn->m_iWidth = 48;
      //   pcolumn->m_iSubItem = 1000;
      //   //pcolumn->m_iControl = _001AddControl(control);
      //   pcolumn->m_uiText = "";
      //   pcolumn->m_bCustomDraw = true;
      //   _001AddColumn(column);

      //   pcolumn->m_iControl = -1;

      //   //class ::user::control::descriptor control;

      //   //control.set_type(user::control_type_edit_plain_text);
      //   ////control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
      //   ////pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
      //   //control.set_data_type(user::control::DataTypeString);
      //   //control.add_function(user::control::function_vms_data_edit);
      //   //control.m_typeinfo = System.type_info < ::user::plain_edit >();
      //   //////control.m_typeinfo = __pointer(type)();
      //   //control.m_iSubItem = 0;
      //   //control.m_id = 1000;
      //   //index iControl = _001AddControl(control);

      //   //auto pcolumn = new_list_column();

      //   //pcolumn->m_iSubItem = 0;
      //   //pcolumn->m_iWidth = 100;
      //   //pcolumn->m_uiText = "Status";
      //   //pcolumn->m_iControl = iControl;

      //   //_001AddColumn(column);

      {
         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 45;
         pcolumn->m_iSubItem = 9003;
         pcolumn->m_uiText = "";
         pcolumn->m_bCustomDraw = true;
         ::visual::dib_pointer dib(allocer());
         dib.load_from_file("matter://statusicon.png");
         pcolumn->m_dibHeader = dib;
         _001AddColumn(column);

         pcolumn->m_iControl = -1;


      }

      //}
      {


         /*      class ::user::control::descriptor control;

               control.m_bTransparent = true;
               control.m_typeinfo = System.type_info < ::user::check_box >();
               control.m_id = "check_box";
               control.m_iSubItem = ::twitter_auto::user::data_status;
               control.add_function(::user::control::function_check_box);
               control.set_type(::user::control_type_check_box);
               control.m_setValue[::check_checked] = "on";
               control.m_setValue[::check_unchecked] = "off";
         */
         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 150;
         pcolumn->m_iSubItem = twitter_auto::user::data_status;
         //pcolumn->m_iControl = _001AddControl(control);
         pcolumn->m_uiText = "STATUS";
         pcolumn->m_bCustomDraw = true;
         _001AddColumn(column);

         pcolumn->m_iControl = -1;

         //class ::user::control::descriptor control;

         //control.set_type(user::control_type_edit_plain_text);
         ////control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         ////pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         //control.set_data_type(user::control::DataTypeString);
         //control.add_function(user::control::function_vms_data_edit);
         //control.m_typeinfo = System.type_info < ::user::plain_edit >();
         //////control.m_typeinfo = __pointer(type)();
         //control.m_iSubItem = ::twitter_auto::user::data_status;
         //control.m_id = 1000;
         //index iControl = _001AddControl(control);

         //auto pcolumn = new_list_column();

         //pcolumn->m_iSubItem = ::twitter_auto::user::data_status;
         //pcolumn->m_iWidth = 100;
         //pcolumn->m_uiText = "Status";
         //pcolumn->m_iControl = iControl;

         //_001AddColumn(column);


      }
      //{


      //   class ::user::control::descriptor control;

      //   control.m_bTransparent = true;
      //   //control.m_typeinfo = System.type_info < ::user::check_box >();
      //   control.m_id = "check_box_same_consumer";
      //   control.m_iSubItem = 12;
      //   control.add_function(::user::control::function_check_box);
      //   control.set_type(::user::control_type_check_box);
      //   control.m_setValue[::check_checked] = "on";
      //   control.m_setValue[::check_unchecked] = "off";
      //
      //   auto pcolumn = new_list_column();

      //   pcolumn->m_iWidth = 32;
      //   pcolumn->m_iSubItem = 12;
      //   pcolumn->m_iControl = _001AddControl(control);
      //   pcolumn->m_uiText = "Same Consumer";
      //   pcolumn->m_bCustomDraw = true;
      //   _001AddColumn(column);

      //   pcolumn->m_iControl = -1;

      //   //class ::user::control::descriptor control;

      //   //control.set_type(user::control_type_edit_plain_text);
      //   ////control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
      //   ////pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
      //   //control.set_data_type(user::control::DataTypeString);
      //   //control.add_function(user::control::function_vms_data_edit);
      //   //control.m_typeinfo = System.type_info < ::user::plain_edit >();
      //   //////control.m_typeinfo = __pointer(type)();
      //   //control.m_iSubItem = ::twitter_auto::user::data_status;
      //   //control.m_id = 1000;
      //   //index iControl = _001AddControl(control);

      //   //auto pcolumn = new_list_column();

      //   //pcolumn->m_iSubItem = ::twitter_auto::user::data_status;
      //   //pcolumn->m_iWidth = 100;
      //   //pcolumn->m_uiText = "Status";
      //   //pcolumn->m_iControl = iControl;

      //   //_001AddColumn(column);


      //}
      //{


      //   class ::user::control::descriptor control;

      //   control.m_bTransparent = true;
      //   //control.m_typeinfo = System.type_info < ::user::check_box >();
      //   control.m_id = "check_box_same_proxy";
      //   control.m_iSubItem = 13;
      //   control.add_function(::user::control::function_check_box);
      //   control.set_type(::user::control_type_check_box);
      //   control.m_setValue[::check_checked] = "on";
      //   control.m_setValue[::check_unchecked] = "off";

      //   auto pcolumn = new_list_column();

      //   pcolumn->m_iWidth = 32;
      //   pcolumn->m_iSubItem = 13;
      //   pcolumn->m_iControl = _001AddControl(control);
      //   pcolumn->m_uiText = "Same Proxy";
      //   pcolumn->m_bCustomDraw = true;
      //   _001AddColumn(column);

      //   pcolumn->m_iControl = -1;

      //   //class ::user::control::descriptor control;

      //   //control.set_type(user::control_type_edit_plain_text);
      //   ////control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
      //   ////pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
      //   //control.set_data_type(user::control::DataTypeString);
      //   //control.add_function(user::control::function_vms_data_edit);
      //   //control.m_typeinfo = System.type_info < ::user::plain_edit >();
      //   //////control.m_typeinfo = __pointer(type)();
      //   //control.m_iSubItem = ::twitter_auto::user::data_status;
      //   //control.m_id = 1000;
      //   //index iControl = _001AddControl(control);

      //   //auto pcolumn = new_list_column();

      //   //pcolumn->m_iSubItem = ::twitter_auto::user::data_status;
      //   //pcolumn->m_iWidth = 100;
      //   //pcolumn->m_uiText = "Status";
      //   //pcolumn->m_iControl = iControl;

      //   //_001AddColumn(column);


      //}
      {

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = twitter_auto::user::data_hint_username;
         pcolumn->m_iWidth = 200;
         pcolumn->m_uiText = "USERNAME";
         pcolumn->m_pil = &m_i;

         _001AddColumn(column);


      }
      {

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = twitter_auto::user::data_target;
         pcolumn->m_iWidth = 200;
         pcolumn->m_uiText = "TARGET";
         pcolumn->m_pil = &m_i;

         _001AddColumn(column);


      }
      {
         //class ::user::control::descriptor control;

         //control.set_type(user::control_type_button);
         ////control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         ////pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         //control.m_typeinfo = System.type_info < ::user::button >();
         //////control.m_typeinfo = __pointer(type)();
         //control.m_iSubItem = 2;
         //control.m_id = 1000;
         //control.add_function(user::control::function_action);
         //control.m_iSubItemDuplicateCheckBox = 12;
         //control.m_iaSubItemDuplicate.add(6);
         //index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 2000;
         pcolumn->m_iWidth = 150;
         pcolumn->m_uiText = "SETTINGS";
         //pcolumn->m_iControl = iControl;

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

      if (lHint == document::update_import)
      {

         m_ptaska->backup();

         m_ptaska->simple_csv_addup_file(get_document()->m_filepath);

         m_ptaska->save();

         _001OnUpdateItemCount();

         //GetTypedParent < pane_view >()->set_cur_tab_by_id(PaneViewTwitterAutomator);

      }
      else if (lHint == document::update_export)
      {

         m_ptaska->simple_csv_save_file(get_document()->m_filepath);

      }


      //

      //   //stringa straLines;

      //   //if (str.has_char())
      //   //{

      //   //   straLines.add_lines(str);

      //   //}

      //   //while (straLines.get_size() < 100)
      //   //{

      //   //   str.Format("Item %d", straLines.get_size() + 1);

      //   //   straLines.add(str);

      //   //}

      //   //m_straName.set_size(straLines.get_size());
      //   //m_straOption.set_size(straLines.get_size());

      //   //stringa straLine;

      //   //for (index i = 0; i < straLines.get_size(); i++)
      //   //{

      //   //   straLine.explode(";", straLines[i]);

      //   //   m_straName[i] = straLine[0];

      //   //   if (straLine.get_count() > 1)
      //   //   {

      //   //      m_straOption[i] = straLine[1];

      //   //   }
      //   //   else
      //   //   {

      //   //      m_straOption[i] = "Option 1";

      //   //   }

      //   //}


      //}
      //else if (lHint == update_save)
      //{

      //   // __pointer(control) pcontrol = pupdate;

      //   /*string str;

      //   for (index i = 0; i < m_straName.get_size(); i++)
      //   {

      //      if (i > 0)
      //      {

      //         str += "\r\n";

      //      }

      //      str += m_straName[i];
      //      str += ";";
      //      str += m_straOption[i];

      //   }*/

//         m_ptaska->simple_csv_save_file(Context.dir().home() / "Twitter Auto.csv");

      //}

   }

   void main_form_list::_001OnContextMenu(::message::message * pobj)
   {

      track_popup_xml_matter_menu("devedge_contextmenu.xml", 0, pobj);

   }
   void main_form_list::_001OnUpdateImport(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnImport(::message::message * pobj)
   {

      pobj->m_bRet = true;

      ::fork(get_context_application(), [&]()
      {

         string str;

         data_get("last_import_folder", str);

         if (str.is_empty())
         {

            str = Context.dir().home() / "Twitter Auto";

            if (!Context.dir().is(str))
            {

               str = ::windows::get_known_folder(FOLDERID_Desktop);

               if (!Context.dir().is(str))
               {

                  str = "";

               }

            }


         }

         property_set set;

         set["file_filter_specs"] = "*.csv";
         set["file_filter_names"] = "Image Files";
         set["folder"] = str;

         if (!Context.os().browse_file_open(get_handle(), set))
         {

            return;
            //::twitter_auto::get_file_open(GetTopLevelFrame(), str);

         }

         file::path path = set["file_name"];

         if (path.is_empty())
         {

            return;

         }

         data_set("last_import_folder", path.folder());

         get_document()->m_filepath = path;

         get_document()->update_all_views(NULL, document::update_import);

      });

   }

   void main_form_list::_001OnUpdateExport(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnExport(::message::message * pobj)
   {

      pobj->m_bRet = true;

      ::fork(get_context_application(), [&]()
      {

         string str;

         data_get("last_export_folder", str);

         if (str.is_empty())
         {

            str = ::windows::get_known_folder(FOLDERID_Desktop);

            if (!Context.dir().is(str))
            {

               str = "";

            }

         }
         property_set set;

         set["file_filter_specs"] = "*.csv";
         set["file_filter_names"] = "Image Files";
         set["folder"] = str;
         set["default_file_extension"] = "csv";

         if (!Context.os().browse_file_save(get_handle(), set))
         {

            return;
            //file::path path = ::twitter_auto::get_file_save(GetTopLevelFrame(), str);


         }

         file::path path = set["file_name"];

         if (path.is_empty())
         {

            return;

         }

         if (path.extension().is_empty())
         {

            path += ".csv";

         }

         data_set("last_export_folder", path.folder());

         get_document()->m_filepath = path;

         get_document()->update_all_views(NULL, document::update_export);

      });

   }

   void main_form_list::_001OnUpdateStartAllTasks(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnStartAllTasks(::message::message * pobj)
   {

      m_ptaska->start_all_tasks();


   }

   void main_form_list::_001OnUpdateStopAllTasks(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnStopAllTasks(::message::message * pobj)
   {
      m_ptaska->stop_all_tasks();
   }

   void main_form_list::_001OnUpdateAddTask(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnAddTask(::message::message * pobj)
   {

      {

         synch_lock sl(m_ptaska->mutex());

         m_ptaska->add(canew(::twitter_auto::user(m_ptaska)));

         m_ptaska->last().simple_csv_get_line(m_ptaska->m_stra.element_at_grow(m_ptaska->get_upper_bound()));

      }

      _001OnUpdateItemCount();

      pobj->m_bRet = true;

   }


   void main_form_list::_001OnUpdateDeleteTask(::message::message * pobj)
   {
      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(_001GetSelectedItemCount() > 0);
   }
   void main_form_list::_001OnDeleteTask(::message::message * pobj)
   {
      //SCAST_PTR(::user::command, pcmdui, pobj);
      if (_001GetSelectedItemCount() > 0)
      {
         index_array ia;
         _001GetSelectedItems(ia);
         ::sort::quick_sort(ia, false);
         for (index i = 0; i < ia.get_size(); i++)
         {
            m_ptaska->delete_task(ia[i]);
         }
         _001OnUpdateItemCount();
      }
      pobj->m_bRet = true;
   }


   void main_form_list::_001OnUpdateEditUndo(::message::message * pobj)
   {
      SCAST_PTR(::user::command, pcmdui, pobj);
      //pcmdui->Enable(m_ptree->m_editfile.CanUndo());
   }

   void main_form_list::_001OnUpdateEditRedo(::message::message * pobj)
   {
      SCAST_PTR(::user::command, pcmdui, pobj);
      //pcmdui->Enable(m_ptree->m_editfile.GetRedoBranchCount() > 0);
   }

   void main_form_list::_001OnEditUndo(::message::message * pobj)
   {


      UNREFERENCED_PARAMETER(pobj);

      //Undo();


   }


   void main_form_list::_001OnEditRedo(::message::message * pobj)
   {


      UNREFERENCED_PARAMETER(pobj);

      //Redo();


   }

   /*   void main_form_list::_001GetItemText(::user::mesh_item * pitem)
      {
      }*/

   void main_form_list::_001GetItemImage(::user::mesh_item * pitem)
   {

      if (pitem->m_iSubItem == ::twitter_auto::user::data_hint_username)
      {

         pitem->m_bOk = true;

         pitem->m_iImage = 0;

         return;

      }
      else if (pitem->m_iSubItem == ::twitter_auto::user::data_target)
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
      IGUI_MSG_LINK(WM_CONTEXTMENU, pchannel, this, &main_form_list::_001OnContextMenu);


   }


   void main_form_list::_001OnCreate(::message::message * pobj)
   {


      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      if (pcreate->m_bRet)
         return;

      //m_font->create_point_font("Tahoma", 10, 800);

      //m_fontHover->create_point_font("Tahoma", 10, 800);

      //m_plistheader->m_font->create_point_font("Tahoma", 10, 800);

      m_fontUsername->create_point_font("Tahoma", 11, 400);



      m_ptaska->thread::begin();

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

            if (m_ptaska->get_active_count() == m_ptaska->get_count())
            {

               if (m_ptaska->get_count() == 0)
               {


                  piconNew = m_piconUndefined;

               }
               else
               {

                  piconNew = m_piconRunning;
               }

            }
            else if (m_ptaska->get_active_count() < m_ptaska->get_count())
            {

               if (m_ptaska->get_active_count() == 0)
               {

                  piconNew = m_piconFullFail;

               }
               else
               {

                  piconNew = m_piconError;

               }

            }

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



   void main_form_list::_001OnUpdateMediaTransfer(::message::message * pobj)
   {


      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(TRUE);


   }


   void main_form_list::_001OnMediaTransfer(::message::message * pobj)
   {


      UNREFERENCED_PARAMETER(pobj);


   }


   bool main_form_list::keyboard_focus_OnSetFocus()
   {

      return true;

   }


   ::count main_form_list::_001GetItemCount()
   {

      synch_lock sl(m_ptaska->mutex());

      return m_ptaska->m_stra.get_count();

   }


   void main_form_list::_001GetItemColor(::user::mesh_item * pitem)
   {

      if (pitem->m_iSubItem == ::twitter_auto::user::data_source_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

         pitem->m_bOk = true;

         return;

      }

      if (pitem->m_iSubItem == ::twitter_auto::user::data_target_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

         pitem->m_bOk = true;

         return;

      }

      if (pitem->m_iSubItem == ::twitter_auto::user::data_target_api_token_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

         pitem->m_bOk = true;

         return;

      }

      return simple_form_list_view::_001GetItemColor(pitem);

   }


   ::e_align main_form_list::get_draw_text_align(::user::list::EView eview)
   {

      if (m_eview == impact_icon)
      {

         return e_align_top_center;

      }
      else
      {

         return e_align_left_center;

      }

   }


   ::e_draw_text main_form_list::get_draw_text_flags(::user::list::EView eview)
   {

      if (m_eview == impact_icon)
      {

         return e_draw_text_end_ellipsis | e_draw_text_no_prefix | e_draw_text_word_break;

      }
      else
      {

         return e_draw_text_end_ellipsis | e_draw_text_no_prefix | e_draw_text_single_line;

      }

   }


      void main_form_list::_001GetItemText(::user::mesh_item * pitem)
   {

      if (pitem->m_iItem >= m_ptaska->m_stra.get_count())
      {
         pitem->m_bOk = false;

         return;

      }

      if (pitem->m_iSubItem == 2000)
      {

         pitem->m_strText = "SETTINGS";

         pitem->m_crText = ARGB(255, 0x07, 0x6d, 0x91);

         pitem->m_bOk = true;

         return;

      }

      if (pitem->m_iSubItem == ::twitter_auto::user::data_hint_username)
      {

         ::user::draw_list_item * p = dynamic_cast < ::user::draw_list_item * >(pitem);

         p->m_pgraphics->SelectObject(m_fontUsername);

      }
      else if (pitem->m_iSubItem == ::twitter_auto::user::data_target)
      {

         ::user::draw_list_item * p = dynamic_cast < ::user::draw_list_item * >(pitem);

         p->m_pgraphics->SelectObject(m_fontUsername);

      }

      if (pitem->m_iSubItem == ::twitter_auto::user::data_source_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

      }
      else if (pitem->m_iSubItem == ::twitter_auto::user::data_target_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

      }
      else if (pitem->m_iSubItem == ::twitter_auto::user::data_target_api_token_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

      }
      else
      {

         pitem->m_crText = ARGB(200, 80, 80, 80);

      }

      synch_lock sl(m_ptaska->mutex());

      if (pitem->m_iItem == m_ptaska->m_stra.get_count())
      {

         pitem->m_strText = "Click Add button to add new Automator";

         pitem->m_bOk = true;

         return;

      }

      pitem->m_strText = m_ptaska->m_stra[pitem->m_iItem].safe_at(pitem->m_iSubItem);

      if (pitem->m_iSubItem == ::twitter_auto::user::data_status)
      {

         if (pitem->m_strText.compare_ci("ON") == 0)
         {

            pitem->m_crText = ARGB(255, 120, 210, 150);

         }
         else
         {

            pitem->m_crText = ARGB(255, 210, 150, 120);

         }

      }
      else
      {

         pitem->m_crText = ARGB(255, 100, 100, 100);

      }


      pitem->m_bOk = true;

      return;

   }


   void main_form_list::_001SetItemText(::user::mesh_item * pitem)
   {

      {

         synch_lock sl(m_ptaska->mutex());

         if (pitem->m_iItem == m_ptaska->m_stra.get_count())
         {

            pitem->m_bOk = false;

            return;

         }

         m_ptaska->set_item_text(pitem->m_iItem, pitem->m_iSubItem, pitem->m_strText);

      }

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

            m_pdialog->m_ptaska = m_ptaska;

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

         m_pdialog->m_ptaska = m_ptaska;

         m_pdialog->m_iTask = iItem;

         m_pdialog->show();

         return true;

      }

      if (iSubItem == ::twitter_auto::user::data_status)
      {

         ::user::draw_list_item item(this);

         item.m_iSubItem = ::twitter_auto::user::data_status;

         item.m_iItem = iItem;

         try
         {

            _001GetItemText(&item);

         }
         catch (...)
         {

            item.m_bOk = false;

         }

         if (item.m_bOk)
         {

            if (item.m_strText.compare_ci("ON") == 0)
            {

               item.m_strText = "off";

               _001SetItemText(&item);

            }
            else
            {

               item.m_strText = "on";

               _001SetItemText(&item);

            }

         }


         _001HideEditingControls();

         if (m_pdialog.is_set() && m_pdialog->m_pframe.is_set() && m_pdialog->m_pframe->IsWindowVisible())
         {

            m_pdialog->m_ptaska = m_ptaska;

            m_pdialog->m_iTask = iItem;

            m_pdialog->show();

         }


         return true;

      }

      if (m_pdialog.is_set() && m_pdialog->m_pframe.is_set() && m_pdialog->m_pframe->IsWindowVisible())
      {

         m_pdialog->m_ptaska = m_ptaska;

         m_pdialog->m_iTask = iItem;

         m_pdialog->show();

      }

      return true;
      return simple_form_list_view::on_click(item);

   }


   void   main_form_list::_001DrawSubItem(::user::draw_list_item * pdrawitem)
   {

      if (pdrawitem->m_iSubItem == 1000)
      {

         pdrawitem->m_iListItem = -1;

         _001GetElementRect(pdrawitem, ::user::list::element_text);

         try
         {

            _001GetItemText(pdrawitem);

         }
         catch (...)
         {

            pdrawitem->m_bOk = false;

         }

         if (pdrawitem->m_bOk)
         {

            rect rScreen = pdrawitem->m_rectSubItem;

            ClientToScreen(rScreen);

            if (rScreen.contains(Session.m_ptCursor))
            {

               COLORREF crBackHover = _001GetColor(::user::color_background_hover, ARGB(40, 0, 0, 0));

               pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectSubItem, crBackHover);

            }

            ::draw2d::brush_pointer brush(allocer());

            brush->create_solid(ARGB(255, 0, 0, 0));

            pdrawitem->m_pgraphics->SelectObject(brush);

            rect rect;

            rect = pdrawitem->m_rectSubItem;

            ::rect r;

            r.left = 0;

            r.top = 0;

            r.right = 8;

            r.bottom = 8;

            r.Align(::align_center, rect);

            pdrawitem->m_pgraphics->FillEllipse(r);

         }

         return;

      }
      else if (pdrawitem->m_iSubItem == ::twitter_auto::user::data_status)
      {

         pdrawitem->m_iListItem = -1;

         _001GetElementRect(pdrawitem, ::user::list::element_text);

         try
         {

            _001GetItemText(pdrawitem);

         }
         catch (...)
         {

            pdrawitem->m_bOk = false;

         }

         if (pdrawitem->m_bOk)
         {
            rect rScreen = pdrawitem->m_rectSubItem;

            ClientToScreen(rScreen);

            if (rScreen.contains(Session.m_ptCursor))
            {

               COLORREF crBackHover = _001GetColor(::user::color_background_hover, ARGB(40, 0, 0, 0));

               pdrawitem->m_pgraphics->FillSolidRect(pdrawitem->m_rectSubItem, crBackHover);

            }


            bool bOk = pdrawitem->m_strText.compare_ci("ON") == 0;

            if (bOk)
            {

               pdrawitem->m_strText = "VALID";

            }
            else
            {

               pdrawitem->m_strText = "PENDING";

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

            if (bOk)
            {

               crBrush = ARGB(200, 100, 220, 120);

            }
            else
            {

               crBrush = ARGB(200, 220, 120, 100);

            }

            ::draw2d::pen_pointer pen(allocer());

            pen->create_solid(1.0, crPen);

            pdrawitem->m_pgraphics->SelectObject(pen);

            ::draw2d::brush_pointer brush(allocer());

            brush->create_solid(crBrush);

            pdrawitem->m_pgraphics->SelectObject(brush);

            pdrawitem->m_pgraphics->FillEllipse(rect);

            pdrawitem->m_rectText.left = rect.right + m_iImageSpacing;

            pdrawitem->draw_text();

         }

         return;

      }

      simple_form_list_view::_001DrawSubItem(pdrawitem);

   }

   i32 main_form_list::_001CalcItemHeight(int iBaseHeight)
   {

      return simple_form_list_view::_001CalcItemHeight(iBaseHeight) * 3/2;

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


} // namespace twitter_auto





