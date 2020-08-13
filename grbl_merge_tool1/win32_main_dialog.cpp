#include "framework.h"

CLASS_DECL_AURA bool read_resource_as_memory_dup(memory & m, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);

namespace grbl_merge_tool
{


   main_dialog::main_dialog(::aura::application * papp) :
      ::object(papp),
      m_pmaininterface(dynamic_cast < ::main_interface * > (papp)),
      ::win32::dialog(papp, MAKEINTRESOURCE(IDD_DIALOG2)),
      m_dlg1(papp),
      m_dlg2(papp)
   {

      m_hbr = ::CreateSolidBrush(RGB(253, 253, 253));
   }


   main_dialog::~main_dialog()
   {

   }

   bool main_dialog::on_init_dialog()
   {

      set_dlg_item_text(IDC_VERSION, ::version::get_version());

      memory_file m(get_context_application());

      read_resource_as_memory_dup(*m.get_primitive_memory(), System.m_hinstance, IDB_LOGO, "PNG");

      m_dib.alloc(allocer());

      m_dib.load_from_file(&m);

      get_dlg_item(m_update, IDC_UPDATE);

      get_dlg_item(m_cancel, IDCANCEL);

      get_dlg_item(m_restart, IDC_RESTART);

      get_dlg_item(m_labelApp, IDC_LABEL_UPDATE_APP);

      get_dlg_item(m_updateApp, IDC_UPDATE_APP);

      m_update.enable_window(false);

      m_fTitle1.create_point_font(170, FONT_APP, 800);

      m_fTitle2.create_point_font(120, FONT_APP, 800);

      m_fText1.create_point_font(110, FONT_APP);

      m_fText2.create_point_font(140, FONT_APP);

      m_update.set_font(m_fText2);

      m_cancel.set_font(m_fText2);

      m_restart.set_font(m_fText2);

      m_updateApp.set_font(m_fText2);

      m_dlg1.m_pparent = this;

      m_dlg2.m_pparent = this;

      m_dlg1.create_dialog(this);

      m_dlg2.create_dialog(this);

      Application.to_close();

      m_dlg1.show_window(SW_SHOW);

      Application.app_check_version();

      return true;


   }



   void main_dialog::on_command(UINT nId)
   {

      if (nId == IDC_UPDATE_APP)
      {

         m_dlg1.enable_children(false);

         m_dlg2.enable_children(false);

         Application.update_app();

      }
      else if (nId == IDCANCEL)
      {

         if (m_pmaininterface->m_bUpdating)
         {

            m_pmaininterface->m_bCancel = true;

         }
         else if (m_pmaininterface->m_iUpdating > 0)
         {

            m_pmaininterface->m_bCancel = true;

         }
         else if (m_pmaininterface->m_bDetecting)
         {

            m_pmaininterface->m_bCancel = true;

         }
         else
         {

            post_message(WM_QUIT);

         }

      }
      else if (nId == IDC_UPDATE)
      {

         if (::IsWindowVisible(m_dlg1.m_hwnd))
         {


            string strFirmwareVersion;

            strFirmwareVersion.Format("Firmware Version: %s", Application.m_strVersion);

            m_dlg2.m_firmwareVersion.set_window_text(strFirmwareVersion);

            m_dlg1.show_window(SW_HIDE);

            m_dlg2.show_window(SW_SHOW);

            m_dlg2.m_combo.show_window(SW_HIDE);

            m_dlg2.m_update2.show_window(SW_HIDE);

            m_restart.show_window(SW_SHOW);

            m_restart.enable_window(false);

            m_update.enable_window(false);

            m_cancel.enable_window(false);

            m_update.set_window_text("Back");

            ::RedrawWindow(m_dlg2.m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

            m_pmaininterface->start_update();

         }
         else
         {

            Application.to_close();

            m_dlg2.show_window(SW_HIDE);

            m_dlg1.show_window(SW_SHOW);

            m_update.enable_window(true);

            m_cancel.enable_window(true);

            m_restart.show_window(SW_HIDE);

            m_restart.enable_window(false);

            m_update.set_window_text("Update");

            ::RedrawWindow(m_dlg2.m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

         }

      }
      else if (nId == IDC_RESTART)
      {

         m_pmaininterface->start_update();

      }
      else if (nId == IDC_UPDATE_APP)
      {

         Application.update_app();


      }


   }

   void main_dialog::on_close()
   {
      destroy_window();
      __post_quit_message(0);
   }



   INT_PTR main_dialog::on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
      if (uiMessage == WM_CTLCOLORSTATIC)
      {
         SetBkMode((HDC)wparam, TRANSPARENT);
         return (BOOL)GetSysColorBrush(NULL_BRUSH);
      }
      else if (uiMessage == WM_CTLCOLORDLG)
      {

         SelectObject((HDC)wparam, m_hbr);
         return (INT_PTR)m_hbr;

      }
      else if (uiMessage == WM_APP + 100)
      {
         int iMax = 70;
         int i = iMax * wparam / m_pmaininterface->get_total_size();
         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 20 + i);
         m_dlg2.m_uploadProgress.set_window_text(::str::from(20 + i) + "%");

      }

      return ::win32::dialog::on_dialog_message(uiMessage, wparam, lparam);

   }

//void dialog::detect()
//{

//   Application.fork([&]()
//   {
//      m_bError = false;
//      try
//      {

//         if (m_dlg2.m_pserial != NULL)
//         {

//            delete m_dlg2.m_pserial;

//            m_dlg2.m_pserial = NULL;

//         }

//      }
//      catch (...)
//      {

//      }

//      string & strCom = m_strCom;

//      string & strPath = m_strPath;

//      strCom.Empty();

//      strPath.Empty();

//      {
//         keep < bool > keepDetecting(&m_bDetecting, true, false, true);
//         m_bCancel = false;
//         m_dlg2.m_progressStatus.set_window_text("Detecting...");
//         m_dlg2.m_result.set_window_text("");
//         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 5);
//         m_dlg2.m_uploadProgress.set_window_text("5%");
//         m_dlg2.m_combo.show_window(SW_HIDE);
//         m_dlg2.m_update2.show_window(SW_HIDE);
//         m_dlg2.m_combo.send_message(CB_RESETCONTENT);
//         m_cancel.enable_window(true);
//         simple_log("");
//         simple_log("=====");
//         simple_log("Update Utility " + ::version::get_version());
//         simple_log(System.datetime().international().get_gmt_date_time());


//         string strFilePath = m_dlg1.m_pathSend;

//         string strFileName = m_dlg1.m_pathSend.name();

//         Context.file().as_memory(strFilePath, m_m);

//         logprintf("Information about file to send (1)\n");

//         logprintf("  file path: \"%s\"\n", strFilePath);
//         logprintf("  file name: \"%s\"\n", strFileName);
//         logprintf("  file size: %d bytes\n", m_m.get_size());

//         if (m_m.get_size() <= 0)
//         {

//            logprintf("File to be send has no content.");

//            m_dlg2.m_progressStatus.set_window_text("File error.");

//            m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);

//            m_dlg2.m_uploadProgress.set_window_text("");

//            m_bError = true;

//            m_dlg2.m_result.set_window_text("File empty or unaccessible.");

//            m_update.enable_window(false);

//            redraw_window();

//            return;

//         }



//         simple_log("Detecting Serial port...");



//         int iOk = 0;

//         for (index i = 0; i < 15 && !m_bCancel; i++)
//         {

//            string strDot;

//            for (index j = 0; j < (i % 4); j++)
//            {

//               strDot += ".";

//            }

//            m_dlg2.m_result.set_window_text("Waiting for New Device to Connect" + strDot);

//            {

//               m_straCom.remove_all();

//               m_straPath.remove_all();

//               list_serial_port(m_straCom, m_straPath);

//               if (m_straCom.get_size() == m_straPath.get_size() && m_straCom.has_elements())
//               {

//                  iOk = m_straCom.get_upper_bound();

//                  strCom = m_straCom[iOk];

//                  strPath = m_straPath[iOk];

//               }
//               else
//               {

//                  strCom.Empty();

//                  strPath.Empty();

//               }

//            }

//            for (int j = 0; j < 5 && !m_bCancel; j++)
//            {

//               Sleep(100);

//            }


//         }


//      }
//      if (m_strCom.has_char() && !m_bCancel)
//      {
//
//         simple_log("Detected Serial port...");

//         update();

//         return;

//      }

//      if (m_dlg2.m_pserial == NULL)
//      {
//
//         simple_log("Serial port not opened...");

//         if(m_bCancel)
//         {

//            simple_log("User cancelled device scan");

//         }

//         m_straCom.remove_all();

//         m_straPath.remove_all();

//         list_serial_port(m_straCom, m_straPath);

//         m_dlg2.m_progressStatus.set_window_text("Failed.");

//         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);

//         m_dlg2.m_uploadProgress.set_window_text("");

//         if (m_straCom.is_empty())
//         {

//            m_bError = true;

//            m_dlg2.m_result.set_window_text("Failed waiting for device to connect. No serial ports detected.");

//            m_update.enable_window(true);

//            redraw_window();

//         }
//         else
//         {

//            if (m_bCancel)
//            {

//               m_dlg2.m_result.set_window_text("Auto detection cancelled. You can try to select correct serial port manually.");

//            }
//            else
//            {

//               m_dlg2.m_result.set_window_text("Failed connecting to device. You can try to select correct serial port manually.");

//            }

//            m_dlg2.m_combo.send_message(CB_RESETCONTENT);

//            for (int i = 0; i < m_straCom.get_size(); i++)
//            {

//               m_dlg2.m_combo.send_message(CB_ADDSTRING, 0, (LPARAM) m_straCom[i].c_str());

//            }

//            m_dlg2.m_combo.send_message(CB_SETCURSEL, m_straCom.get_upper_bound());

//            m_strCom = m_straCom.last();

//            m_strPath = m_straPath.last();

//            m_dlg2.m_combo.show_window(SW_SHOW);

//            m_dlg2.m_update2.show_window(SW_SHOW);

//            m_update.enable_window(true);

//         }

//         m_bCancel = false;

//      }

//   });

//}


//void dialog::fork_update()
//{

//   if (m_strCom.is_empty())
//   {

//      if (m_dlg2.m_pserial)
//      {

//         delete m_dlg2.m_pserial;

//         m_dlg2.m_pserial = NULL;

//      }

//      m_bError = true;

//      m_dlg2.m_progressStatus.set_window_text("Device not detected.");
//      m_dlg2.m_result.set_window_text("Failed. No serial port detected.");
//      m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
//      m_dlg2.m_uploadProgress.set_window_text("");
//      m_cancel.enable_window(true);
//      m_update.enable_window(true);
//      m_cancel.set_window_text("Close");
//
//      redraw_window();

//      m_dlg2.m_iCom = -1;

//      return;

//   }


//   Application.fork([=]()
//   {

//      update();

//   });

//}

#ifdef RASPBERRY

   void dialog::update()
   {

      int iLine;
      try
      {

         if (m_dlg2.m_pserial == NULL)
         {

            m_dlg2.m_pserial = new ::serial::Serial(get_context_application(), "", 115200, ::serial::Timeout::simpleTimeout(1000));

            m_dlg2.m_pserial->setPort(m_strCom);

            m_dlg2.m_pserial->open();

         }

      }
      catch (...)
      {
         delete m_dlg2.m_pserial;
         m_dlg2.m_pserial = NULL;
         m_bError = true;
         m_dlg2.m_progressStatus.set_window_text("Device not detected.");
         m_dlg2.m_result.set_window_text("Failed. Could not open connection.");
         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
         m_dlg2.m_uploadProgress.set_window_text("");
         m_cancel.enable_window(true);
         m_cancel.set_window_text("Close");
         redraw_window();
         return;

      }
      m_dlg2.m_progressStatus.set_window_text(m_strCom + " Detected");
      m_dlg2.m_result.set_window_text("");
      m_dlg2.m_progressControl.send_message(PBM_SETPOS, 15);
      m_dlg2.m_uploadProgress.set_window_text("15%");
      m_dlg2.m_combo.show_window(SW_HIDE);
      m_dlg2.m_update2.show_window(SW_HIDE);
      m_dlg2.m_combo.send_message(CB_RESETCONTENT);
      m_cancel.enable_window(true);
      simple_log("Serial port " + m_strCom + " opened...");

      Sleep(100);
      //      m_dlg2.m_pserial->setTimeout();
      simple_log("15% Writing Enter Key");
      string str1 = "\n";
      m_dlg2.m_pserial->write(str1);
      int iState = 1;
      iLine = 0;
      simple_log("Waiting First Screen");
      while (iLine < 20 && iState != 5)
      {

         string strLine = m_dlg2.m_pserial->readline();
         simple_log("Current State " + ::str::from(iState));
         simple_log("readline: " + strLine);
         m_dlg2.m_result.set_window_text(strLine);

         //         if (iState == 1 && strLine.contains_ci("in-application programming application"))
         //     {
         //          iState = 2;
         //   }
         // else if (iState == 2 && strLine.contains_ci("by mcd application team"))
         //{
         // iState = 3;
         //}
         if (iState == 1 && strLine.contains_ci("login:"))
         {
            Sleep(100);
            m_dlg2.m_pserial->write(string("p"));
            Sleep(100);
            m_dlg2.m_pserial->write(string("i"));
            Sleep(100);
            m_dlg2.m_pserial->write(string("\n"));
            simple_log("State 2");
            iState = 2;
         }
         else if (iState == 2 && strLine.contains_ci("password"))
         {
            string str;
            str = "raspberry\n";
            for (int i = 0; i < str.get_length(); i++)
            {
               Sleep(100);
               m_dlg2.m_pserial->write(str);
            }
            simple_log("State 3");
            iState = 3;
         }

         Sleep(100);

         iLine++;
      }

      if (iState == 3)
      {

         iState = -1;

         simple_log("Choosing Option 1");

         string str1 = "rb\n";
         m_dlg2.m_pserial->write(str1);

         Context.file().as_memory(m_dlg1.m_pathSend, m_m);

         simple_log("Sending File");

         string strFileName = m_dlg1.m_pathSend.name();
         //iState = //ymodem_send(this, m_m, m_dlg1.m_pathSend.name(), PROTOCOL_YMODEM, 0);
         iState = ymodem_send(m_m.get_data(), m_m.get_size(), (char *)(const char*)strFileName);

         if (iState < 0)
         {
            delete m_dlg2.m_pserial;
            m_dlg2.m_pserial = NULL;
            m_bError = true;
            simple_log("xymodem_send failed with error code = " + ::str::from(iState));
            m_dlg2.m_progressStatus.set_window_text("Upload Error.");
            m_dlg2.m_result.set_window_text("Failed. File Transfer error.");
            m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
            m_dlg2.m_uploadProgress.set_window_text("");
            m_cancel.enable_window(true);
            m_cancel.set_window_text("Close");
            redraw_window();
            return;

         }

      }
      else
      {

         iState = -1;

      }

      Sleep(500);
      //string str1 = "\n";
      //m_dlg2.m_pserial->write(str1);

      if (iState >= 0)
      {

         iState = 1;

         simple_log("Waiting Response of File Transfer");

         string strRName;
         int iRLen = -1;
         iLine = 0;
         while (iLine < 15)
         {

            string strLine = m_dlg2.m_pserial->readline();
            simple_log("Current State " + ::str::from(iState));
            simple_log("readline: " + strLine);
            m_dlg2.m_result.set_window_text(strLine);

            if (iState == 1 && strLine.contains_ci("programming completed successfully"))
            {
               iState = 2;
            }
            else if (iState == 2 && strLine.contains_ci("name:"))
            {
               ::str::begins_eat_ci(strLine, "name:");
               strRName = strLine;
               strRName.trim();
               iState = 3;
            }
            else if (iState == 3 && strLine.contains_ci("size:"))
            {
               strLine.trim();
               ::str::begins_eat_ci(strLine, "size:");
               ::str::ends_eat_ci(strLine, "bytes");
               strLine.trim();
               iRLen = atoi(strLine);
               iState = 4;

               if (strRName.compare_ci(m_dlg1.m_pathSend.name()) != 0 || iRLen != m_m.get_size())
               {
                  delete m_dlg2.m_pserial;
                  m_dlg2.m_pserial = NULL;
                  m_bError = true;
                  m_dlg2.m_progressStatus.set_window_text("Upload Error.");
                  m_dlg2.m_result.set_window_text("File name or size incorrect.");
                  m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
                  m_dlg2.m_uploadProgress.set_window_text("");
                  m_cancel.enable_window(true);
                  m_cancel.set_window_text("Close");
                  return;
               }
               m_dlg2.m_progressStatus.set_window_text("Upload Success.");
               m_dlg2.m_result.set_window_text("Upload progress 100%.");
               m_dlg2.m_progressControl.send_message(PBM_SETPOS, 90);
               m_dlg2.m_uploadProgress.set_window_text("90%");
               m_cancel.enable_window(true);
               m_cancel.set_window_text("Close");

            }
            Sleep(100);
            iLine++;
         }
      }

      if (iState == 4)
      {

         iState = 1;

         simple_log("Waiting for Main Menu (2)");

         while (true)
         {

            string strLine = m_dlg2.m_pserial->readline();
            simple_log("Current State " + ::str::from(iState));
            simple_log("readline: " + strLine);
            m_dlg2.m_result.set_window_text(strLine);

            if (iState == 1 && strLine.contains_ci("Main Menu"))
            {
               iState = 2;
            }
            else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
            {
               iState = 3;
            }
            else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
            {
               iState = 4;
            }
            else if (iState == 4 && strLine.contains("====="))
            {
               iState = 5;
               strLine = m_dlg2.m_pserial->readline();
               m_dlg2.m_progressStatus.set_window_text("Activating.");
               m_dlg2.m_result.set_window_text("Activating update.");
               m_dlg2.m_progressControl.send_message(PBM_SETPOS, 95);
               m_dlg2.m_uploadProgress.set_window_text("95%");
               m_cancel.enable_window(true);
               m_cancel.set_window_text("Close");
            }

            Sleep(100);
            iLine++;
         }

      }

      if (iState == 5)
      {

         simple_log("Writing Option 2");

         string str2 = "2";
         m_dlg2.m_pserial->write(str2);

         simple_log("Waiting for Main Menu (3)");

         iState = 1;
         while (true)
         {

            string strLine = m_dlg2.m_pserial->readline();
            simple_log("Current State " + ::str::from(iState));
            simple_log("readline: " + strLine);
            m_dlg2.m_result.set_window_text(strLine);

            if (iState == 1 && strLine.contains_ci("Main Menu"))
            {
               iState = 2;
            }
            else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
            {
               iState = 3;
            }
            else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
            {
               iState = 4;
            }
            else if (iState == 4 && strLine.contains("====="))
            {
               iState = 5;
               strLine = m_dlg2.m_pserial->readline();
               m_dlg2.m_progressControl.send_message(PBM_SETPOS, 100);
               m_dlg2.m_progressControl.redraw_window();
               m_dlg2.m_uploadProgress.set_window_text("100%");
               break;
            }

            Sleep(100);
            iLine++;
         }

      }

      if (m_dlg2.m_pserial)
      {
         delete m_dlg2.m_pserial;
         m_dlg2.m_pserial = NULL;
      }

      if (iState == 5)
      {

         simple_log("Success");

         m_bError = false;
         m_dlg2.m_progressStatus.set_window_text("Success!");
         m_dlg2.m_result.set_window_text("Device update completed succesfully!");
         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 100);
         m_dlg2.m_uploadProgress.set_window_text("100%");
         m_cancel.enable_window(true);
         m_update.enable_window(true);
         m_cancel.set_window_text("Close");
         redraw_window();
         m_dlg2.m_iCom = -1;

      }
      else
      {
         simple_log("Failed");
         if (m_dlg2.m_pserial)
         {
            delete m_dlg2.m_pserial;
            m_dlg2.m_pserial = NULL;
         }
         m_bError = true;
         m_dlg2.m_progressStatus.set_window_text("Protocol Error");
         m_dlg2.m_result.set_window_text("Failed to update device.");
         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
         m_dlg2.m_uploadProgress.set_window_text("");
         m_cancel.enable_window(true);
         m_update.enable_window(true);
         m_cancel.set_window_text("Cancel");
         redraw_window();
         m_dlg2.m_iCom = -1;

      }

   }

#else

//void dialog::update()
//{

//   int iLine;

//   try
//   {

//      if (m_dlg2.m_pserial == NULL)
//      {

//         m_dlg2.m_pserial = new ::serial::Serial(get_context_application(), m_strCom, 115200, ::serial::Timeout::simpleTimeout(1000));

//      }

//   }
//   catch (...)
//   {

//      delete m_dlg2.m_pserial;

//      m_dlg2.m_pserial = NULL;

//      m_bError = true;
//      m_dlg2.m_progressStatus.set_window_text("Device not detected.");
//      m_dlg2.m_result.set_window_text("Failed. Could not open connection.");
//      m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
//      m_dlg2.m_uploadProgress.set_window_text("");
//      m_cancel.enable_window(true);
//      m_cancel.set_window_text("Close");
//      redraw_window();

//      return;

//   }

//   m_dlg2.m_progressStatus.set_window_text(m_strCom + " Detected");
//   m_dlg2.m_result.set_window_text("Uploading Firmware");
//   m_dlg2.m_progressControl.send_message(PBM_SETPOS, 15);
//   m_dlg2.m_uploadProgress.set_window_text("15%");
//   m_dlg2.m_combo.show_window(SW_HIDE);
//   m_dlg2.m_update2.show_window(SW_HIDE);
//   m_dlg2.m_combo.send_message(CB_RESETCONTENT);

//   m_cancel.enable_window(true);

//   simple_log("Serial port " + m_strCom + " opened...");

//   /* Flush the RX FIFO, after a cool off delay */
//   Sleep(1000);
//   while (serial_read() >= 0);

//   simple_log("Flushing serial read");

//   simple_log("15% Writing Enter Key");

//   string str1 = "\n";

//   m_dlg2.m_pserial->write(str1);

//   int iState = 1;

//   iLine = 0;

//   simple_log("Waiting First Screen");

//   while (iLine < 15)
//   {

//      string strLine = m_dlg2.m_pserial->readline();

//      simple_log("current state: " + ::str::from(iState));

//      simple_log("     readline: " + strLine);

//      //m_dlg2.m_result.set_window_text(strLine);

//      if (iState == 1 && strLine.contains_ci("Main Menu"))
//      {
//
//         simple_log("       state=> 2");

//         iState = 2;

//      }
//      else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
//      {

//         simple_log("       state=> 3");

//         iState = 3;

//      }
//      else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
//      {

//         simple_log("       state=> 4");

//         iState = 4;

//      }
//      else if (iState == 4 && strLine.contains("====="))
//      {

//         simple_log("       state=> 5");

//         iState = 5;

//         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 20);

//         m_dlg2.m_uploadProgress.set_window_text("20%");

//         m_dlg2.m_progressControl.redraw_window();

//         Sleep(500);

//         break;

//      }

//      Sleep(100);

//      iLine++;

//   }

//   string strFilePath = m_dlg1.m_pathSend;

//   string strFileName = m_dlg1.m_pathSend.name();

//   if (iState == 5)
//   {

//      iState = -1;

//      simple_log("Choosing Option 1");

//      string str1 = "1";

//      m_dlg2.m_pserial->write(str1);

//      logprintf("Information about file to send (2)\n");

//      logprintf("  file path: \"%s\"\n", strFilePath);

//      logprintf("  file name: \"%s\"\n", strFileName);

//      logprintf("  file size: %d bytes\n", m_m.get_size());

//      iState = ymodem_send(m_m.get_data(), m_m.get_size(), (char *)(const char*)strFileName, m_hwnd);

//      if (iState == 0 || iState != m_m.get_size())
//      {

//         delete m_dlg2.m_pserial;

//         m_dlg2.m_pserial = NULL;

//         m_bError = true;

//         simple_log("ymodem_send failed with result " + ::str::from(iState));

//         m_dlg2.m_progressStatus.set_window_text("Upload Error.");

//         m_dlg2.m_result.set_window_text("Failed. File Transfer error.");

//         m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);

//         m_dlg2.m_uploadProgress.set_window_text("");
//
//         m_cancel.enable_window(true);

//         m_cancel.set_window_text("Close");

//         redraw_window();

//         return;

//      }

//   }
//   else
//   {

//      iState = -1;

//   }

//   Sleep(500);

//   if (iState > 0)
//   {

//      iState = 1;

//      simple_log("Waiting Response of File Transfer");

//      string strRName;

//      int iRLen = -1;

//      iLine = 0;

//      while (iLine < 15)
//      {

//         string strLine = m_dlg2.m_pserial->readline();

//         simple_log("current state: " + ::str::from(iState));

//         simple_log("     readline: " + strLine);

//         //m_dlg2.m_result.set_window_text(strLine);

//         if (iState == 1 && strLine.contains_ci("programming completed successfully"))
//         {
//
//            simple_log("       state=> 2");

//            iState = 2;

//         }
//         else if (iState == 2 && strLine.contains_ci("name:"))
//         {
//
//            strLine.trim();

//            ::str::begins_eat_ci(strLine, "name:");
//
//            strRName = strLine;

//            strRName.trim();

//            simple_log("       state=> 3");

//            simple_log("programmed name: \"" + strRName + "\"");

//            iState = 3;

//         }
//         else if (iState == 3 && strLine.contains_ci("size:"))
//         {

//            strLine.trim();

//            simple_log("       state=> 4");

//            ::str::begins_eat_ci(strLine, "size:");

//            ::str::ends_eat_ci(strLine, "bytes");

//            iRLen = atoi(strLine);

//            simple_log("programmed bytes: " + ::str::from(iRLen));

//            iState = 4;

//            if (strRName.compare_ci(strFileName) != 0 || iRLen != m_m.get_size())
//            {

//               delete m_dlg2.m_pserial;

//               m_dlg2.m_pserial = NULL;

//               m_bError = true;
//               simple_log("Programmed file name or programmed file size incorrect.");
//               m_dlg2.m_progressStatus.set_window_text("Upload Error.");
//               m_dlg2.m_result.set_window_text("File name or size incorrect.");
//               m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
//               m_dlg2.m_uploadProgress.set_window_text("");
//               m_cancel.enable_window(true);
//               m_cancel.set_window_text("Close");
//
//               return;

//            }

//            m_dlg2.m_progressStatus.set_window_text("Upload Success.");
//            m_dlg2.m_result.set_window_text("Finalizing...");
//            m_dlg2.m_progressControl.send_message(PBM_SETPOS, 95);
//            m_dlg2.m_uploadProgress.set_window_text("95%");
//            m_cancel.enable_window(true);
//            m_cancel.set_window_text("Close");

//            break;

//         }

//         Sleep(100);

//         iLine++;

//      }

//   }

//   if (iState == 4)
//   {

//      iState = 1;

//      simple_log("Waiting for Main Menu (2)");

//      iLine = 0;

//      while (iLine < 20)
//      {

//         string strLine = m_dlg2.m_pserial->readline();
//
//         simple_log("current state: " + ::str::from(iState));
//
//         simple_log("     readline: " + strLine);

//         //m_dlg2.m_result.set_window_text(strLine);

//         if (iState == 1 && strLine.contains_ci("Main Menu"))
//         {

//            simple_log("       state=> 2");

//            iState = 2;

//         }
//         else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
//         {

//            simple_log("       state=> 3");

//            iState = 3;

//         }
//         else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
//         {

//            simple_log("       state=> 4");

//            iState = 4;

//         }
//         else if (iState == 4 && strLine.contains("====="))
//         {

//            simple_log("       state=> 5");

//            iState = 5;
//            break;

//         }

//         Sleep(100);

//         iLine++;

//      }

//   }

//   if (iState == 5)
//   {

//      iState = 1;

//      m_dlg2.m_progressStatus.set_window_text("Activating.");
//      m_dlg2.m_result.set_window_text("Activating update.");

//      m_cancel.enable_window(true);

//      m_cancel.set_window_text("Close");

//      simple_log("100% Writing Option 2");

//      string str2 = "2";

//      m_dlg2.m_pserial->write(str2);

//      iState = 2;

//      //simple_log("Waiting for Main Menu (3)");

//      //iLine = 0;

//      //while (iLine < 15)
//      //{

//      //   string strLine = m_dlg2.m_pserial->readline();

//      //   simple_log("current state:" + ::str::from(iState));

//      //   simple_log("     readline: " + strLine);

//      //   m_dlg2.m_result.set_window_text(strLine);

//      //   if (iState == 1 && strLine.contains_ci("Main Menu"))
//      //   {

//      //      simple_log("       state=> 2");

//      //      iState = 2;

//      //   }
//      //   else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
//      //   {

//      //      simple_log("       state=> 3");

//      //      iState = 3;

//      //   }
//      //   else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
//      //   {
//      //
//      //      simple_log("       state=> 4");

//      //      iState = 4;

//      //   }
//      //   else if (iState == 4 && strLine.contains("====="))
//      //   {

//      //      simple_log("       state=> 5");

//      //      iState = 5;

//      //      strLine = m_dlg2.m_pserial->readline();

//      //      m_dlg2.m_progressControl.send_message(PBM_SETPOS, 100);

//      //      m_dlg2.m_progressControl.redraw_window();

//      //      m_dlg2.m_uploadProgress.set_window_text("100%");

//      //      break;

//      //   }

//      //   Sleep(100);

//      //   iLine++;

//      //}

//   }

//   if (m_dlg2.m_pserial)
//   {
//
//      delete m_dlg2.m_pserial;

//      m_dlg2.m_pserial = NULL;

//   }

//   if (iState == 2)
//   {

//      simple_log("Success");

//      m_bError = false;

//      m_dlg2.m_progressStatus.set_window_text("Success!");
//      m_dlg2.m_result.set_window_text("Device update completed succesfully!");
//      m_dlg2.m_progressControl.send_message(PBM_SETPOS, 100);
//      m_dlg2.m_uploadProgress.set_window_text("100%");
//      m_cancel.enable_window(true);
//      m_update.enable_window(true);
//      m_cancel.set_window_text("Close");
//
//      redraw_window();

//      m_dlg2.m_iCom = -1;

//   }
//   else
//   {
//
//      simple_log("Failed");

//      if (m_dlg2.m_pserial)
//      {
//
//         delete m_dlg2.m_pserial;
//         m_dlg2.m_pserial = NULL;

//      }

//      m_bError = true;

//      m_dlg2.m_progressStatus.set_window_text("Protocol Error");
//      m_dlg2.m_result.set_window_text("Failed to update device.");
//      m_dlg2.m_progressControl.send_message(PBM_SETPOS, 0);
//      m_dlg2.m_uploadProgress.set_window_text("");
//      m_cancel.enable_window(true);
//      m_update.enable_window(true);
//      m_cancel.set_window_text("Cancel");
//
//      redraw_window();

//      m_dlg2.m_iCom = -1;

//   }

//}

#endif


} // namespace grbl_merge_tool



