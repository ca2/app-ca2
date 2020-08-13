#include "framework.h"

#define TEST_DOWNLOAD_APP 0
#ifdef LINUX
extern "C"
gboolean linux_main_window(gpointer pdata);
//GMainContext * gtk_main_context;
#endif
//#include <libudev.h>
//#include <stdio.h>
//#include <usb.h>

//static struct udev_device*
//get_child(
//     struct udev* udev, struct udev_device* parent, const char* subsystem)
//{
//  struct udev_device* child = NULL;
//  struct udev_enumerate *enumerate = udev_enumerate_new(udev);
//
//  udev_enumerate_add_match_parent(enumerate, parent);
//  udev_enumerate_add_match_subsystem(enumerate, subsystem);
//  udev_enumerate_scan_devices(enumerate);
//
//  struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
//  struct udev_list_entry *entry;
//
//  udev_list_entry_foreach(entry, devices) {
//    const char *path = udev_list_entry_get_name(entry);
//    child = udev_device_new_from_syspath(udev, path);
//    break;
//  }
//
//  udev_enumerate_unref(enumerate);
//  return child;
//}
//
//static void enumerate_usb_mass_storage(struct udev* udev)
//{
//  struct udev_enumerate* enumerate = udev_enumerate_new(udev);
//
//  udev_enumerate_add_match_subsystem(enumerate, "scsi");
//  udev_enumerate_add_match_property(enumerate, "DEVTYPE", "scsi_device");
//  udev_enumerate_scan_devices(enumerate);
//
//  struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
//  struct udev_list_entry *entry;
//
//  udev_list_entry_foreach(entry, devices) {
//    const char* path = udev_list_entry_get_name(entry);
//    struct udev_device* scsi = udev_device_new_from_syspath(udev, path);
//
//    struct udev_device* block = get_child(udev, scsi, "block");
//    struct udev_device* scsi_disk = get_child(udev, scsi, "scsi_disk");
//
//    struct udev_device* usb
//      = udev_device_get_parent_with_subsystem_devtype(
//          scsi, "usb", "usb_device");
//
//    if (block && scsi_disk && usb) {
//      string strBlock = udev_device_get_devnode(block);
//      string strVendor = udev_device_get_sysattr_value(usb, "idVendor");
//      string strProduct = udev_device_get_sysattr_value(usb, "idProduct");
//      string strVend = udev_device_get_sysattr_value(usb, "vendor");
//        printf("block = %s, usb = %s:%s, scsi = %s\n",
//          udev_device_get_devnode(block),
//          udev_device_get_sysattr_value(usb, "idVendor"),
//          udev_device_get_sysattr_value(usb, "idProduct"),
//          udev_device_get_sysattr_value(scsi, "vendor"));
//    }
//
//    if (block)
//      udev_device_unref(block);
//
//    if (scsi_disk)
//      udev_device_unref(scsi_disk);
//
//    udev_device_unref(scsi);
//  }
//
//  udev_enumerate_unref(enumerate);
//}
//

namespace version
{

   int compare(string str1, string str2)
   {

      stringa stra1;

      stringa stra2;

      stra1.explode(".", str1);

      stra2.explode(".", str2);

      int iCmp = 0;

      for (int i = 0; i < MIN(stra1.get_size(), stra2.get_size()); i++)
      {

         iCmp = atoi(stra1[i]) - atoi(stra2[i]);

         if(iCmp != 0)
         {

            return iCmp;

         }

      }

      if (iCmp == 0)
      {

         iCmp = (int) (stra1.get_size() - stra2.get_size());

      }

      return iCmp;

   }

} // namespace version


namespace grbl_merge_tool
{

   application * g_papp = NULL;

   ::serial::Serial * application::m_pserial = NULL;

   application::application()
   {
      m_bDetecting = false;
      m_bError = false;

//      m_iCom = -1;
      m_pserial = NULL;

      g_papp = this;
      m_iCom = -1;
      init_printf(NULL, &simple_log_putf);

      m_strAppName = "app/grbl_merge_tool";
      m_strLibraryName = "app/grbl_merge_tool";
      m_strBaseSupportId = "ca2_flag";
      m_bLicense = false;

   }

   void total_progress(int iTotal)
   {

      g_papp->total_progress(iTotal);

   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

//     struct udev* udev = udev_new();
//
//  enumerate_usb_mass_storage(udev);
//
//  udev_unref(udev);
//
//
//
//    struct usb_bus *bus;
//    struct usb_device *dev;
//    usb_init();
//    usb_find_busses();
//    usb_find_devices();
//    for (bus = usb_busses; bus; bus = bus->next)
//        for (dev = bus->devices; dev; dev = dev->next){
//            printf("Trying device %s/%s\n", bus->dirname, dev->filename);
//            printf("\tID_VENDOR = 0x%04x\n", dev->descriptor.idVendor);
//            printf("\tID_PRODUCT = 0x%04x\n", dev->descriptor.idProduct);
//        }


      if (!::aura::application::init_instance())
      {

         return false;

      }

      return true;

   }


   void application::term_instance()
   {

      ::aura::application::term_instance();

   }

//   void application::redraw_window()
//   {
//
//   }

   void application::on_request(::create * pcreate)
   {

#if 0

#ifdef _DEBUG

      output_debug_string("_DEBUG build? (basis)");

      ASSERT(FALSE);

#else

      output_debug_string("RELEASE build? (stage)");

      ASSERT(FALSE);

#endif

#endif


#ifdef WINDOWS_DESKTOP

      fork([&]()
      {

         output_debug_string("\nfinished grbl_merge_tool::on_request");

         property_set set;

         set["raw_http"] = true;
         set["disable_common_name_cert_check"] = true;

         main_dialog dlg(this);

         m_pmainwindow = &dlg;

         dlg.create_dialog();

         rect r;
         GetWindowRect(GetDesktopWindow(), r);
         rect r2;
         GetWindowRect(dlg.m_hwnd, r2);

         r2.Align(align_horizontal_center, r);

         r2.top = (r.height() - r2.height()) / 3;

         SetWindowPos(dlg.m_hwnd, HWND_TOP, r2.left, r2.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);

         dlg.show_window(SW_RESTORE);

         BOOL ret;
         MSG msg;
         while ((ret = GetMessage(&msg, 0, 0, 0)) != 0)
         {
            if (ret == -1) /* error found */
               return;

            if (!IsDialogMessage(dlg.m_hwnd, &msg))
            {
               TranslateMessage(&msg); /* translate virtual-key messages */
               DispatchMessage(&msg); /* send it to dialog procedure */
            }
         }

         System.set_finish();

      });
#elif defined(MACOS)

      show_main_dialog();

      app_check_version();

#elif defined(LINUX)


      auto idle_source = g_idle_source_new();

      g_source_set_callback(idle_source, &linux_main_window, (::aura::application *) this, NULL);

      g_source_attach(idle_source, gtk_main_context);


#endif

   }





   i64 application::add_ref()
   {

      return ::object::add_ref();

   }

   i64 application::dec_ref()
   {

      return ::object::dec_ref();

   }


   void application::detect()
   {

      fork([&]()
      {

         m_bError = false;

         try
         {

            if (m_pserial != NULL)
            {

               delete m_pserial;

               m_pserial = NULL;

            }

         }
         catch (...)
         {

         }

         m_portinfo.port.Empty();
         m_portinfo.description.Empty();
         m_portinfo.hardware_id.Empty();


         {
            keep < bool > keepDetecting(&m_bDetecting, true, false, true);
            m_bCancel = false;
            to_cancel();
            ::aura::del(m_pserial);
            string strFilePath = m_pathSend;

            string strFileName = m_pathSend.name();

            Context.file().as_memory(strFilePath, m_m);

            logprintf("Information about file to send (1)\n");

            logprintf("  file path: \"%s\"\n", strFilePath.c_str());
            logprintf("  file name: \"%s\"\n", strFileName.c_str());
            logprintf("  file size: %d bytes\n", m_m.get_size());

            simple_log("");
            simple_log("=====");
            simple_log("Update Utility " + ::version::get_version());
            simple_log(System.datetime().international().get_gmt_date_time());
            cancel_enable(true);
            set_progress(0);
            set_progress_status("Detecting...");
            set_result("");



            if (m_m.get_size() <= 0)
            {

               logprintf("File to be send has no content.");

               set_progress_status("File error.");

               set_progress(0);

               m_bError = true;

               set_result("File empty or unaccessible.");

               cancel_enable(true);

               redraw_window();

               return;

            }

            simple_log("Detecting Serial port...");

            m_strDetectedOrSelected = "Detected";

            int iOk = 0;

            for (index i = 0; i < 15 && !m_bCancel; i++)
            {

               string strDot;

               for (index j = 0; j < (i % 4); j++)
               {

                  strDot += ".";

               }

               set_result("Waiting for New Device to Connect" + strDot);

               {

                  list_serial_ports();

                  if (m_portinfoa.has_elements())
                  {

#ifdef MACOS

                     iOk = 0;


#else

                     iOk = m_portinfoa.get_upper_bound();


#endif

                     m_portinfo = m_portinfoa[iOk];

                     break;

                  }
                  else
                  {

                     m_portinfo.port.Empty();
                     m_portinfo.description.Empty();
                     m_portinfo.hardware_id.Empty();

                  }

               }

               for (int j = 0; j < 5 && !m_bCancel; j++)
               {

                  Sleep(35);

               }


            }


         }

         if (m_portinfo.port.has_char() && !m_bCancel)
         {

            simple_log("Detected Serial port...");

            simple_log("port: " + m_portinfo.port);

            update();

            return;

         }

         to_close();

         if(m_bCancel)
         {

            simple_log("User cancelled device scan");

         }
         else
         {

            simple_log("No serial ports not detected");

         }

         list_serial_ports();

         set_progress_status("Failed.");

         set_progress(0);

         if (m_portinfoa.is_empty())
         {

            m_bError = true;

            set_result("Failed waiting for device to connect. No serial ports detected.");

            update_enable(true);

            redraw_window();

         }
         else
         {

            if (m_bCancel)
            {

               set_result("Auto detection cancelled. You can try to select correct serial port manually.");

            }
            else
            {

               set_result("Failed connecting to device. You can try to select correct serial port manually.");

            }

            manual_com();

         }

      });

   }

   void application::fork_update()
   {

      if (m_portinfo.port.is_empty())
      {

         if (m_pserial)
         {

            delete m_pserial;

            m_pserial = NULL;

         }

         m_bError = true;

         set_progress_status("Device not detected.");
         set_result("Failed. No serial port detected.");
         set_progress(0);
         cancel_enable(true);
         update_enable(true);
         to_close();

         redraw_window();

         m_iCom = -1;

         return;

      }


      Application.fork([=]()
      {

         update();

      });

   }

#ifdef RASPBERRY

   void application::update()
   {

      int iLine;
      try
      {

         if (m_pserial == NULL)
         {

            m_pserial = new ::serial::Serial(get_context_application(), "", 115200, ::serial::Timeout::simpleTimeout(1000));

            m_pserial->setPort(m_strCom);

            m_pserial->open();

         }

      }
      catch (...)
      {
         delete m_pserial;
         m_pserial = NULL;
         m_bError = true;
         set_progress_status("Device not detected.");
         set_result("Failed. Could not open connection.");
         set_progress(0);
         m_dlg2.m_uploadProgress.set_window_text("");
         cancel_enable(true);
         m_cancel.set_window_text("Close");
         redraw_window();
         return;

      }
      set_progress_status(m_strCom + m_strDetectedOrSelected);
      set_result("");
      set_progress(15);
      m_dlg2.m_uploadProgress.set_window_text("15%");
      m_dlg2.m_combo.show_window(SW_HIDE);
      m_dlg2.m_update2.show_window(SW_HIDE);
      m_dlg2.m_combo.send_message(CB_RESETCONTENT);
      cancel_enable(true);
      simple_log("Serial port " + m_strCom + " opened...");

      Sleep(100);
      //      m_pserial->setTimeout();
      simple_log("15% Writing Enter Key");
      string str1 = "\n";
      m_pserial->write(str1);
      int iState = 1;
      iLine = 0;
      simple_log("Waiting First Screen");
      while (iLine < 20 && iState != 5)
      {

         string strLine = m_pserial->readline();
         simple_log("Current State " + ::str::from(iState));
         simple_log("readline: " + strLine);
         set_result(strLine);

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
            m_pserial->write(string("p"));
            Sleep(100);
            m_pserial->write(string("i"));
            Sleep(100);
            m_pserial->write(string("\n"));
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
               m_pserial->write(str);
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
         m_pserial->write(str1);

         Context.file().as_memory(m_dlg1.m_pathSend, m_m);

         simple_log("Sending File");

         string strFileName = m_dlg1.m_pathSend.name();
         //iState = //ymodem_send(this, m_m, m_dlg1.m_pathSend.name(), PROTOCOL_YMODEM, 0);
         iState = ymodem_send(m_m.get_data(), m_m.get_size(), (char *)(const char*) strFileName);

         if (iState < 0)
         {
            delete m_pserial;
            m_pserial = NULL;
            m_bError = true;
            simple_log("xymodem_send failed with error code = " + ::str::from(iState));
            set_progress_status("Upload Error.");
            set_result("Failed. File Transfer error.");
            set_progress(0);
            m_dlg2.m_uploadProgress.set_window_text("");
            cancel_enable(true);
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
      //m_pserial->write(str1);

      if(iState >= 0)
      {

         iState = 1;

         simple_log("Waiting Response of File Transfer");

         string strRName;
         int iRLen = -1;
         iLine = 0;
         while (iLine < 15)
         {

            string strLine = m_pserial->readline();
            simple_log("Current State " + ::str::from(iState));
            simple_log("readline: " + strLine);
            set_result(strLine);

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
                  delete m_pserial;
                  m_pserial = NULL;
                  m_bError = true;
                  set_progress_status("Upload Error.");
                  set_result("File name or size incorrect.");
                  set_progress(0);
                  m_dlg2.m_uploadProgress.set_window_text("");
                  cancel_enable(true);
                  m_cancel.set_window_text("Close");
                  return;
               }
               set_progress_status("Upload Success.");
               set_result("Upload progress 100%.");
               set_progress(90);
               m_dlg2.m_uploadProgress.set_window_text("90%");
               cancel_enable(true);
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

            string strLine = m_pserial->readline();
            simple_log("Current State " + ::str::from(iState));
            simple_log("readline: " + strLine);
            set_result(strLine);

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
               strLine = m_pserial->readline();
               set_progress_status("Activating.");
               set_result("Activating update.");
               set_progress(95);
               m_dlg2.m_uploadProgress.set_window_text("95%");
               cancel_enable(true);
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
         m_pserial->write(str2);

         simple_log("Waiting for Main Menu (3)");

         iState = 1;
         while (true)
         {

            string strLine = m_pserial->readline();
            simple_log("Current State " + ::str::from(iState));
            simple_log("readline: " + strLine);
            set_result(strLine);

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
               strLine = m_pserial->readline();
               set_progress(100);
               m_dlg2.m_progressControl.redraw_window();
               m_dlg2.m_uploadProgress.set_window_text("100%");
               break;
            }

            Sleep(100);
            iLine++;
         }

      }

      if (m_pserial)
      {
         delete m_pserial;
         m_pserial = NULL;
      }

      if (iState == 5)
      {

         simple_log("Success");

         m_bError = false;
         set_progress_status("Success!");
         set_result("Device update completed succesfully!");
         set_progress(100);
         m_dlg2.m_uploadProgress.set_window_text("100%");
         cancel_enable(true);
         update_enable(true);
         m_cancel.set_window_text("Close");
         redraw_window();
         m_dlg2.m_iCom = -1;

      }
      else
      {
         simple_log("Failed");
         if (m_pserial)
         {
            delete m_pserial;
            m_pserial = NULL;
         }
         m_bError = true;
         set_progress_status("Protocol Error");
         set_result("Failed to update device.");
         set_progress(0);
         m_dlg2.m_uploadProgress.set_window_text("");
         cancel_enable(true);
         update_enable(true);
         m_cancel.set_window_text("Cancel");
         redraw_window();
         m_dlg2.m_iCom = -1;

      }

   }

#else

   void application::update()
   {

      {

         keep < bool > keepUpdating(&m_bUpdating, true, false, true);

         m_bError = false;
         to_cancel();
         update_enable(false);
         int iLine;

         bool bGeneralException = false;

         try
         {

            if (m_pserial == NULL)
            {

               m_pserial = new ::serial::Serial(get_context_application(), m_portinfo.port, 115200, ::serial::Timeout::simpleTimeout(1000));

            }

         }
         catch (invalid_argument_exception *pe)
         {

            string strResult = string("Failed. ") + pe->get_message();

            //fork([=]()
            //{

            ::aura::del(m_pserial);

            m_bError = true;
            set_progress_status("Error opening serial.");
            set_result(strResult);
            set_progress(0);
            cancel_enable(true);
            to_close();
            manual_com();
            redraw_window();

            //});

            return;

         }
         catch (serial::SerialException *pe)
         {

            string strResult = string("Failed. ") + pe->what();

            //fork([=]()
            //{

            ::aura::del(m_pserial);

            m_bError = true;
            set_progress_status("Error opening serial.");
            set_result(strResult);
            set_progress(0);
            cancel_enable(true);
            to_close();
            manual_com();
            redraw_window();

            //});

            return;

         }
         catch (serial::IOException * pe)
         {

            string strResult = string("Failed. ") + pe->what();

            //fork([=]()
            //{
            string strExtra;

            #ifdef RASPBIAN


            if(pe->getErrorNumber() == 13)
            {

            if(::system("sudo chmod 777 " + m_portinfo.port) == 0)
            {

               strExtra = "Permissions fixed for " + m_portinfo.port +". Please try again.";

            }
            else{

              strExtra = "Unable to fixed permissions for "+m_portinfo.port+".";

              }

            }

            #endif

            ::aura::del(m_pserial);

            m_bError = true;
            set_progress_status("Error opening serial.");
            set_result(strResult + " " + strExtra);
            set_progress(0);
            cancel_enable(true);
            to_close();
            manual_com();
            redraw_window();

            //});

            return;

         }
         catch (...)
         {

            //fork([&]()
            //{

            ::aura::del(m_pserial);

            m_bError = true;
            set_progress_status("Device not detected.");
            set_result("Failed. Could not open connection.");
            set_progress(0);
            cancel_enable(true);
            to_close();
            manual_com();
            redraw_window();

            //});

            return;

         }

         set_progress_status(m_portinfo.port + " " + m_strDetectedOrSelected);
         set_result("Verifying Connection");
         set_progress(15);

         hide_manual_com();

         cancel_enable(true);

         simple_log("Serial port " + m_portinfo.port + " opened...");

         /* Flush the RX FIFO, after a cool off delay */
         Sleep(500);
         while (serial_read() >= 0);

         simple_log("Flushing serial read");

         simple_log("15% Writing Enter Key");

         string str1 = "\n";

         m_pserial->write(str1);

         int iState = 1;

         iLine = 0;

         simple_log("Waiting First Screen");

         int iTryCount = 5;

         while (iLine < iTryCount)
         {

            if (m_bCancel)
            {

               goto cancel;

            }

            string strLine = m_pserial->readline();

            simple_log("current state: " + ::str::from(iState));

            simple_log("     readline: " + strLine);

            //set_result(strLine);

            if (iState == 1 && strLine.contains_ci("Main Menu"))
            {

               simple_log("       state=> 2");

               iState = 2;

               iTryCount++;

            }
            else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
            {

               simple_log("       state=> 3");

               iState = 3;

               iTryCount++;

            }
            else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
            {

               simple_log("       state=> 4");

               iState = 4;

               iTryCount++;

            }
            else if (iState == 4 && strLine.contains("====="))
            {

               simple_log("       state=> 5");

               iState = 5;

               set_progress(20);

               redraw_window();

               Sleep(500);

               break;

            }

            Sleep(100);

            iLine++;

         }

         string strFilePath = m_pathSend;

         string strFileName = m_pathSend.name();

         if (iState == 5)
         {

            iState = -1;

            simple_log("Choosing Option 1");

            string str1 = "1";

            m_pserial->write(str1);

            logprintf("Information about file to send (2)\n");

            logprintf("  file path: \"%s\"\n", strFilePath.c_str());

            logprintf("  file name: \"%s\"\n", strFileName.c_str());

            logprintf("  file size: %d bytes\n", m_m.get_size());

            set_result("Uploading Firmware");

            iState = (int)ymodem_send(m_m.get_data(), (int)(u32)m_m.get_size(), (char*)(const char*)strFileName, &::grbl_merge_tool::total_progress);

            if (iState == 0 || iState != m_m.get_size())
            {

               delete m_pserial;

               m_pserial = NULL;

               m_bError = true;

               simple_log("ymodem_send failed with result " + ::str::from(iState));

               set_progress_status("Upload Error.");

               set_result("Failed. File Transfer error.");

               set_progress(0);

               cancel_enable(true);

               to_close();

               manual_com();

               redraw_window();

               return;

            }

         }
         else
         {

            iState = -1;

         }

         Sleep(500);

         if (iState > 0)
         {

            iState = 1;

            simple_log("Waiting Response of File Transfer");

            string strRName;

            int iRLen = -1;

            iLine = 0;

            while (iLine < 15)
            {

               string strLine = m_pserial->readline();

               simple_log("current state: " + ::str::from(iState));

               simple_log("     readline: " + strLine);

               //set_result(strLine);

               if (iState == 1 && strLine.contains_ci("programming completed successfully"))
               {

                  simple_log("       state=> 2");

                  iState = 2;

               }
               else if (iState == 2 && strLine.contains_ci("name:"))
               {

                  strLine.trim();

                  ::str::begins_eat_ci(strLine, "name:");

                  strRName = strLine;

                  strRName.trim();

                  simple_log("       state=> 3");

                  simple_log("programmed name: \"" + strRName + "\"");

                  iState = 3;

               }
               else if (iState == 3 && strLine.contains_ci("size:"))
               {

                  strLine.trim();

                  simple_log("       state=> 4");

                  ::str::begins_eat_ci(strLine, "size:");

                  ::str::ends_eat_ci(strLine, "bytes");

                  iRLen = atoi(strLine);

                  simple_log("programmed bytes: " + ::str::from(iRLen));

                  iState = 4;

                  if (strRName.compare_ci(strFileName) != 0 || iRLen != m_m.get_size())
                  {

                     delete m_pserial;

                     m_pserial = NULL;

                     m_bError = true;
                     simple_log("Programmed file name or programmed file size incorrect.");
                     set_progress_status("Upload Error.");
                     set_result("File name or size incorrect.");
                     set_progress(0);
                     cancel_enable(true);
                     to_close();

                     manual_com();

                     return;

                  }

                  set_progress_status("Upload Success.");
                  set_result("Finalizing...");
                  set_progress(95);
                  cancel_enable(true);
                  to_close();

                  break;

               }

               Sleep(100);

               iLine++;

            }

         }

         if (iState == 4)
         {

            iState = 1;

            simple_log("Waiting for Main Menu (2)");

            iLine = 0;

            while (iLine < 20)
            {


               if (m_bCancel)
               {

                  goto cancel;

               }


               string strLine = m_pserial->readline();

               simple_log("current state: " + ::str::from(iState));

               simple_log("     readline: " + strLine);

               //set_result(strLine);

               if (iState == 1 && strLine.contains_ci("Main Menu"))
               {

                  simple_log("       state=> 2");

                  iState = 2;

               }
               else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
               {

                  simple_log("       state=> 3");

                  iState = 3;

               }
               else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
               {

                  simple_log("       state=> 4");

                  iState = 4;

               }
               else if (iState == 4 && strLine.contains("====="))
               {

                  simple_log("       state=> 5");

                  iState = 5;
                  break;

               }

               Sleep(100);

               iLine++;

            }

         }

         if (iState == 5)
         {

            iState = 1;

            set_progress_status("Activating.");
            set_result("Activating update.");

            cancel_enable(true);

            to_close();

            simple_log("100% Writing Option 2");

            string str2 = "2";

            m_pserial->write(str2);

            iState = 2;

            //simple_log("Waiting for Main Menu (3)");

            //iLine = 0;

            //while (iLine < 15)
            //{

            //   string strLine = m_dlg2.m_pserial->readline();

            //   simple_log("current state:" + ::str::from(iState));

            //   simple_log("     readline: " + strLine);

            //   set_result(strLine);

            //   if (iState == 1 && strLine.contains_ci("Main Menu"))
            //   {

            //      simple_log("       state=> 2");

            //      iState = 2;

            //   }
            //   else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
            //   {

            //      simple_log("       state=> 3");

            //      iState = 3;

            //   }
            //   else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
            //   {
            //
            //      simple_log("       state=> 4");

            //      iState = 4;

            //   }
            //   else if (iState == 4 && strLine.contains("====="))
            //   {

            //      simple_log("       state=> 5");

            //      iState = 5;

            //      strLine = m_dlg2.m_pserial->readline();

            //      set_progress(100);

            //      m_dlg2.m_progressControl.redraw_window();

            //      m_dlg2.m_uploadProgress.set_window_text("100%");

            //      break;

            //   }

            //   Sleep(100);

            //   iLine++;

            //}

         }

         if (m_pserial)
         {

            delete m_pserial;

            m_pserial = NULL;

         }

         if (iState == 2)
         {

            simple_log("Success");

            m_bError = false;

            set_progress_status("Success!");
            set_result("Device update completed succesfully!");
            set_progress(100);
            cancel_enable(true);
            update_enable(true);
            to_close();

            redraw_window();

            m_iCom = -1;

         }
         else
         {

            simple_log("Failed");

            if (m_pserial)
            {

               delete m_pserial;
               m_pserial = NULL;

            }

            m_bError = true;

            set_progress_status("Protocol Error");
            set_result("Failed to update device.");
            set_progress(0);
            cancel_enable(true);
            update_enable(true);
            to_close();
            manual_com();
            redraw_window();

            m_iCom = -1;

         }

      }
      return;

cancel:

      ::aura::del(m_pserial);

      m_bCancel = false;

      set_progress_status("Cancelled");
      set_progress(0);
      set_result("User cancelled.");
      to_close();
      cancel_enable(true);
      update_enable(true);
      manual_com();


   }


#endif




//   void application::list_serial_port(stringa & stra, stringa & straPath) //added fucntion to find the present serial ports of the system;
//   {
//
//#ifdef WINDOWS_DESKTOP
//
//      TCHAR lpTargetPath[5000]; // buffer to store the path of the COMPORTS
//      DWORD test;
//      bool gotPort = 0; // in case the port is not found
//
//      string str;
//
//      for (int i = 0; i<255; i++) // checking ports from COM0 to COM255
//      {
//
//         string strComName;
//
//         strComName.Format(_T("COM%d"), i);
//
//         test = QueryDosDevice(strComName, (LPSTR)lpTargetPath, 5000);
//
//         // Test the return value and error if any
//         if (test != 0) //QueryDosDevice returns zero if it didn't find an object
//         {
//            stra.add(strComName);
//            straPath.add(lpTargetPath);
//            str += strComName;
//            str += " : ";
//            str += lpTargetPath;
//            str += "\r\n";
//
//         }
//
//      }
//
//      //Context.file().put_contents(dir::system() / "update_program/serial.txt", str) ;
//
//#elif defined(MACOS)
//
//#endif // WINDOWS_DESKTOP
//
//   }

   void application::app_check_version()
   {

      fork([&]()
      {


         property_set set;

         set["raw_http"] = true;
         set["disable_common_name_cert_check"] = true;

#ifdef WINDOWS_DESKTOP

         m_strAppDownload = Context.http().get("http://admin.quasarscience.com/update/rainbow/rainbow_pc_autoupdate.php", set);

#elif defined(MACOS)

         m_strAppDownload = Context.http().get("http://admin.quasarscience.com/update/rainbow/rainbow_mac_autoupdate.php", set);

#elif defined(LINUX)

         m_strAppDownload = Context.http().get("http://admin.quasarscience.com/update/rainbow/rainbow_raspi_autoupdate.php", set);

#endif

         string strHttpStatus = set["http_status"];

         i64 iHttpStatusCode = set["http_status_code"].int64();

         bool bInternetConnected = strHttpStatus.has_char();;

         m_bInternetConnected = iHttpStatusCode > 0;

         stringa stra;

         stra.add_lines(m_strAppDownload);

         if (stra.get_size() >= 2)
         {

            m_strAppVersion = stra[0];

            m_strAppUrl = stra[1];

         }
         else
         {

            m_strAppVersion.Empty();

            m_strAppUrl.Empty();

         }


         //on_version_response("Error.", "Version: " + m_strVersion);

         if (m_strAppVersion.has_char() && m_strAppUrl.has_char())
         {

            int  iCmp = version::compare(m_strAppVersion, version::get_version());

#if TEST_DOWNLOAD_APP
            if(1)
#else
            if (iCmp > 0)
#endif
            {

               on_app_check_version("Version " + m_strAppVersion + " is available.");

            }
            else
            {

               m_strAppVersion.Empty();

               m_strAppUrl.Empty();

            }

         }
         else
         {

            m_strAppVersion.Empty();

            m_strAppUrl.Empty();

         }

      });


   }


   void application::check_version()
   {

      fork([&]()
      {

         on_check_version_response("", false);

         //m_pparent->m_update.enable_window(false);

         property_set set;

         set["raw_http"] = true;
         set["disable_common_name_cert_check"] = true;

         m_strDownload = Context.http().get("http://admin.quasarscience.com/update/rainbow/file_list.php", set);

         stringa stra;

         stra.add_lines(m_strDownload);

         if (stra.get_size() >= 2)
         {

            m_strVersion = stra[0];
            m_strUrl = stra[1];


         }
         else
         {

            m_strVersion.Empty();
            m_strUrl.Empty();

         }


         //on_version_response("Error.", "Version: " + m_strVersion);

         if (set["http_status_code"].int64() != 200 || m_strVersion.is_empty() || m_strUrl.is_empty())
         {

            on_check_version_response("Error.", false);
            //m_versionStatus.set_window_text("Error.");

         }
         else
         {

            on_check_version_response("Version: " + m_strVersion, true);
            //m_versionStatus.set_window_text("Version: " + m_strVersion);

         }

         if (m_strVersion.has_char() && m_strUrl.has_char())
         {

            //m_download.enable_window(true);
            //m_downloadProgress.enable_window(true);

         }

      });


   }



   void application::download()
   {

      fork([&]()
      {

         memory m;

         property_set set;

         set["raw_http"] = true;
         set["disable_common_name_cert_check"] = true;
         set["app"] = get_context_application();
         set["http_downloaded_rate_listener"] = dynamic_cast < ::double_scalar_source::listener * > (this);
         set["http_downloaded_rate_id"] = "firmware_download_progress";

         Context.http().get(m_strUrl, m, set);

         m_pathDownload = Context.dir().get_download_folder() / ::file::path(m_strUrl).name();

         Context.file().put_contents(m_pathDownload, m);

         if (set["http_status_code"].int64() == 200 && m.get_size() > 0 && Context.file().exists(m_pathDownload))
         {

//         m_downloadProgress.set_window_text("100%");
//         m_pparent->m_update.enable_window(true);

            on_download_response("100%",true);
            m_pathSend = m_pathDownload;

            check_downloaded_bin();

         }
         else
         {
            Context.file().del(m_pathDownload);
            m_pathDownload.Empty();
            m_pathSend.Empty();
            on_download_response("Error.",false);
//         m_pparent->m_update.enable_window(false);

         }

      });

   }


   void application::radio_download()
   {

      bool bDownloadEnable = m_strVersion.has_char();

      bool bUpdateEnable = m_pathDownload.has_char() && Context.file().exists(m_pathDownload);

      if (bUpdateEnable)
      {

         m_pathSend = m_pathDownload;

      }
      else
      {

         m_pathSend.Empty();

      }

      on_radio_download_response(bDownloadEnable, bUpdateEnable);

   }


   void application::radio_browse()
   {

      bool bUpdateEnable = Context.file().exists(m_pathBrowse);

      if(bUpdateEnable)
      {

         m_pathSend = m_pathBrowse;

      }
      else
      {

         m_pathSend.Empty();

      }

      on_radio_browse_response(bUpdateEnable);

   }


   void application::radio_latest()
   {

      bool bUpdateEnable = Context.file().exists(m_pathLatestDownloadedBin);

      if (bUpdateEnable)
      {

         m_pathSend = m_pathLatestDownloadedBin;

      }
      else
      {

         m_pathSend.Empty();

      }

      on_radio_latest_response(bUpdateEnable);

   }


   void application::update_app()
   {

      fork([&]()
      {


         if (download_new_app())
         {

            install_new_app();

         }




      });



   }


   bool application::download_new_app()
   {

      memory m;

      property_set set;

      set["raw_http"] = true;
      set["disable_common_name_cert_check"] = true;
      set["app"] = get_context_application();
      set["http_downloaded_rate_listener"] = dynamic_cast < ::double_scalar_source::listener * > (this);
      set["http_downloaded_rate_id"] = "app_download_progress";


#if TEST_DOWNLOAD_APP && defined(WINDOWS_DESKTOP)

      m = file_as_memory_dup(Context.file().module());
      set["http_status_code"] = 200;
#else

      Context.http().get(m_strAppUrl, m, set);

#endif

      string strTitle = ::file::path(m_strAppUrl).name();

#ifdef WINDOWS_DESKTOP

      str::ends_eat_ci(strTitle, ".exe");

#elif defined(LINUX)

      str::ends_eat_ci(strTitle, ".tar.gz");

#elif defined(MACOS)

      str::ends_eat_ci(strTitle, ".zip");

#endif

      strTitle += " - ";

#ifdef WINDOWS_DESKTOP

      m_pathAppNew = Context.file().time(this, Context.dir().home(), 1,strTitle, ".exe");

#elif defined(LINUX)

      m_pathAppNew = Context.file().time(this, Context.dir().home(), 1,strTitle, ".tar.gz");

#elif defined(MACOS)

      m_pathAppNew = Context.file().time(this, Context.dir().home(), 1,strTitle, ".zip");

#endif

      Context.file().put_contents(m_pathAppNew, m);

      int iStatus = set["http_status_code"].int64();

      if (iStatus == 200 && m.get_size() > 0 && Context.file().exists(m_pathAppNew))
      {


         return true;

      }
      else
      {
         Context.file().del(m_pathAppNew);
         m_pathAppNew.Empty();

         return false;
      }

   }

   void application::install_new_app()
   {

#ifdef WINDOWS

      call_async(m_pathAppNew, " : install_app=\""+Context.file().module()+"\"", m_pathAppNew.folder(), SW_HIDE, true);

      m_pmainwindow->show_window(SW_HIDE);

#elif defined(LINUX)

      string str = m_pathAppNew;

      str.replace(" ", "\\ ");

      string strFolder = m_pathAppNew.folder();

      strFolder.replace(" ", "\\ ");

      string strFolder2 = Context.dir().home();

      string strName = m_pathAppNew.name();

      str::ends_eat_ci(strName, ".tar.gz");

      strFolder2 = ::file::path(strFolder2) / strName;

      ::dir::mk(strFolder2);

      strFolder2.replace(" ", "\\ ");

      string strCommand = "tar -xzvf " + str + " -C " + strFolder2 + " --strip-components=1";

//      file_put_contents_dup("/bergedge/tst", strCommand);

      ::system(strCommand);

      ::system(strFolder2 + "/install.sh");

      ::system(strFolder2 + "/init.sh");

      ::system("sudo reboot &");

      set_finish();

      System.set_finish();

      set_finish();

      System.set_finish();

      quit_system();

      return;

#elif defined(MACOS)

      string strFolder;

      strFolder = m_pathAppNew;

      ::str::ends_eat_ci(strFolder, ".zip");

      ::dir::mk(strFolder);

      ::system("/bin/bash -c 'unzip -d \""+strFolder+"\" \""+m_pathAppNew+"\"'");

      ::file::path pathSrc;

      pathSrc = strFolder;

      pathSrc /= "Quasar Science Update Utility.app";

      ::file::path pathDst;

      pathDst = strFolder + ".app";

      ::system("mv \""+pathSrc+"\" \""+pathDst+"\"");

      ::system("rmdir \"" +strFolder +"\"");

      ::system("open -a \""+pathDst+"\"");

#endif

      Sleep(3000);

      set_finish();

      System.set_finish();

      set_finish();

      System.set_finish();

      quit_system();

   }

   void application::on_browse(const char * pszPath)
   {

      m_pathBrowse = pszPath;

      fork([&]()
      {

         bool bUpdateEnable = Context.file().exists(m_pathBrowse);

         if(bUpdateEnable)
         {

            m_pathSend = m_pathBrowse;

         }
         else
         {

            Application.m_pathSend.Empty();

         }

         on_browse_response(bUpdateEnable);

      });

   }


   int application::get_total_size()
   {

      return (int) m_m.get_size();

   }

   void application::select_serial_and_update(int iIndex)
   {

      if(iIndex < 0 || iIndex >= m_portinfoa.get_count())
      {
         return;
      }

      m_portinfo = m_portinfoa[iIndex];

      fork_update();
   }


   void application::list_connected_serial_port(stringa & stra, stringa & straPath) //added fucntion to find the present serial ports of the system;
   {

#ifdef WINDOWS_DESKTOP
      TCHAR lpTargetPath[5000]; // buffer to store the path of the COMPORTS
      DWORD test;
      bool gotPort = 0; // in case the port is not found

      string str;

      for (int i = 0; i<255; i++) // checking ports from COM0 to COM255
      {

         string strComName;

         strComName.Format(_T("COM%d"), i);

         try
         {

            ::serial::Serial * pserial = new ::serial::Serial(get_context_application(), strComName, 115200);

            if (pserial != NULL)
            {

               delete pserial;

               Sleep(50);

               test = QueryDosDevice(strComName, (LPSTR)lpTargetPath, 5000);

               // Test the return value and error if any
               if (test != 0) //QueryDosDevice returns zero if it didn't find an object
               {
                  stra.add(strComName);
                  straPath.add(lpTargetPath);

               }
            }

         }
         catch (...)
         {

         }

      }

      //Context.file().put_contents(dir::system() / "update_program/serial.txt", str) ;
#endif

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

   }


   void application::manual_com()
   {

      array < const char * > psza;

      for (index i = 0; i < m_portinfoa.get_count(); i++)
      {

         psza.add(m_portinfoa[i].port);

      }

      int iOk;

#ifdef MACOS

      iOk = 0;

#else

      iOk = psza.get_upper_bound();

#endif

      show_manual_com(psza.get_data(), (int)(u32)psza.get_count(), iOk);

      update_enable(true);

      m_bCancel = false;

   }


   void application::on_set_scalar(double_scalar_source * psource, e_scalar escalar, double d, int iFlags)
   {

      if (psource->m_id == "firmware_download_progress")
      {

         string str;

         int iPercent = (int) (d * 100.0);

         str.Format("%d%%", iPercent);

         on_download_response(str, false);

      }
      else if (psource->m_id == "app_download_progress")
      {

         string str;

#ifdef WINDOWS_DESKTOP

         int iPercent = (int)(d * 100.0);

#elif defined(LINUX)

         int iPercent = (int)(d * 50.0);

#elif defined(MACOS)

         int iPercent = (int)(d * 100.0);

#endif

         str.Format("Progress: %d%%", iPercent);

         on_app_installation_response(str);

      }

   }


   void application::show_log()
   {

      ::file::path path;

#ifdef MACOS
      path = ::dir::home() / "update_utility/log.txt";
#else
      path = ::dir::system() / "update_utility/log.txt";
#endif
      Context.os().file_open(path);

   }

   void application::check_downloaded_bin()
   {

      ::file::listing ls(get_context_application());

      stringa straPattern({ "Rainbow_Update_V*" });

      ::file::path pathFolder(Context.dir().home() / "Downloads");

      ls.m_bDir = false;

      ls.m_bFile = true;

      ls.ls_pattern(pathFolder, straPattern);

      if (ls.get_count() <= 0)
      {

         on_disable_install_downloaded_bin();

      }
      else
      {

         stringa stra;
         output_debug_string("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx\n");
         for (auto & p : ls)
         {
            string str = p.name();
            ::str::begins_eat_ci(str, "Rainbow_Update_V");
            ::str::ends_eat_ci(str, ".bin");
            stra.add(str);
            output_debug_string(p);
            output_debug_string("\n-------------------------------------------------------------\n");
         }
         output_debug_string("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx\n");

         ::sort::pred_array_quick_sort(stra, &::Version::le);


         on_enable_install_downloaded_bin(stra.last());


         m_pathLatestDownloadedBin = Context.dir().home() / "Downloads" / ("Rainbow_Update_V" + stra.last() + ".bin");

         m_pathSend = m_pathLatestDownloadedBin;

      }

   }

} // namespace grbl_merge_tool




extern "C"
::aura::library * app_grbl_merge_tool_get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < ::grbl_merge_tool::application >(papp, "app/grbl_merge_tool");

}






extern "C"
{

   void _putchar(int c)
   {

      u8 ui = c;

      grbl_merge_tool::application::m_pserial->write(&ui, sizeof(ui));

   }


   int  _getchar(int timeout)
   {

      u8 ui = 0;

      if (timeout < 0)
      {

         while (true)
         {

            if (grbl_merge_tool::application::m_pserial->read(&ui, sizeof(ui)) >= 1)
            {

               return ui;

            }

         }

      }
      else
      {

         do
         {

            int iRes = (int) grbl_merge_tool::application::m_pserial->read(&ui, sizeof(ui));

            if (iRes >= 1)
            {

               return ui;

            }

            timeout--;

         }
         while (timeout > 0);

      }

      return -1;

   }


   int  serial_read(void)
   {

      int iChar = _getchar(2);

      if (iChar > 0)
      {

         simple_log_no_new_line(string((char)iChar));

      }

      return iChar;

   }

   void _ssleep(int iSeconds)
   {

      ::Sleep(iSeconds * 1000);
   }

}


void grbl_merge_tool_set_main_interface(main_interface ** pmaininterface)
{

   *pmaininterface = ::grbl_merge_tool::g_papp;

}


void main_interface::set_progress(int iPercent)
{

   string strStatus;

   strStatus.Format("%d%%", iPercent);

   set_progress(iPercent, strStatus);

}

void main_interface::total_progress(int iBytes)
{

   total_progress(iBytes, get_total_size());

}







void main_interface::total_progress(int iBytes, int iTotalBytes)
{

   int iMax = 70;

   int i = iMax * iBytes / iTotalBytes;

   set_progress(i);

}


