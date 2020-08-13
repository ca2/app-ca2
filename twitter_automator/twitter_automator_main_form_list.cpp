#include "framework.h"


namespace twitter_automator
{


   main_form_list::main_form_list(::aura::application * papp) :
      ::object(papp),
      container(papp),
      m_piconUndefined(allocer()),
      m_piconRunning(allocer()),
      m_piconError(allocer()),
      m_piconFullFail(allocer())
   {

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

      m_piconUndefined->load_matter("main/undefined.ico");
      m_piconRunning->load_matter("main/icon.ico");
      m_piconError->load_matter("main/error.ico");
      m_piconFullFail->load_matter("main/full_fail.ico");



   }

   main_form_list::~main_form_list()
   {
   }




   void main_form_list::_001InsertColumns()
   {

      {


         /*      class ::user::control::descriptor control;

         control.m_bTransparent = true;
         control.m_typeinfo = System.type_info < ::user::check_box >();
         control.m_id = "check_box";
         control.m_iSubItem = ::twitter_automator::task::data_status;
         control.add_function(::user::control::function_check_box);
         control.set_type(::user::control_type_check_box);
         control.m_setValue[::check_checked] = "on";
         control.m_setValue[::check_unchecked] = "off";
         */
         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 48;
         pcolumn->m_iSubItem = 1000;
         //pcolumn->m_iControl = _001AddControl(control);
         pcolumn->m_uiText = "";
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
         //control.m_iSubItem = 0;
         //control.m_id = 1000;
         //index iControl = _001AddControl(control);

         //auto pcolumn = new_list_column();

         //pcolumn->m_iSubItem = 0;
         //pcolumn->m_iWidth = 100;
         //pcolumn->m_uiText = "Status";
         //pcolumn->m_iControl = iControl;

         //_001AddColumn(column);


      }
      {


         /*      class ::user::control::descriptor control;

               control.m_bTransparent = true;
               control.m_typeinfo = System.type_info < ::user::check_box >();
               control.m_id = "check_box";
               control.m_iSubItem = ::twitter_automator::task::data_status;
               control.add_function(::user::control::function_check_box);
               control.set_type(::user::control_type_check_box);
               control.m_setValue[::check_checked] = "on";
               control.m_setValue[::check_unchecked] = "off";
         */
         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 48;
         pcolumn->m_iSubItem = ::twitter_automator::task::data_status;
         //pcolumn->m_iControl = _001AddControl(control);
         pcolumn->m_uiText = "Status";
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
         //control.m_iSubItem = 0;
         //control.m_id = 1000;
         //index iControl = _001AddControl(control);

         //auto pcolumn = new_list_column();

         //pcolumn->m_iSubItem = 0;
         //pcolumn->m_iWidth = 100;
         //pcolumn->m_uiText = "Status";
         //pcolumn->m_iControl = iControl;

         //_001AddColumn(column);


      }
      {


         class ::user::control_descriptor control;

         control.m_bTransparent = true;
         //control.m_typeinfo = System.type_info < ::user::check_box >();
         control.m_id = "check_box_same_consumer";
         //control.m_iSubItem = ::twitter_automator::task::data_same_consumer;
         control.add_function(::user::control_function_check_box);
         control.set_control_type(::user::control_type_check_box);
         control.m_setValue[::check_checked] = "on";
         control.m_setValue[::check_unchecked] = "off";

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 32;
         //pcolumn->m_iSubItem = ::twitter_automator::task::data_same_consumer;
         pcolumn->m_iControl = _001AddControl(control);
         pcolumn->m_uiText = "Same Consumer";
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
         //control.m_iSubItem = 0;
         //control.m_id = 1000;
         //index iControl = _001AddControl(control);

         //auto pcolumn = new_list_column();

         //pcolumn->m_iSubItem = 0;
         //pcolumn->m_iWidth = 100;
         //pcolumn->m_uiText = "Status";
         //pcolumn->m_iControl = iControl;

         //_001AddColumn(column);


      }
      {


         class ::user::control_descriptor control;

         control.m_bTransparent = true;
         //control.m_typeinfo = System.type_info < ::user::check_box >();
         control.m_id = "check_box_same_proxy";
         control.m_iSubItem = 13;
         control.add_function(::user::control_function_check_box);
         control.set_control_type(::user::control_type_check_box);
         control.m_setValue[::check_checked] = "on";
         control.m_setValue[::check_unchecked] = "off";

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 32;
         pcolumn->m_iSubItem = 13;
         pcolumn->m_iControl = _001AddControl(control);
         pcolumn->m_uiText = "Same Proxy";
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
         //control.m_iSubItem = 0;
         //control.m_id = 1000;
         //index iControl = _001AddControl(control);

         //auto pcolumn = new_list_column();

         //pcolumn->m_iSubItem = 0;
         //pcolumn->m_iWidth = 100;
         //pcolumn->m_uiText = "Status";
         //pcolumn->m_iControl = iControl;

         //_001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 1;
         control.m_id = 1000;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 1;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Source";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 2;
         control.m_id = 1000;
         control.add_function(user::control_function_duplicate_on_check_box);
         control.m_iSubItemDuplicateCheckBox = 12;
         control.m_iaSubItemDuplicate.add(6);
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 2;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Consumer Key";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 3;
         control.m_id = 1000;
         control.add_function(user::control_function_duplicate_on_check_box);
         control.m_iSubItemDuplicateCheckBox = 12;
         control.m_iaSubItemDuplicate.add(7);
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 3;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Consumer Secret";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 4;
         control.m_id = 1000;
         control.add_function(user::control_function_duplicate_on_check_box);
         control.m_iSubItemDuplicateCheckBox = 13;
         control.m_iaSubItemDuplicate.add(10);
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 4;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Source Proxy";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 5;
         control.m_id = 1000;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 5;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Target Hint";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 6;
         control.m_id = 1000;
         control.add_function(user::control_function_disable_on_check_box);
         control.m_iSubItemDisableCheckBox = 12;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 6;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Target Consumer Key";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.add_function(user::control_function_disable_on_check_box);
         control.m_iSubItemDisableCheckBox = 12;
         control.m_iSubItem = 7;
         control.m_id = 1000;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 7;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Target Consumer Secret";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 8;
         control.m_id = 1000;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 8;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Token Key";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 9;
         control.m_id = 1000;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 9;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Token Consumer Secret";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }

      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_edit_plain_text);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_vms_data_edit);
         control.m_typeinfo = System.type_info < ::user::plain_edit >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 10;
         control.m_id = 1000;
         control.add_function(user::control_function_disable_on_check_box);
         control.m_iSubItemDisableCheckBox = 13;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 10;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Target Proxy";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);


      }
      {
         class ::user::control_descriptor control;

         control.set_control_type(user::control_type_combo_box);
         //control.m_dataid = "FILE_MANAGER_ID_FILE_NAME";
         //pcontrol->descriptor().m_id = _vms::FILE_MANAGER_ID_FILE_NAME;
         control.set_data_type(user::control_data_type_string);
         control.add_function(user::control_function_data_selection);
         control.m_typeinfo = System.type_info < ::user::combo_box >();
         ////control.m_typeinfo = __pointer(type)();
         control.m_iSubItem = 11;
         control.m_id = 1001;
         index iControl = _001AddControl(control);

         auto pcolumn = new_list_column();

         pcolumn->m_iSubItem = 11;
         pcolumn->m_iWidth = 100;
         pcolumn->m_uiText = "Copy Mode";
         pcolumn->m_iControl = iControl;

         _001AddColumn(column);

      }


   }

   bool main_form_list::get_color(COLORREF & cr, ::user::e_color ecolor, ::user::style_context * pcontext)
   {

      if (ecolor == ::user::color_background)
      {

         cr = ARGB(255, 255, 255, 255);

         return true;

      }

      return user::form_list_view::get_color(cr, ecolor, pcontext);

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

      if (pcontrol->descriptor().m_id == 1001)
      {

         __pointer(::user::combo_box) pcombobox = pcontrol;

         if (pcombobox.is_set())
         {

            if (pcombobox->m_straList.get_size() != 3)
            {
               pcombobox->m_bEdit = false;
               //pcombobox->AddString("Copy One Link", 1);
               pcombobox->AddString("Copy Link", 1);
               pcombobox->AddString("Copy Photos, Text and Videos", 2);
               pcombobox->AddString("Copy Text", 3);
               pcombobox->AddString("Copy All", 4);

            }

         }

      }
      else if (pcontrol->descriptor().m_id == "status_check_box")
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

      ::user::form_list_view::on_update(pSender, lHint, pupdate);

      if (lHint == document::update_import)
      {

         m_ptaska->backup();

         m_ptaska->simple_csv_load_file(get_document()->m_filepath);

         m_ptaska->save();

         _001OnUpdateItemCount();

         GetTypedParent < pane_view >()->set_cur_tab_by_id(PaneViewTwitterAutomator);

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

//         m_ptaska->simple_csv_save_file(Context.dir().home() / "Twitter Automator.csv");

      //}

   }

   void main_form_list::_001OnContextMenu(::message::message * pobj)
   {

      track_popup_xml_matter_menu("devedge_contextmenu.xml", 0, pobj);

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

         m_ptaska->add(canew(task(m_ptaska)));

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


      ::user::form_list_view::install_message_routing(pchannel);


      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &main_form_list::_001OnCreate);
      IGUI_MSG_LINK(WM_CONTEXTMENU, pchannel, this, &main_form_list::_001OnContextMenu);


   }


   void main_form_list::_001OnCreate(::message::message * pobj)
   {


      SCAST_PTR(::message::create, pcreate, pobj);

      pcreate->previous();

      if (pcreate->m_bRet)
         return;

      //m_font->create_point_font("Verdana", 12);

      //m_fontHover->create_point_font("Verdana", 12);

      m_ptaska->thread::begin();

      _001UpdateColumns();
      _001OnUpdateItemCount();

      SetTimer(1234, 250, NULL);

   }

   void main_form_list::_001OnTimer(timer * ptimer)
   {

      if (ptimer->m_nIDEvent == 1234)
      {

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

            if (m_ptaska->get_active_count() == m_ptaska->get_count())
            {

               if (m_ptaska->get_count() == 0)
               {

                  GetTypedParent<main_frame>()->m_pnotifyicon->ModifyIcon(m_piconUndefined);

               }
               else
               {

                  GetTypedParent<main_frame>()->m_pnotifyicon->ModifyIcon(m_piconRunning);
               }

            }
            else if (m_ptaska->get_active_count() < m_ptaska->get_count())
            {

               if (m_ptaska->get_active_count() == 0)
               {

                  GetTypedParent<main_frame>()->m_pnotifyicon->ModifyIcon(m_piconFullFail);

               }
               else
               {

                  GetTypedParent<main_frame>()->m_pnotifyicon->ModifyIcon(m_piconError);

               }



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

      if (pitem->m_iSubItem == task::data_source_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

         pitem->m_bOk = true;

         return;

      }

      if (pitem->m_iSubItem == task::data_target_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

         pitem->m_bOk = true;

         return;

      }

      if (pitem->m_iSubItem == task::data_target_api_token_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

         pitem->m_bOk = true;

         return;

      }

      return ::user::form_list_view::_001GetItemColor(pitem);

   }


   void main_form_list::_001GetItemText(::user::mesh_item * pitem)
   {

      if (pitem->m_iSubItem == task::data_source_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

      }
      else if (pitem->m_iSubItem == task::data_target_api_consumer_sec)
      {

         pitem->m_crText = ARGB(0, 0, 0, 0);

      }
      else if (pitem->m_iSubItem == task::data_target_api_token_sec)
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

      pitem->m_bOk = true;

      return;

   }


   void main_form_list::_001SetItemText(::user::mesh_item * pitem)
   {

      {

         synch_lock sl(m_ptaska->mutex());

         if (pitem->m_iItem == m_ptaska->m_stra.get_count())
         {
            pitem->m_bOk = true;

            return;

         }

         m_ptaska->set_item_text(pitem->m_iItem, pitem->m_iSubItem, pitem->m_strText);

      }

      pitem->m_bOk = true;

      return;

   }

   bool main_form_list::on_click(const ::user::item & item)
   {

      if (iSubItem == 1000)
      {

         m_rangeSelection.clear();

         ::user::mesh::item_range r;

         r.m_iLowerBound = iItem;

         r.m_iUpperBound = iItem;

         m_rangeSelection.add_item(r);

         _001HideEditingControls();

         return true;

      }

      if (iSubItem == 0)
      {

         ::user::draw_list_item item(this);

         item.m_iSubItem = 0;
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

         return true;

      }

      return ::user::form_list_view::on_click(item);

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

               pdrawitem->m_pgraphics->fill_solid_rect(pdrawitem->m_rectSubItem, crBackHover);

            }

            bool bOk = pdrawitem->m_strText == "on";

            COLORREF crPen = ARGB(255, 80, 80, 75);

            //COLORREF crBrush;

            rect rect;

            rect = pdrawitem->m_rectSubItem;

            int iMin = MIN(MAX(rect.get_size().get_minimum_dimension() - 2, 1), 8);

            rect.bottom -= (rect.height() - iMin) / 2;

            rect.top = rect.bottom - iMin;

            rect.left += 4;

            rect.right = rect.left + iMin;

            pdrawitem->m_pgraphics->fill_solid_rect(rect, ARGB(128, 80, 80, 80));

         }

         return;

      }
      else if (pdrawitem->m_iSubItem == 0)
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

               pdrawitem->m_pgraphics->fill_solid_rect(pdrawitem->m_rectSubItem, crBackHover);

            }


            bool bOk = pdrawitem->m_strText == "on";

            COLORREF crPen = ARGB(255, 80, 80, 75);

            COLORREF crBrush;

            rect rect;

            rect = pdrawitem->m_rectSubItem;

            int iMin = MAX(rect.get_size().get_minimum_dimension() - 2, 1);

            rect.bottom--;

            rect.top = rect.bottom - iMin;

            rect.left++;

            rect.right = rect.left + iMin;

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

         }

         return;

      }

      ::user::form_list_view::_001DrawSubItem(pdrawitem);

   }


} // namespace twitter_automator






#include <shobjidl.h>

namespace twitter_automator
{

   CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR string get_file_open(::user::frame * pframe, string strFolder)
   {

      string str;

      try
      {

         pframe->enable_window(false);

      }
      catch (...)
      {


      }

      try
      {

         defer_co_initialize_ex(false);

         IFileOpenDialog *pFileOpen;

         // Create the FileOpenDialog object.
         HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                                       IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

         if (SUCCEEDED(hr))
         {
            if (strFolder.has_char())
            {
               wstring wstr(strFolder);
               IShellItem *psi = NULL;
               hr = SHCreateItemFromParsingName(wstr, NULL, IID_IShellItem, (void **)&psi);
               if (SUCCEEDED(hr))
               {

                  pFileOpen->SetFolder(psi);

               }
               if (psi != NULL)
               {
                  psi->Release();
               }
            }

            COMDLG_FILTERSPEC rgSpec[] =
            {
               { L"CSV files", L"*.csv" },
               { L"Text files", L"*.txt" },
               { L"All files", L"*.*" },
            };

            pFileOpen->SetFileTypes(3, rgSpec);


            // Show the Open dialog box.
            //hr = pFileOpen->Show(pframe->get_handle());
            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr))
            {


               // Get the file name from the dialog box.
               if (SUCCEEDED(hr))
               {
                  IShellItem *pItem;
                  hr = pFileOpen->GetResult(&pItem);
                  if (SUCCEEDED(hr))
                  {
                     PWSTR pszFilePath;
                     hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                     // Display the file name to the user.
                     if (SUCCEEDED(hr))
                     {
                        str = pszFilePath;
                        CoTaskMemFree(pszFilePath);
                     }
                     pItem->Release();
                  }
               }
            }
            pFileOpen->Release();
         }

      }
      catch (...)
      {


      }

      try
      {

         pframe->enable_window(true);

         pframe->SetWindowPos(ZORDER_TOP, ::null_rect(), SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

         pframe->SetForegroundWindow();

         pframe->BringWindowToTop();

         pframe->BringToTop(SW_NORMAL);

      }
      catch (...)
      {


      }

      return str;
   }


   CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR string get_file_save(::user::frame * pframe, string strFolder)
   {
      string str;
      try
      {

         pframe->enable_window(false);

      }
      catch (...)
      {


      }

      try
      {

         IFileSaveDialog *pFileSave;

         // Create the FileSaveDialog object.
         HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                                       IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

         if (SUCCEEDED(hr))
         {

            COMDLG_FILTERSPEC rgSpec[] =
            {
               { L"CSV files", L"*.csv" },
               { L"Text files", L"*.txt" },
               { L"All files", L"*.*" },
            };

            pFileSave->SetFileTypes(3, rgSpec);


            pFileSave->SetDefaultExtension(L"csv");

            pFileSave->SetFileName(L"Twitter Automator Export");

            if (strFolder.has_char())
            {
               wstring wstr(strFolder);
               IShellItem *psi = NULL;
               hr = SHCreateItemFromParsingName(wstr, NULL, IID_IShellItem, (void **)&psi);
               if (SUCCEEDED(hr))
               {

                  pFileSave->SetFolder(psi);

               }
               if (psi != NULL)
               {
                  psi->Release();
               }
            }


            // Show the Save dialog box.
            //hr = pFileSave->Show(pframe->get_handle());
            hr = pFileSave->Show(NULL);

            if (SUCCEEDED(hr))
            {


               // Get the file name from the dialog box.
               if (SUCCEEDED(hr))
               {
                  IShellItem *pItem;
                  hr = pFileSave->GetResult(&pItem);
                  if (SUCCEEDED(hr))
                  {
                     PWSTR pszFilePath;
                     hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                     // Display the file name to the user.
                     if (SUCCEEDED(hr))
                     {
                        str = pszFilePath;
                        CoTaskMemFree(pszFilePath);
                     }
                     pItem->Release();
                  }
               }
            }
            pFileSave->Release();
         }
      }
      catch (...)
      {


      }
      try
      {

         pframe->enable_window(true);

         pframe->SetWindowPos(ZORDER_TOP, ::null_rect(), SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

         pframe->SetForegroundWindow();

         pframe->BringWindowToTop();

         pframe->BringToTop(SW_NORMAL);

      }
      catch (...)
      {


      }

      return str;

   }

} // namespace twitter_automator
