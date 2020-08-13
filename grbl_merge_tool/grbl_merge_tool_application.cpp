#include "framework.h"
#include "aura/net/net_sockets.h"


namespace grbl_merge_tool
{


   application::application()
   {

      m_pedit = NULL;
      m_pedit1 = NULL;
      m_pedit2 = NULL;
      m_scalar.m_psource = this;
      m_scalar.m_escalar = scalar_progress;
      set_local_data_key_modifier();

      ::sockets::tcp_socket::s_bReuseSession = false;

      m_emode = mode_none;

      m_ptemplateGrblMergeToolMain = NULL;

      m_strAppName            = "grbl_merge_tool";
      m_strBaseSupportId      = "ca2_flag";
      m_bLicense              = false;
      m_eexclusiveinstance = ExclusiveInstanceLocal;


      m_plist = NULL;

      m_ppaneview = NULL;

      m_pedit = NULL;

      m_pcontrolview = NULL;
      m_straCoord[0] = "0";
      m_straCoord[1] = "0";
      m_straCoord[2] = "0";
      m_straCoord[3] = "0";
      m_straCoord[4] = "0";
      m_straCoord[5] = "0";
      m_straCoord[6] = unitext("⇦");
      m_straCoord[7] = unitext("⇨");
      m_straCoord[8] = unitext("⇧");
      m_straCoord[9] = unitext("⇩");
      m_straCoord[10] = unitext("▼");
      m_straCoord[11] = unitext("▲");
      m_straCoord[12] = "10.0";
      m_straCoord[13] = "0.01";
      m_straCoord[14] = "0.1";
      m_straCoord[15] = "1";
      m_straCoord[16] = "5";
      m_straCoord[17] = "10";
      m_straCoord[18] = "100";

   }


   application::~application()
   {
   }


   bool application::init_instance()
   {


      connect_command("file_start", &application::_001OnFileStart);
      connect_command_probe("file_start", &application::_001OnUpdateFileStart);
      connect_command("file_stop", &application::_001OnFileStop);
      connect_command_probe("file_stop", &application::_001OnUpdateFileStop);
      connect_command("file_reset", &application::_001OnFileReset);
      connect_command_probe("file_reset", &application::_001OnUpdateFileReset);
      connect_command("impact_main", &application::_001OnViewMain);
      connect_command("file_open_file", &application::_001OnFileOpenFile);
      connect_command_probe("file_open_file", &application::_001OnUpdateFileOpenFile);
      connect_command("file_open_library", &application::_001OnFileOpenLibrary);
      connect_command_probe("file_open_library", &application::_001OnUpdateFileOpenLibrary);
      connect_command("tools_options", &application::_001OnToolsOptions);
      connect_command("file_barcode", &application::_001OnFileBarcode);


      System.factory().creatable_small < ::grbl_merge_tool::main_form_list >();

      System.factory().creatable_small < ::grbl_merge_tool::document >();
      System.factory().creatable_small < ::grbl_merge_tool::frame >();
      System.factory().creatable_small < ::grbl_merge_tool::top_frame >();
      System.factory().creatable_small < ::grbl_merge_tool::top_pane_view >();
      System.factory().creatable_small < ::grbl_merge_tool::main_frame >();
      System.factory().creatable_small < ::grbl_merge_tool::main_view >();
      System.factory().creatable_small < ::user::button_view >();
      System.factory().creatable_small < ::grbl_merge_tool::pane_view >();
      System.factory().creatable_small < ::grbl_merge_tool::split_view1 >();
      System.factory().creatable_small < ::grbl_merge_tool::control_view >();

      if (!::core::application::init_instance())
      {

         return false;

      }

      m_pcontrol = canew(control(this));

      User.filemanager()->m_pathDefault = Context.dir().home() / "GRBL Merge Tool";

      ::user::single_document_template* pDocTemplate;

      pDocTemplate = canew(::user::single_document_template(
                           this,
                           "main",
                           System.type_info < ::user::document >(),
                           System.type_info < top_frame > (),
                           System.type_info < top_pane_view > ()));

      add_document_template(pDocTemplate);

      m_ptemplateGrblMergeToolTop = pDocTemplate;

      pDocTemplate = canew(::user::single_document_template(
                           this,
                           "main",
                           System.type_info < document >(),
                           System.type_info < main_frame > (),
                           System.type_info < pane_view > ()));

      add_document_template(pDocTemplate);

      m_ptemplateGrblMergeToolMain = pDocTemplate;

      return true;

   }


   void application::term_instance()
   {

      ::multithreading::set_finish(m_pcontrol->m_pthreadRead);

      m_pcontrol.release();

      ::core::application::term_instance();

   }


   bool application::open_serial_port(string strPort, int iBaud)
   {

      if (strPort.is_empty())
      {

         data_get("serial_port", strPort);

      }
      else
      {

         data_set("serial_port", strPort);

      }

      if (iBaud <= 0)
      {

         data_get("baud_rate", iBaud);

      }
      else
      {

         data_set("baud_rate", iBaud);

      }

      return m_pcontrol->open_serial_port(strPort, iBaud);

   }



   void application::list_serial_ports()
   {

      auto porta = ::serial::list_ports();

#ifdef WINDOWS
      porta.pred_sort([](auto & port1, auto & port2)
      {

         string strPort1(port1.port);

         string strPort2(port2.port);

         ::str::begins_eat_ci(strPort1, "com");

         ::str::begins_eat_ci(strPort2, "com");

         int iPort1 = atoi(strPort1);

         int iPort2 = atoi(strPort2);

         return iPort1 < iPort2;

      });

#endif

      m_portinfoa = porta;

#ifdef DEV_DEBUG

      {

         ::serial::PortInfo p;
         p.port = "/dev/ttyS10";
         m_portinfoa.add(p);

      }

      {

         ::serial::PortInfo p;
         p.port = "/dev/ttyS12";
         m_portinfoa.add(p);

      }

      {

         ::serial::PortInfo p;
         p.port = "/dev/ttyS14";
         m_portinfoa.add(p);

      }

#endif

   }


   void application::on_request(::create * pcreatecontext)
   {

      if(m_ptemplateGrblMergeToolTop->get_document_count() == 0)
      {

         m_ptemplateGrblMergeToolTop->request_create(pcreatecontext);

      }


      on_new_document();

   }


   i64 application::add_ref()
   {

      return ::object::add_ref();

   }

   i64 application::dec_ref()
   {

      return ::object::dec_ref();

   }


   string application::preferred_experience()
   {

      return "wndfrm_anthill";

   }


   void application::set_mode(e_mode emode)
   {

      m_emode = emode;

      if (m_pedit1 != NULL)
      {
         m_pedit1->enable_window(emode == mode_library);

      }
      if (m_pedit2 != NULL)
      {
         m_pedit2->enable_window(emode == mode_library);
      }

   }


   bool application::insert(string strFile, bool bCharCount)
   {

      ::file::path path;

      path = m_filepath / (strFile + ".nc");

      if (!file().exists(path))
      {

         path = m_filepath / (strFile + ".nc.txt");

         if (!file().exists(path))
         {

            if (bCharCount)
            {

               m_bInvalidCharsCount = true;

               return true;

            }

            return false;

         }

      }

      if (bCharCount)
      {

         m_bInvalidCharsCount = false;

      }

      stringa stra;

      file().load_lines(stra, path);

      stra.trim();

      stra.remove_empty();

      command command;

      command.m_iIndex = m_commanda.get_size();

      command.m_estate = command::state_queue;

      for (auto & str : stra)
      {

         command.m_strCommand = str;

         m_commanda.add(command);

         command.m_iIndex++;

      }

      return true;

   }


   bool application::update_commands(bool bInteractive)
   {

      synch_lock sl(mutex());

      if (m_filepath.is_empty() || !dir().is(m_filepath))
      {

         if (bInteractive)
         {

            MessageBox(m_ptoppaneview->get_safe_handle(), "There is no library open", "Error", MB_ICONEXCLAMATION);

         }

         return false;

      }

      Application.m_pedit->_001GetText(m_strCode);

      stringa stra;

      m_bInvalidCharsCount = true;

      int iCount = m_strCode.get_length();

      m_commanda.remove_all();

      //if (iCount <= 0)
      //{

      //   return;

      //}

      if (!insert("header"))
      {

         m_commanda.remove_all();

         sl.unlock();

         MessageBox(NULL, "Missing header file.", "Error", MB_ICONINFORMATION);

         return false;

      }

      for (int i = 0; i < iCount; i++)
      {

         string strChar = m_strCode.Mid(i, 1);

         if (!insert(strChar))
         {

            m_commanda.remove_all();

            sl.unlock();

            MessageBox(NULL, "Missing character file.", "Error", MB_ICONINFORMATION);

            return false;

         }

      }

      insert("char_" + str::from(iCount), true);

      if (!insert("footer"))
      {

         m_commanda.remove_all();

         sl.unlock();

         MessageBox(NULL, "Missing footer file.", "Error", MB_ICONINFORMATION);

         return false;

      }

      for (auto & command : m_commanda)
      {

         stra.add(command.m_strCommand);

      }

      sl.unlock();

      //file().put_lines(m_filepath / ("plate_" + m_strCode + ".nc.txt"), stra);

end:

      update_all_views(update_open_file);

      return true;

   }

   void application::update_all_views(LONG lHint)
   {


      if (m_ptemplateGrblMergeToolMain->get_document() != NULL)
      {

         m_ptemplateGrblMergeToolMain->update_all_views(NULL, lHint, NULL);

      }


   }

   void application::on_transfer_success()
   {

      {

         synch_lock sl(mutex());

         m_commanda.remove_all();

      }

      ::run_pred_on_main_thread([this]()
      {

         if (m_emode == mode_library)
         {

            m_pedit->enable_window(true);

            m_pedit->SetFocus();

         }

         m_pedit->_001SetText("", ::source_user);

      });



   }


   bool application::on_new_document()
   {

      string str;

      data_get("last_opened_library", str);

      if (str.has_char())
      {

         return on_open_document(str);

      }

      return true;

   }


   bool application::on_open_document(const var & varFile)
   {

      m_filepath = varFile;

      if (dir().is(m_filepath))
      {

         set_mode(mode_library);

         update_all_views( update_open_library);

      }
      else
      {

         set_mode(mode_file);

         synch_lock sl(mutex());

         stringa stra;

         file().load_lines(stra, varFile);

         command command;

         command.m_iIndex = 0;

         command.m_estate = command::state_queue;

         stra.trim();

         stra.remove_empty();

         for (auto & str : stra)
         {

            command.m_strCommand = str;

            m_commanda.add(command);

            command.m_iIndex++;

         }

         update_all_views(update_open_file);

      }

      return true;

   }

   void application::_001OnFileStart(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      on_start();

   }

   void application::get_scalar_minimum(e_scalar escalar, double & d)
   {
      d = 0.0;
   }
   void application::get_scalar(e_scalar escalar, double & d)
   {
      if (m_commanda.get_count() <= 0)
      {
         d = 0.0;
         return;
      }
      d = (double)m_pcontrol->m_iFileProcessedCommandIndex / (double)m_commanda.get_count();
   }
   void application::get_scalar_maximum(e_scalar escalar, double & d)
   {
      d =1.0;
   }

   void application::on_start()
   {

      if (m_emode == mode_library)
      {

         if (m_bInvalidCharsCount)
         {

            if (MessageBox(NULL, "Invalid number of characters.\n\n(Missing char_" + ::str::from(m_strCode.length()) + ".nc file.)\n\nDo you want to proceed?", "Attention", MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
            {

               //{

               //   synch_lock sl(mutex());

               //   m_commanda.remove_all();

               //}

               //::run_pred_on_main_thread([this]()
               //{

               //   m_pedit->_001SetText("", ::source_user);

               //});

               return;

            }

         }

      }

      m_pedit->enable_window(false);

      if (m_pcontrol->start_sending_file(&m_commanda))
      {

         update_all_views(update_start);

      }

   }

   void application::_001OnUpdateFileStart(::message::message * pmessage)
   {

      SCAST_PTR(::user::command, pcmdui, pmessage);

      pcmdui->Enable(m_filepath.has_char() && m_commanda.get_count() > 0 && !m_pcontrol->is_sending_file());

   }

   void application::_001OnFileStop(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      if (m_pcontrol->stop_sending_file())
      {

         update_all_views(update_stop);

      }

   }


   void application::_001OnUpdateFileStop(::message::message * pmessage)
   {

      SCAST_PTR(::user::command, pcmdui, pmessage);

      pcmdui->Enable(m_pcontrol->is_sending_file());

   }


   void application::_001OnFileReset(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      m_pcontrol->grblReset();

      update_all_views(update_reset);

   }


   void application::_001OnUpdateFileReset(::message::message * pmessage)
   {

      SCAST_PTR(::user::command, pcmdui, pmessage);

      pcmdui->Enable(!m_pcontrol->is_sending_file());

   }

   void application::_001OnViewMain(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      m_ppaneview->set_cur_tab_by_id(pane_main);

   }


   void application::_001OnFileOpenFile(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      fork([this]()
      {

         string str;

         data_get("last_opened_file_folder", str);

         if (str.is_empty())
         {

            str = Context.dir().home() / "GRBL Merge Tool";

            if (!dir().is(str))
            {

               str = Context.dir().desktop();

               if (!dir().is(str))
               {

                  str = "";

               }

            }


         }

         property_set set;

         set["file_filter_specs"] = "*.*";
         set["file_filter_names"] = "Any File";
         set["folder"] = str;

         if (!Context.os().browse_file_open(m_pmainview->GetParentFrame()->get_handle(), set))
         {

            return;
            //::grbl_merge_tool::get_file_open(GetTopLevelFrame(), str);

         }

         file::path path = set["file_name"];

         if (path.is_empty())
         {

            return;

         }

         data_set("last_opened_file_folder", path.folder());

         on_open_document(path);

         m_ppaneview->set_cur_tab_by_id(pane_main);

      });

   }



   void application::_001OnUpdateFileOpenFile(::message::message * pmessage)
   {

      SCAST_PTR(::user::command, pcmdui, pmessage);

      pcmdui->Enable(!m_pcontrol->is_sending_file());

   }


   void application::_001OnFileBarcode(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      m_ptoppaneview->set_cur_tab_by_id("form_main");

   }


   void application::_001OnFileOpenLibrary(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      fork([this]()
      {

         string str;

         data_get("last_opened_library", str);

         if (str.is_empty())
         {

            str = Context.dir().home() / "GRBL Merge Tool";

            if (!dir().is(str))
            {

               str = Context.dir().desktop();

               if (!dir().is(str))
               {

                  str = "";

               }

            }


         }

         property_set set;

         set["file_filter_specs"] = "*.*";
         set["file_filter_names"] = "Any File";
         set["folder"] = str;

         if (!Context.os().browse_folder(m_pmainview->GetParentFrame()->get_handle(), set))
         {

            return;
            //::grbl_merge_tool::get_file_open(GetTopLevelFrame(), str);

         }

         file::path path = set["folder"];

         if (path.is_empty())
         {

            return;

         }

         data_set("last_opened_library", path);

         on_open_document(path);

         m_ppaneview->set_cur_tab_by_id(pane_main);

         ::run_pred_on_main_thread([this]()
         {

            if (m_emode == mode_library)
            {

               m_pedit->enable_window(true);

               m_pedit->SetFocus();

            }

         });

      });

   }



   void application::_001OnUpdateFileOpenLibrary(::message::message * pmessage)
   {

      SCAST_PTR(::user::command, pcmdui, pmessage);

      pcmdui->Enable(!m_pcontrol->is_sending_file());

   }

   void application::_001OnToolsOptions(::message::message * pmessage)
   {

      pmessage->m_bRet = true;

      m_ppaneview->set_cur_tab_by_id("form_options");

   }



} // namespace grbl_merge_tool




extern "C"
::aura::library * app_ca2_grbl_merge_tool_get_new_library(::aura::application * papp)
{

   return canew(::aura::single_application_library < ::grbl_merge_tool::application >(papp, "app-ca2/grbl_merge_tool"));

}




