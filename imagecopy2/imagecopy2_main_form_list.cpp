#include "framework.h"
#include <ShlObj.h>

//string get_file_open(::user::frame_window * pframe, string strFolder);
//string get_file_save(::user::frame_window * pframe, string strFolder);

int win_message_box(oswindow window, const char * text, const char * pcaption, UINT uiType)
{

   return ::MessageBoxW(window, wstring(text), wstring(pcaption), uiType);

}

namespace windows
{

   ::file::path get_known_folder(REFKNOWNFOLDERID kfid);

}


namespace imagecopy2
{




   main_form_list::main_form_list(::aura::application * papp) :
      ::object(papp),
      simple_form_list_view(papp),
      m_piconUndefined(allocer()),
      m_piconRunning(allocer()),
      m_piconError(allocer()),
      m_piconFullFail(allocer()),
      m_i(get_context_application()),
      m_fontUsername(allocer()),
      m_ls(get_context_application()),
      m_mutexOk(papp),
      m_mutexParameters(papp),
      m_evChange(papp)
   {
      m_bChangedFilter = false;
      m_iProcess = -1;
      m_iProcessTotal = -1;
      m_bThumbnailsPrevious = false;
      m_pthreadThumb = NULL;
      m_pmainview = NULL;
//      m_mapImageTime.InitHashTable(2048);
//      m_mapImage.InitHashTable(2048);
      m_iImageSpacing = 7;
      //m_bMorePlain = true;
      m_i.create(128, 72);
      m_bSortEnable = false;
      //m_ptaska->m_pformlist = this;
      m_iItemHeight = 64;
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


      connect_command_probe("open_folder", &main_form_list::_001OnUpdateOpenFolder);
      connect_command("open_folder", &main_form_list::_001OnOpenFolder);
      connect_command_probe("file_copy", &main_form_list::_001OnUpdateFileCopy);
      connect_command("file_copy", &main_form_list::_001OnFileCopy);


      connect_command_probe("parameters", &main_form_list::_001OnUpdateParameters);
      connect_command("parameters", &main_form_list::_001OnParameters);
      connect_command_probe("task_stop_all", &main_form_list::_001OnUpdateStopAllTasks);
      connect_command("task_stop_all", &main_form_list::_001OnStopAllTasks);

      m_piconUndefined->load_matter("main/undefined.ico");
      m_piconRunning->load_matter("main/valid.ico");
      m_piconError->load_matter("main/error.ico");
      m_piconFullFail->load_matter("main/full_fail.ico");

      m_i.add_file("matter://imagecopy.png");

   }

   main_form_list::~main_form_list()
   {

   }




   void main_form_list::_001InsertColumns()
   {

      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 160;
         pcolumn->m_uiText = "";
         pcolumn->m_iSubItem = 1000;
         pcolumn->m_pil = &m_i;

         _001AddColumn(column);


      }
      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 200;
         pcolumn->m_uiText = "File Name";
         pcolumn->m_iSubItem = 0;

         _001AddColumn(column);


      }
      {

         auto pcolumn = new_list_column();

         pcolumn->m_iWidth = 300;
         pcolumn->m_uiText = "Date Taken";
         pcolumn->m_iSubItem = 1;
         //pcolumn->m_pil = &m_i;

         _001AddColumn(column);


      }

      m_i.remove_all();

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

   void main_form_list::on_layout()
   {

      simple_form_list_view::on_layout();


   }

   void main_form_list::on_update(::user::impact * pSender, LPARAM lHint, ::object * pupdate)
   {

      simple_form_list_view::on_update(pSender, lHint, pupdate);

      if (lHint == document::update_import)
      {

         ::file::listing ls(get_context_application());

         ls.remove_all();

         m_pathSource = get_document()->m_filepath;

         ls.ls_file(m_pathSource);

         __pointera(image_info) imageinfoa;

         imageinfoa.set_size(ls.get_size());
         for (index i = 0; i < ls.get_size();)
         {
            m_iProcess = i;
            m_iProcessTotal = ls.get_size();
            //TRACE("Checking File: %s", ls[i].name());
            if (!touch_file(ls[i], imageinfoa[i]))
            {

               //TRACE(" : not Image or Image has no properties.\n");
               ls.remove_at(i);

            }
            else
            {
               //string strTime = System.datetime().international().get_local_date_time(imageinfoa[i]->m_time);

               //TRACE(" : OK (%s)\n", strTime);
               i++;

            }

         }

         m_iProcess = -1;
         m_iProcessTotal = -1;

         imageinfoa.set_size(ls.get_size());


         ::sort::array::pred_sort(imageinfoa,
                                  [&](const __pointer(image_info) & p1, const __pointer(image_info) & p2)
         {

            return p1->m_time <= p2->m_time;

         });


         {

            synch_lock sl(mutex());

            m_ls = ls;

            m_imageinfoa = imageinfoa;

         }



         _001OnUpdateItemCount();


         defer_create_thumbnails();

         if (m_imageinfoa.has_elements() && get_document()->m_bOpen)
         {

            m_strStartDateTime = System.datetime().international().get_local_date_time(m_imageinfoa.first().m_time);

            m_strEndDateTime = System.datetime().international().get_local_date_time(m_imageinfoa.last().m_time);

            m_pmainview->update_form(false);

         }
         m_bChangedFilter = true;
         save_parameters();

      }
      else if (lHint == document::update_export)
      {

         //m_ptaska->simple_csv_save_file(get_document()->m_filepath);

      }



   }

   void main_form_list::_001OnContextMenu(::message::message * pobj)
   {

      track_popup_xml_matter_menu("devedge_contextmenu.xml", 0, pobj);

   }

   void main_form_list::_001OnUpdateParameters(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }



   void main_form_list::_001OnParameters(message::message * pobj)
   {

      //if (m_pdialog.is_null())
      //{

      //   m_pdialog = canew(task_dialog(get_context_application()));

      //}

      //m_pdialog->m_plist = this;

      //m_pdialog->show();

   }

   void main_form_list::_001OnUpdateOpenFolder(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }



   void main_form_list::_001OnOpenFolder(::message::message * pobj)
   {
      if (pobj != NULL)
      {
         pobj->m_bRet = true;

      }

      ::fork(get_context_application(), [&]()
      {

         string str = get_document()->m_filepath;

         if (str.is_empty())
         {

            str = ::windows::get_known_folder(FOLDERID_Desktop);

            if (!Context.dir().is(str))
            {

               str = "";

            }

         }

         property_set set;

         get_document()->data_get("open_folder", set);

         if (Context.os().browse_folder(get_handle(), set))
         {

            get_document()->m_bOpen = true;

            get_document()->data_set("open_folder", set);

            get_document()->m_filepath = set["folder"].get_string();

            get_document()->update_all_views(NULL, document::update_import);

         }

      });

   }

   void main_form_list::_001OnUpdateFileCopy(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnFileCopy(::message::message * pobj)
   {

      if (pobj != NULL)
      {

         pobj->m_bRet = true;

      }

      ::fork(get_context_application(), [&]()
      {


         ::file::patha pathaFiltered;
         ::file::path pathFolder;

         {

            synch_lock sl(&m_mutexData);

            pathaFiltered = m_pathaOk;

            pathFolder = m_pathFolder;

         }

         string str(pathFolder);
         //         ::file::path pathFolder;
         str.trim();
         if (str.find(":") != 1 && !::str::begins_ci(str, "\\\\"))
         {
            ::file::path pathSource = m_pathSource;
            pathFolder =  pathSource / str;
         }

         Context.dir().mk(pathFolder);

         for (index i = 0; i < pathaFiltered.get_size(); i++)
         {
            ::file::path p = pathaFiltered[i];
            //::datetime::time t(m_imageinfoa[i]);
            //string strTime = System.datetime().international().get_local_date_time(t);
            m_pathCopy = p;
            ensure_image_visible();

            ::file::path pathDestination = pathFolder / p.name();

//            TRACE("%5d: %s Copying file to: %s\n", i + 1, strTime.c_str(), pathDestination.c_str());

            if (!::CopyFileW(wstring(p), wstring(pathDestination), TRUE))
            {
               DWORD dwLastError = ::GetLastError();
               string strError = get_last_error_string();

               string strMessage;
               string strFormat;
               strFormat.Format("Failed to copy file: %s to %s \n", p.c_str(), pathDestination.c_str());
               strMessage += strFormat;
               strFormat.Format("Error (%d) : %s\n", dwLastError, strError.c_str());
               strMessage += strFormat;
               strFormat.Format("Operation aborted\n");
               strMessage += strFormat;
               if (i == 0)
               {
                  strFormat.Format("No files copied.\n");
               }
               else
               {
                  strFormat.Format("Only %d files copied so far.\n", i);
               }
               strMessage += strFormat;
               m_pathCopy.Empty();
               win_message_box(NULL, strMessage, "Image Copy Error", MB_ICONEXCLAMATION);
               return;

            }
            //Sleep(1000);
         }
         m_pathCopy.Empty();
         open_folder(pathFolder);
//         win_message_box(NULL, ::str::from(pathaFiltered.get_count()) + " images copied", "Image Copy", MB_ICONINFORMATION);

      });

   }

   void main_form_list::ensure_image_visible()
   {

      synch_lock sl(mutex());

      index iFind = m_ls.find_first_ci(m_pathCopy);

      _001EnsureVisible(iFind);

   }


   void main_form_list::open_folder(::file::path pathFolder)
   {
      wstring wstr(pathFolder);
      PIDLIST_ABSOLUTE pidl;
      defer_co_initialize_ex(false);
      if (SUCCEEDED(SHParseDisplayName(wstr, 0, &pidl, 0, 0)))
      {
         // we don't want to actually select anything in the folder, so we pass an empty
         // PIDL in the array. if you want to select one or more items in the opened
         // folder you'd need to build the PIDL array appropriately
         ITEMIDLIST idNull = { 0 };
         LPCITEMIDLIST pidlNull[1] = { &idNull };
         SHOpenFolderAndSelectItems(pidl, 1, pidlNull, 0);
         ILFree(pidl);
      }

   }

//   void main_form_list::_001OnUpdateStartAllTasks(::message::message * pobj)
//   {
//
//      SCAST_PTR(::user::command, pcmdui, pobj);
//
//      pcmdui->Enable(true);
//
//   }
//
//
//   void main_form_list::_001OnStartAllTasks(::message::message * pobj)
//   {
//
////      m_ptaska->start_all_tasks();
//
//
//   }

   void main_form_list::_001OnUpdateStopAllTasks(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnStopAllTasks(::message::message * pobj)
   {
//      m_ptaska->stop_all_tasks();
   }

   void main_form_list::_001OnUpdateAddTask(::message::message * pobj)
   {

      SCAST_PTR(::user::command, pcmdui, pobj);

      pcmdui->Enable(true);

   }


   void main_form_list::_001OnAddTask(::message::message * pobj)
   {

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


      if(!m_bThumbnails)
      {

         pitem->m_bOk = false;

      }
      else if (pitem->m_iSubItem == 1000)
      {

         if (pitem->m_iItem < m_i.get_image_count() / 2)
         {

            pitem->m_iImage = pitem->m_iItem * 2;
            if (!is_item_ok(pitem->m_iItem))
            {

               pitem->m_iImage++;

            }

            pitem->m_bOk = true;

         }
         else
         {

            pitem->m_iImage = -1;

            pitem->m_bOk = false;

         }

      }

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

      m_fontUsername->create_point_font("Tahoma", 11, 400);

      data_get("startdatetime", m_strStartDateTime);
      data_get("enddatetime", m_strEndDateTime);
      data_get("interval", m_strInterval);
      data_get("folder", m_strFolder);

      if (!data_get("thumbnails", m_bThumbnails))
      {

         m_bThumbnails = true;

      }

      _001UpdateColumns();
      _001OnUpdateItemCount();

      SetTimer(1234, 250, NULL);

      m_plistheader->m_iImageSpacing = 16;


      ::fork(get_context_application(), [&]()
      {

         while (::thread_get_run())
         {

            if (!m_evChange.lock(seconds(15000)))
            {

               m_evChange.ResetEvent();

               continue;

            }

            m_evChange.ResetEvent();

            try
            {

               update_ok_list();

            }
            catch (...)
            {


            }

            set_need_layout(true);
//            CreateDatePick(get_handle());


         }

      });

   }

   bool main_form_list::save_parameters(bool bChangedFilter, bool bChangedThumb)
   {
      synch_lock sl(&m_mutexParameters);

      data_set("startdatetime", m_strStartDateTime);
      data_set("enddatetime", m_strEndDateTime);
      data_set("interval", m_strInterval);
      data_set("folder", m_strFolder);
      data_set("thumbnails", m_bThumbnails);

      if(!m_bThumbnailsPrevious && m_bThumbnails && bChangedThumb)
      {

         defer_create_thumbnails();

      }
      else
      {

         m_bThumbnailsPrevious = false;

      }

      if (bChangedFilter)
      {

         m_bChangedFilter = true;

      }

      m_evChange.SetEvent();

      return true;



   }

   bool main_form_list::parse_parameters()
   {

      synch_lock sl(&m_mutexParameters);

      if (m_strStartDateTime.is_empty())
      {

         return false;

      }

      if (m_strEndDateTime.is_empty())
      {

         return false;

      }

      {

         int iPath = 0;

         int iPathCount = 0;

         m_timeStart = System.datetime().strtotime(NULL, m_strStartDateTime, iPath, iPathCount);

      }

      {

         int iPath = 0;

         int iPathCount = 0;

         m_timeEnd = System.datetime().strtotime(NULL, m_strEndDateTime, iPath, iPathCount);

      }

      m_iInterval = atoi(m_strInterval);

      m_pathFolder = m_strFolder;

      return true;

   }

   void main_form_list::update_ok_list()
   {




      if (parse_parameters())
      {

         if (m_bChangedFilter)
         {
            m_bChangedFilter = false;
            ::file::patha pathaSrc;
            __pointera(image_info) imageinfoa;

            {

               synch_lock sl(mutex());

               // m_columna[0]->m_bVisible = m_bThumbnails;

               pathaSrc = m_ls;

               imageinfoa = m_imageinfoa;

            }
            if (pathaSrc.has_elements())
            {
               ::file::patha pathaFiltered;


               ::count cOutput = 0;
               __time64_t s = MAX(1, ::datetime::time_span(0, 0, 0, (i32) m_iInterval).GetTotalSeconds());

               //__time64_t timeNext;
               __time64_t timeStart = m_timeStart.m_time;
               __time64_t timeEnd = m_timeEnd.m_time;

               index iStart = 0;
               m_iProcess = 0;
               m_iProcessTotal = m_ls.get_size();
               ::count c = pathaSrc.get_size();
               ::array <__time64_t> ta;

               ta.set_size(c);
               for (index i = iStart; i < c; i++)
               {
                  ta[i] = imageinfoa[i]->m_time.m_time;
               }
               pathaFiltered.set_size(c);

               index i = 0;
               while (i < c && ta[i] < timeStart)
               {
                  i++;
                  m_iProcess = i;
               }

               if (i < c)
               {
                  __time64_t tBeg = timeStart;
                  index iSpace = (index) ((ta[i] - tBeg) / s);
                  while (ta[i] <= timeEnd)
                  {
                     pathaFiltered[cOutput] = pathaSrc[i];
                     cOutput++;
                     iStart = i + 1;
                     m_iProcess = i;
                     while (true)
                     {
                        i++;
                        if (i >= c)
                        {

                           goto end;

                        }
                        index iNewSpace = (index) ((ta[i] - tBeg) / s);
                        if (iNewSpace > iSpace)
                        {
                           iSpace = iNewSpace;
                           break;
                        }
                     }

                  }
end:
                  ;
                  pathaFiltered.set_size(cOutput);
                  m_iProcess = -1;
                  m_iProcessTotal = -1;

                  synch_lock sl(&m_mutexOk);

                  m_pathaOk = pathaFiltered;


               }
            }
         }
         else
         {


         }
      }


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

      return m_ls.get_size();

   }

   bool main_form_list::is_item_ok(index iItem)
   {
      ::file::path path;
      {
         synch_lock sl(mutex());

         path = m_ls[iItem];

      }

      synch_lock sl(&m_mutexOk);

      return m_pathaOk.find_first_ci(path) >= 0;

   }

   void main_form_list::_001GetItemColor(::user::mesh_item * pitem)
   {

      //if (pitem->m_iSubItem == 0)
      {

         if (is_item_ok(pitem->m_iItem))
         {

            pitem->m_crText = ARGB(255, 0, 0,0);

            pitem->m_bOk = true;

         }
         else
         {

            pitem->m_crText = ARGB(255, 128, 128, 128);

            pitem->m_bOk = true;

         }

         if (m_ls[pitem->m_iItem].compare_ci(m_pathCopy) == 0)
         {

            pitem->m_crItemBackground = ARGB(127, 100, 150, 200);

         }


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
      if (pitem->m_iSubItem == 0)
      {

         pitem->m_strText = m_ls[pitem->m_iItem].name();

         if (m_ls[pitem->m_iItem].compare_ci(m_pathCopy) == 0)
         {

            pitem->m_strText += " (Copying"+::str::block('.', (get_tick_count() / 500) % 4)+")";

         }

         pitem->m_bOk = true;

      }
      else if (pitem->m_iSubItem == 1)
      {

         pitem->m_strText = System.datetime().international().get_local_date_time(m_imageinfoa[pitem->m_iItem]->m_time);

         pitem->m_bOk = true;

      }
      else
      {

         pitem->m_bOk = false;

      }


   }


   void main_form_list::_001SetItemText(::user::mesh_item * pitem)
   {

      return simple_form_list_view::_001SetItemText(pitem);

   }

   bool main_form_list::on_click(const ::user::item & item)
   {

      if (iSubItem == 1000 && m_bThumbnails)
      {
         synch_lock sl(mutex());
         ShellExecuteW(get_handle(), L"open", wstring(m_ls[iItem]), L"", wstring(m_ls[iItem].folder()), SW_SHOWNORMAL);

      }
      return simple_form_list_view::on_click(item);

   }


   void   main_form_list::_001DrawSubItem(::user::draw_list_item * pdrawitem)
   {


      simple_form_list_view::_001DrawSubItem(pdrawitem);

   }

   document * main_form_list::get_document()
   {
      return dynamic_cast <document *> (::user::impact::get_document());
   }

   i32 main_form_list::_001CalcItemHeight(int iBaseHeight)
   {

      return m_i.m_size.cy + 8;

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


   void main_form_list::defer_create_thumbnails()
   {

      if (m_bThumbnails)
      {

         ::file::patha patha;

         {

            synch_lock sl(mutex());

            patha = m_ls;

         }

         ::fork(get_context_application(),[=]()
         {

            __pointer(::thread) pthreadOldThread;

            {

               synch_lock sl(mutex());

               pthreadOldThread = m_pthreadThumb;

               m_pthreadThumb = ::get_thread();

            }

            if (pthreadOldThread.is_set())
            {

               ::multithreading::post_quit_and_wait(pthreadOldThread, minutes(1));

            }

            ::multithreading::set_priority(::multithreading::priority_lowest);
            m_i.remove_all();
            ::visual::dib_pointer dLoad(allocer());
            ::draw2d::dib_pointer d(allocer());
            d->create(m_i.m_size.cx, m_i.m_size.cy);
            ::draw2d::dib_pointer d2(allocer());
            d2->create(m_i.m_size.cx, m_i.m_size.cy);
            int cx = m_i.m_size.cx;
            int cy = m_i.m_size.cy;
            m_i.m_iGrow = (i32) (patha.get_size() * 2 + 100);
            for (int i = 0; i <  patha.get_size() && m_bThumbnails; i++)
            {
               m_iProcess = i;
               m_iProcessTotal = patha.get_size();
               ::file::path pathFile = patha[i];

               //{

               //   synch_lock sl(mutex());
               //   int j;
               //   if (m_mapImage.lookup(pathFile, j))
               //   {

               //      continue;

               //   }

               //}

               if (!dLoad.load_thumbnail(pathFile, cx, cy))
               {
                  d->Fill(0, 0, 0, 0);

               }
               else
               {

                  d->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

                  d->get_graphics()->SetStretchBltMode(COLORONCOLOR);

                  d->get_graphics()->StretchBlt(0, 0, d->m_size.cx,
                                                d->m_size.cy, dLoad.get_graphics(), 0, 0,
                                                dLoad->m_size.cx, dLoad.m_size.cy);

               }

               {
                  synch_lock sl(mutex());

                  m_i.add_dib(d);

                  d2->FillByte(0);

                  d2->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_blend);

                  d2->blend(null_point(), d, null_point(), d2->m_size, 32);

                  m_i.add_dib(d2);

                  //m_mapImage.set_at(pathFile, i);

               }


            }
            m_iProcess = -1;
            m_iProcessTotal = -1;

            m_pthreadThumb = NULL;
         });

      }

      m_bThumbnailsPrevious = m_bThumbnails;

   }


} // namespace imagecopy2







#undef new
#define min MIN
#define max MAX
#include <gdiplus.h>
#define new ACME_NEW


namespace imagecopy2
{


   using namespace Gdiplus;
#undef new

   //::datetime::time main_form_list::get_image_time(string str)
   //{

   //   return m_mapImageTime[str];

   //}

   bool main_form_list::touch_file(string strPath, __pointer(image_info) & imageinfo)
   {
      bool bSet = false;

      UINT    size = 0;
      UINT    count = 0;
      wstring wstr(strPath);
      Bitmap* bitmap = new Bitmap(wstr);
      bitmap->GetPropertySize(&size, &count);
      //printf("There are %d pieces of metadata in the file.\n", count);
      //printf("The total size of the metadata is %d bytes.\n", size);
      if (count > 0)
      {

         PropertyItem* propertyItem = NULL;

         // Assume that the image has a property item of type PropertyItemEquipMake.
         // Get the size of that property item.
         size = bitmap->GetPropertyItemSize(PropertyTagExifDTDigitized);

         // Allocate a buffer to receive the property item.
         propertyItem = (PropertyItem*)malloc(size);

         // Get the property item.
         if (bitmap->GetPropertyItem(PropertyTagExifDTDigitized, size, propertyItem) == Gdiplus::Ok)
         {


            if (propertyItem->type == PropertyTagTypeASCII)
            {

               string str = (const char *)propertyItem->value;
               str.trim();
               int iPath = 0;

               int iPathCount = 0;
               // m_mapImageTime.set_at(strPath, System.datetime().strtotime(NULL, str, iPath, iPathCount));
               ::datetime::time t = System.datetime().strtotime(NULL, str, iPath, iPathCount);
               if (imageinfo.is_null())
               {
                  imageinfo = canew(image_info);
               }
               imageinfo->m_path = strPath;
               imageinfo->m_time = t;
               bSet = true;

            }

         }

         free(propertyItem);


      }
      delete bitmap;

      return bSet;
   }

}