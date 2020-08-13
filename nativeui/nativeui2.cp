#include "framework.h"

#ifdef WINDOWS
#include <SetupAPI.h>
#endif
//#define DEV_DEBUG
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

      stringa straSep;

      straSep.add(".");
      straSep.add("_");

      stra1.add_smallest_tokens(str1, straSep);

      stra2.add_smallest_tokens(str2, straSep);

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


namespace nativeui
{

   application * g_papp = NULL;

   application::application()
   {

      m_bDetecting = false;

      m_bError = false;

      g_papp = this;
      m_iCom = -1;
      init_printf(NULL, &simple_log_putf);

      m_strAppName = "app/nativeui";
      m_strLibraryName = "app/nativeui";
      m_strBaseSupportId = "ca2_flag";
      m_bLicense = false;

   }

   void total_progress(struct ymodem_serial * pserial, int iTotal)
   {

      g_papp->total_progress(pserial->m_pszPort, iTotal);

   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

      m_bMultiPort = file_exists_dup(::dir::home() / ".quasar_science_multi_port");

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

   bool application::upgrade_cable_driver()
   {

#ifdef WINDOWS

      ::file::path path = ::dir::system() / "UpgradeCableDriver.zip";
      if (!read_resource_as_file_dup(path, System.m_hinstance, IDR_ZIP_UPGRADECABLEDRIVER, "ZIP"))
      {
         return false;
      }

      if (!file_exists_dup(path))
      {
         return false;
      }

      zip::util util;

      if (!util.extract_all(this, dir::system() / "UpgradeCableDriver", path))
      {
         return false;
      }

      PSTR size = 0;
      if (!SetupCopyOEMInf(dir::system() / "UpgradeCableDriver"/ "ch341ser.inf", NULL, SPOST_NONE, SPOST_NONE, 0, NULL, 0, &size))
      {
         return false;
      }
#endif

      return true;

   }


   void application::on_request(::create * pcreate)
   {

      ::file::path pathSrc = get_log_path();

#ifdef WINDOWS

      if (is_windows_7_or_lower())
      {
         if (!upgrade_cable_driver())
         {
            MessageBox(NULL, "Failed to update cable driver.", "Error", MB_ICONEXCLAMATION);
         }

      }

#endif

      if(Application.file().exists(pathSrc))
      {

         ::file::path pathDst;

         pathDst = pathSrc.folder() / (pathSrc.title() + "-backup-" + System.datetime().international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE) + ".log");

         Application.file().move(pathDst, pathSrc);

      }

#if 0

#ifdef DEBUG

      output_debug_string("DEBUG build? (basis)");

      ASSERT(FALSE);

#else

      output_debug_string("RELEASE build? (stage)");

      ASSERT(FALSE);

#endif

#endif


#ifdef WINDOWS_DESKTOP

      fork([&]()
      {

         output_debug_string("\nfinished nativeui::on_request");

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

         System.post_quit();

      });
#elif defined(MACOS)

      show_main_dialog();

      app_check_version();

      check_downloaded_bin();

#elif defined(LINUX)

      fork([&]()
      {

         gtk_init(NULL, NULL);

         g_thread_init(NULL);

         gtk_main_context = g_main_context_default();

         auto idle_source = g_idle_source_new();

         g_source_set_callback(idle_source, &linux_main_window, (::aura::application *) this, NULL);

         g_source_attach(idle_source, gtk_main_context);

         gtk_main();


      });


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


   ::serial::PortInfo application::pick_port()
   {

      m_bError = false;

      ::serial::PortInfo portinfo;

      try
      {

         single_lock sl(&m_mutexPickPort);

         if(m_portinfoaUpdate.get_size() >= get_max_port_count())
         {

            return portinfo;

         }

         {

            keep < bool > keepDetecting(&m_bDetecting, true, false, true);

            Sleep(200);

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

               {

                  list_serial_ports();

                  if (m_portinfoa.has_elements())
                  {

#ifdef MACOS

                     iOk = 0;


#else

                     iOk = -1;

                     bool bFoundFree;

                     for(index j = m_portinfoa.get_upper_bound(); j >= 0; j--)
                     {

                        bFoundFree = true;

                        for(index k = 0; k < m_portinfoaUpdate.get_size(); k++)
                        {

                           if(m_portinfoaUpdate[k].port.compare_ci(m_portinfoa[j].port) == 0)
                           {

                              bFoundFree = false;

                           }

                        }

                        if(bFoundFree)
                        {

                           iOk = j;

                           break;

                        }

                     }

#endif

                     if(iOk >= 0)
                     {

                        portinfo = m_portinfoa[iOk];

                     }

                     break;

                  }

               }

               for (int j = 0; j < 5 && !m_bCancel; j++)
               {

                  Sleep(35);

               }


            }


         }

         Sleep(200);

      }
      catch(...)
      {


      }

      return portinfo;

   }


   void application::start_update()
   {

      if(m_bUpdating)
      {

         return;

      }

      m_bCancel = false;

      fork([&]()
      {

         keep < bool > keepUpdating(&m_bUpdating, true, false, true);

         string strFilePath = m_pathSend;

         string strFileName = m_pathSend.name();

         logprintf("Information about file to send (1)\n");

         logprintf("  file path: \"%s\"\n", strFilePath.c_str());
         logprintf("  file name: \"%s\"\n", strFileName.c_str());
         logprintf("  file size: %d bytes\n", m_m.get_size());

         simple_log("");
         simple_log("=====");
         simple_log("Update Utility " + ::version::get_version());
         simple_log(System.datetime().international().get_gmt_date_time());
         cancel_enable(true);

         Application.file().as_memory(strFilePath, m_m);

         if (m_m.get_size() <= 0)
         {

            logprintf("File to be send has no content.");

            m_bError = true;

            cancel_enable(true);

            redraw_window();

            return;

         }

         m_bUpdate = true;

         while(!m_bCancel && m_bUpdate && ::get_thread_run())
         {

            ::serial::PortInfo portinfo = pick_port();

            if(portinfo.port.has_char())
            {

               fork([&, portinfo]()
               {

                  update_port(portinfo);

               });

            }

            pred_Sleep(1500, [&](){ return !m_bCancel && m_bUpdate && ::get_thread_run(); });

            if (m_bCancel)
            {

               goto cancel;

            }

         }

         return;

cancel:

         m_bCancel = false;

         to_close();
         cancel_enable(true);
         update_enable(true);
         manual_com();


      });

   }


   void application::stop_update()
   {

      m_bUpdate = false;

      m_bCancel = true;

   }


   void application::remove_serial(::serial::PortInfo portinfo, __pointer(::serial::Serial) & pserial)
   {

      try
      {

         if(portinfo.port.has_char())
         {

            synch_lock sl(&m_mutexPickPort);

            remove_port(portinfo.port);

            m_iUpdating--;

            m_portinfoaUpdate.pred_remove([&](auto & info){ return info.port.compare_ci(portinfo.port)== 0; });

         }

      }
      catch(...)
      {

      }





      if(pserial.is_null())
      {

         return;

      }

      try
      {

         synch_lock sl(&m_mutexPickPort);

         m_seriala.remove(pserial);

         pserial.release();

      }
      catch(...)
      {

      }


   }

#ifdef RASPBERRY

   void application::update()
   {

      int iLine;
      try
      {

         if (m_pserial == NULL)
         {

            m_pserial = new ::serial::Serial(get_app(), "", 115200, ::serial::Timeout::simpleTimeout(1000));

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

         Application.file().as_memory(m_dlg1.m_pathSend, m_m);

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

   void application::update_port(::serial::PortInfo portinfo)
   {

      {

         synch_lock sl(&m_mutexPickPort);

         m_iUpdating++;

         m_portinfoaUpdate.add(portinfo);

         add_port(portinfo.port);

      }

      try
      {

         m_bError = false;
         to_cancel();
         update_enable(false);
         int iLine;
         int iState;
         int iTryCount;
         string str1;
         string strFilePath = m_pathSend;

         string strFileName = m_pathSend.name();



         bool bGeneralException = false;

         __pointer(serial::Serial) pserial;

         try
         {

            synch_lock sl(&m_mutexPickPort);

            pserial = canew(::serial::Serial(get_app(), portinfo.port, 115200, ::serial::Timeout::simpleTimeout(1000)));

            m_seriala.add(pserial);

         }
         catch (invalid_argument_exception *pe)
         {

            simple_log(portinfo.port + "> Failed. " +  + pe->get_message());
            set_result(portinfo.port, "Failed");
            set_progress(portinfo.port, 0);
            redraw_window();

            goto cancel;

         }
         catch (serial::SerialException *pe)
         {

            simple_log(portinfo.port + " > Failed. " + pe->what());
            set_result(portinfo.port, "Serial Error.");
            set_progress(portinfo.port, 0);
            redraw_window();

            goto cancel;

         }
         catch (serial::IOException * pe)
         {

            string strResult = "Failed. " + string(pe->what());

            string strExtra;

#ifdef RASPBIAN


            if(pe->getErrorNumber() == 13)
            {

               if(::system("sudo chmod 777 " + portinfo.port) == 0)
               {

                  strExtra = "Permissions fixed for " + portinfo.port +". Please try again.";

               }
               else
               {

                  strExtra = "Unable to fixed permissions for "+portinfo.port+".";

               }

            }

#endif

            simple_log(portinfo.port + "> " + strResult + " " + strExtra);
            set_result(portinfo.port, "IO Error (" + ::str::from(pe->getErrorNumber()) + ")");
            set_progress(portinfo.port, 0);
            redraw_window();

            goto cancel;

         }
         catch (...)
         {

            simple_log(portinfo.port + "> Failed. Could not open connection.");
            set_result(portinfo.port, "Connection Failed.");
            set_progress(portinfo.port, 0);
            redraw_window();

            goto cancel;

         }

         ymodem_serial serial;

         serial.m_popaque = pserial.m_p;
         serial.m_pszPort = portinfo.port;

         set_result(portinfo.port, "Verifying Connection");
         set_progress(portinfo.port, 15);
         hide_manual_com();
         cancel_enable(true);

         Sleep(500);

         simple_log("Serial port " + portinfo.port + " opened...");

         /* Flush the RX FIFO, after a cool off delay */
         Sleep(500);
         while (serial_read(&serial) >= 0);

         simple_log(portinfo.port + "> Flushing serial read");

         simple_log(portinfo.port + "> 15% Writing Enter Key");

         str1 = "\n";

         pserial->write(str1);

         iState = 1;

         iLine = 0;

         simple_log(portinfo.port + "> Waiting First Screen");

         iTryCount = 5;

         while (iLine < iTryCount)
         {


            string strLine = pserial->readline();

            simple_log(portinfo.port + "> current state: " + ::str::from(iState));

            simple_log(portinfo.port + ">      readline: " + strLine);

            if (iState == 1 && strLine.contains_ci("Main Menu"))
            {

               simple_log(portinfo.port + ">        state=> 2");

               iState = 2;

               iTryCount++;

            }
            else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
            {

               simple_log(portinfo.port + ">        state=> 3");

               iState = 3;

               iTryCount++;

            }
            else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
            {

               simple_log(portinfo.port + ">        state=> 4");

               iState = 4;

               iTryCount++;

            }
            else if (iState == 4 && strLine.contains("====="))
            {

               simple_log(portinfo.port + ">        state=> 5");

               iState = 5;

               set_progress(portinfo.port, 20);

               redraw_window();

               Sleep(500);

               break;

            }

            Sleep(100);

            iLine++;

         }

         if (iState == 5)
         {

            iState = -1;

            simple_log(portinfo.port + "> Choosing Option 1");

            string str1 = "1";

            pserial->write(str1);

            logprintf(portinfo.port + "> Information about file to send (2)\n");

            logprintf(portinfo.port + ">   file path: \"%s\"\n", strFilePath.c_str());

            logprintf(portinfo.port + ">   file name: \"%s\"\n", strFileName.c_str());

            logprintf(portinfo.port + ">   file size: %d bytes\n", m_m.get_size());

            set_result(portinfo.port, "Uploading Firmware");

            iState = (int)ymodem_send(&serial, m_m.get_data(), (int)(u32)m_m.get_size(), (char*)(const char*)strFileName, &::nativeui::total_progress);

            if (iState == 0 || iState != m_m.get_size())
            {

               simple_log(portinfo.port + "> ymodem_send failed with result " + ::str::from(iState));

               set_result(portinfo.port, "File Transfer error.");

               set_progress(portinfo.port, 0);

               redraw_window();

               goto cancel;


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

               string strLine = pserial->readline();

               simple_log(portinfo.port + "> current state: " + ::str::from(iState));

               simple_log(portinfo.port + ">      readline: " + strLine);

               if (iState == 1 && strLine.contains_ci("programming completed successfully"))
               {

                  simple_log(portinfo.port + ">        state=> 2");

                  iState = 2;

               }
               else if (iState == 2 && strLine.contains_ci("name:"))
               {

                  strLine.trim();

                  ::str::begins_eat_ci(strLine, "name:");

                  strRName = strLine;

                  strRName.trim();

                  simple_log(portinfo.port + ">        state=> 3");

                  simple_log(portinfo.port + "> programmed name: \"" + strRName + "\"");

                  iState = 3;

               }
               else if (iState == 3 && strLine.contains_ci("size:"))
               {

                  strLine.trim();

                  simple_log(portinfo.port + ">        state=> 4");

                  ::str::begins_eat_ci(strLine, "size:");

                  ::str::ends_eat_ci(strLine, "bytes");

                  iRLen = atoi(strLine);

                  simple_log(portinfo.port + "> programmed bytes: " + ::str::from(iRLen));

                  iState = 4;

                  if (strRName.compare_ci(strFileName) != 0 || iRLen != m_m.get_size())
                  {

                     simple_log(portinfo.port + "> Programmed file name or programmed file size incorrect.");
                     set_result(portinfo.port, "File name/size error.");
                     set_progress(portinfo.port, 0);
                     redraw_window();

                     goto cancel;

                  }

                  set_result(portinfo.port, "Finalizing...");
                  set_progress(portinfo.port, 95);
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

            simple_log(portinfo.port + "> Waiting for Main Menu (2)");

            iLine = 0;

            while (iLine < 20)
            {


               if (m_bCancel)
               {

                  goto cancel;

               }


               string strLine = pserial->readline();

               simple_log(portinfo.port + "> current state: " + ::str::from(iState));

               simple_log(portinfo.port + ">      readline: " + strLine);

               if (iState == 1 && strLine.contains_ci("Main Menu"))
               {

                  simple_log(portinfo.port + ">        state=> 2");

                  iState = 2;

               }
               else if (iState == 2 && strLine.contains_ci("Download Image To the Internal Flash") && strLine.trimmed().ends_ci("1"))
               {

                  simple_log(portinfo.port + ">        state=> 3");

                  iState = 3;

               }
               else if (iState == 3 && strLine.contains_ci("Execute The New Program") && strLine.trimmed().ends_ci("2"))
               {

                  simple_log(portinfo.port + ">        state=> 4");

                  iState = 4;

               }
               else if (iState == 4 && strLine.contains("====="))
               {

                  simple_log(portinfo.port + ">        state=> 5");

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

            set_result(portinfo.port, "Activating update.");

            cancel_enable(true);

            to_close();

            simple_log(portinfo.port + " > 100% Writing Option 2");

            string str2 = "2";

            pserial->write(str2);

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

         if (pserial)
         {

            delete pserial;

            pserial = NULL;

         }

         if (iState == 2)
         {

            simple_log(portinfo.port + "> Success");
            set_result(portinfo.port, "Success!");
            set_progress(portinfo.port, 100);

            redraw_window();

            goto cancel;

         }
         else
         {

            simple_log(portinfo.port + "> Failed");
            set_result(portinfo.port, "Failed to update device.");
            set_progress(portinfo.port, 0);
            redraw_window();

            goto cancel;

         }

cancel:
         #ifdef DEV_DEBUG

         Sleep(10000);

         #endif


         remove_serial(portinfo, pserial);

      }
      catch(...)
      {

      }

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
//      //Application.file().put_contents(dir::system() / "update_program/serial.txt", str) ;
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

         m_strAppDownload = Application.http().get("http://admin.quasarscience.com/update/rainbow/rainbow_pc_autoupdate.php", set);

#elif defined(MACOS)

         m_strAppDownload = Application.http().get("http://admin.quasarscience.com/update/rainbow/rainbow_mac_autoupdate.php", set);

#elif defined(LINUX)

         m_strAppDownload = Application.http().get("http://admin.quasarscience.com/update/rainbow/rainbow_raspi_autoupdate.php", set);

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

         m_strDownload = Application.http().get("http://admin.quasarscience.com/update/rainbow/file_list.php", set);

         stringa stra;

         stra.add_lines(m_strDownload);

         if (stra.get_size() >= 2)
         {

            m_strDownloadVersion = stra[0];
            m_strUrl = stra[1];


         }
         else
         {

            m_strDownloadVersion.Empty();
            m_strUrl.Empty();

         }


         //on_version_response("Error.", "Version: " + m_strVersion);

         if (set["http_status_code"].int64() != 200 || m_strDownloadVersion.is_empty() || m_strUrl.is_empty())
         {

            on_check_version_response("Error.", false);
            //m_versionStatus.set_window_text("Error.");

         }
         else
         {

            on_check_version_response("Version: " + m_strDownloadVersion, true);
            //m_versionStatus.set_window_text("Version: " + m_strVersion);

         }

         if (m_strDownloadVersion.has_char() && m_strUrl.has_char())
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
         set["app"] = get_app();
         set["http_downloaded_rate_listener"] = dynamic_cast < ::double_scalar_source::listener * > (this);
         set["http_downloaded_rate_id"] = "firmware_download_progress";

         Application.http().get(m_strUrl, m, set);

         m_pathDownload = System.dir().download() / ::file::path(m_strUrl).name();

         Application.file().put_contents(m_pathDownload, m);

         if (set["http_status_code"].int64() == 200 && m.get_size() > 0 && Application.file().exists(m_pathDownload))
         {
            m_bInternetConnected = true;
//         m_downloadProgress.set_window_text("100%");
//         m_pparent->m_update.enable_window(true);

            on_download_response("100%",true);
            m_pathSend = m_pathDownload;

            m_strVersion = m_strDownloadVersion;

            check_downloaded_bin();

         }
         else
         {
            Application.file().del(m_pathDownload);
            m_pathDownload.Empty();
            m_pathSend.Empty();
            m_strVersion.Empty();
            on_download_response("Error.",false);
//         m_pparent->m_update.enable_window(false);

         }

      });

   }


   void application::radio_download()
   {

      bool bDownloadEnable = m_strVersion.has_char();

      bool bUpdateEnable = m_pathDownload.has_char() && Application.file().exists(m_pathDownload);

      if (bUpdateEnable)
      {

         m_pathSend = m_pathDownload;

         m_strVersion = m_strDownloadVersion;

      }
      else
      {

         m_pathSend.Empty();

         m_strVersion.Empty();

      }

      on_radio_download_response(bDownloadEnable, bUpdateEnable);

   }


   void application::radio_browse()
   {

      bool bUpdateEnable = Application.file().exists(m_pathBrowse);

      if(bUpdateEnable)
      {

         m_pathSend = m_pathBrowse;

         m_strVersion = m_strBrowseVersion;

      }
      else
      {

         m_pathSend.Empty();

         m_strVersion.Empty();

      }

      on_radio_browse_response(bUpdateEnable);

   }


   void application::radio_latest()
   {

      bool bUpdateEnable = Application.file().exists(m_pathLatestDownloadedBin);

      if (bUpdateEnable)
      {

         m_pathSend = m_pathLatestDownloadedBin;

         m_strVersion = m_strLatestDownloadedBinVersion;

      }
      else
      {

         m_pathSend.Empty();

         m_strVersion.Empty();

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
      set["app"] = get_app();
      set["http_downloaded_rate_listener"] = dynamic_cast < ::double_scalar_source::listener * > (this);
      set["http_downloaded_rate_id"] = "app_download_progress";


#if TEST_DOWNLOAD_APP && defined(WINDOWS_DESKTOP)

      m = file_as_memory_dup(System.file().module());
      set["http_status_code"] = 200;
#else

      Application.http().get(m_strAppUrl, m, set);

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

      m_pathAppNew = System.file().time(this, System.dir().home(), 1,strTitle, ".exe");

#elif defined(LINUX)

      m_pathAppNew = System.file().time(this, System.dir().home(), 1,strTitle, ".tar.gz");

#elif defined(MACOS)

      m_pathAppNew = System.file().time(this, System.dir().home(), 1,strTitle, ".zip");

#endif

      Application.file().put_contents(m_pathAppNew, m);

      int iStatus = set["http_status_code"].int64();

      if (iStatus == 200 && m.get_size() > 0 && Application.file().exists(m_pathAppNew))
      {

         m_bInternetConnected = true;

         return true;

      }
      else
      {
         Application.file().del(m_pathAppNew);
         m_pathAppNew.Empty();

         return false;
      }

   }

   void application::install_new_app()
   {

#ifdef WINDOWS

      call_async(m_pathAppNew, " : install_app=\""+System.file().module()+"\"", m_pathAppNew.folder(), SW_HIDE, true);

      m_pmainwindow->show_window(SW_HIDE);

#elif defined(LINUX)

      string str = m_pathAppNew;

      str.replace(" ", "\\ ");

      string strFolder = m_pathAppNew.folder();

      strFolder.replace(" ", "\\ ");

      string strFolder2 = System.dir().home();

      string strName = m_pathAppNew.name();

      str::ends_eat_ci(strName, ".tar.gz");

      strFolder2 = ::file::path(strFolder2) / strName;

      ::dir::mk(strFolder2);

      strFolder2.replace(" ", "\\ ");

      on_app_installation_response("Installing: 55%");

      Sleep(500);

      string strCommand = "tar -xzvf " + str + " -C " + strFolder2 + " --strip-components=1";

//      file_put_contents_dup("/bergedge/tst", strCommand);

      ::system(strCommand);

      on_app_installation_response("Installing: 70%");

      Sleep(500);

      ::system(strFolder2 + "/install.sh");

      on_app_installation_response("Installing: 90%");

      Sleep(500);

      ::system(strFolder2 + "/init.sh");

      on_app_installation_response("Rebooting: 100%");

      Sleep(500);

      ::system("sudo reboot &");

      post_quit();

      System.post_quit();

      post_quit();

      System.post_quit();

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

      post_quit();

      System.post_quit();

      post_quit();

      System.post_quit();

      quit_system();

   }

   void application::on_browse(const char * pszPath, const char * pszVersion)
   {

      m_pathBrowse = pszPath;

      m_strBrowseVersion = pszVersion;

      fork([&]()
      {

         bool bUpdateEnable = Application.file().exists(m_pathBrowse);

         if(bUpdateEnable)
         {

            m_pathSend = m_pathBrowse;

            if(m_strBrowseVersion.is_empty())
            {

               string str(m_strBrowseVersion);

               str = m_pathSend;

               strsize iFind = str.find_ci("_V");

               if (iFind >= 0)
               {

                  str = str.Mid(iFind + 2);

                  ::str::ends_eat_ci(str, ".bin");

                  str.replace("_", ".");

                  m_strBrowseVersion = str;

                  m_strVersion = m_strBrowseVersion;

               }
               else
               {

                  m_strBrowseVersion = m_pathSend.title();

                  m_strVersion = m_strBrowseVersion;

               }

            }
            else
            {

               m_strVersion = m_strBrowseVersion;

            }

         }
         else
         {

            Application.m_pathSend.Empty();

            m_strVersion.Empty();

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

      //m_portinfo = m_portinfoa[iIndex];

      //fork_update();
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

            ::serial::Serial * pserial = new ::serial::Serial(get_app(), strComName, 115200);

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

      //Application.file().put_contents(dir::system() / "update_program/serial.txt", str) ;
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

         DWORD dwNow = get_tick_count();

         if(dwNow - m_dwLastProgress > 300 || iPercent == 100)
         {

            m_dwLastProgress = dwNow;

            str.Format("%d%%", iPercent);

            on_download_response(str, false);

         }

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

         DWORD dwNow = get_tick_count();

         if(dwNow - m_dwLastProgress > 300 || iPercent == 100 || iPercent == 50)
         {

            m_dwLastProgress = dwNow;

            str.Format("Progress: %d%%", iPercent);

            on_app_installation_response(str);

         }

      }

   }


   void application::show_log()
   {

      System.os().file_open(get_log_path());

   }

   void application::check_downloaded_bin()
   {

      fork([&]()
      {

         Sleep(1000);

         ::file::listing ls(get_app());

         stringa straPattern({ "Rainbow_Update_V*" });

         ::file::path pathFolder(System.dir().download());

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

            m_strLatestDownloadedBinVersion = stra.last();

            m_strLatestDownloadedBinVersion.replace("_", ".");

            on_enable_install_downloaded_bin(m_strLatestDownloadedBinVersion);

            m_strVersion = m_strLatestDownloadedBinVersion;

            m_pathLatestDownloadedBin = System.dir().download() / ("Rainbow_Update_V" + stra.last() + ".bin");

            m_pathSend = m_pathLatestDownloadedBin;

         }

      });

   }

   ::file::path get_log_path()
   {

      ::file::path path;

#if defined(LINUX)
      path = ::dir::home() / ".config/nativeui/log.txt";
#elif defined(MACOS)
      path = ::dir::home() / "Library/Application Support/Quasar Science Update Utility/log.txt";
#else
      path = ::dir::system() / "update_utility/log.txt";
#endif

      return path;

   }

} // namespace nativeui




extern "C"
::aura::library * app_nativeui_get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < ::nativeui::application >(papp, "app/nativeui");

}






extern "C"
{

   void _putchar(struct ymodem_serial * pserial, int c)
   {

      ::serial::Serial * ps = (::serial::Serial *) pserial->m_popaque;

      u8 ui = c;

      ps->write(&ui, sizeof(ui));

   }


   int  _getchar(struct ymodem_serial * pserial, int timeout)
   {

      ::serial::Serial * ps = (::serial::Serial *) pserial->m_popaque;

      u8 ui = 0;

      if (timeout < 0)
      {

         while (true)
         {

            if (ps->read(&ui, sizeof(ui)) >= 1)
            {

               return ui;

            }

         }

      }
      else
      {

         do
         {

            int iRes = (int) ps->read(&ui, sizeof(ui));

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


   int  serial_read(struct ymodem_serial * pserial)
   {

      ::serial::Serial * ps = (::serial::Serial *) pserial->m_popaque;

      int iChar = _getchar(pserial, 2);

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


void nativeui_set_main_interface(main_interface ** pmaininterface)
{

   *pmaininterface = ::nativeui::g_papp;

}


void main_interface::set_progress(const char * pszPort, int iPercent)
{

   string strStatus;

   strStatus.Format("%d%%", iPercent);

   set_progress(pszPort, iPercent, strStatus);

}

void main_interface::total_progress(const char * pszPort,  int iBytes)
{

   total_progress(pszPort, iBytes, get_total_size());

}







void main_interface::total_progress(const char * pszPort, int iBytes, int iTotalBytes)
{

   int iMax = 70;

   int i = iMax * iBytes / iTotalBytes;

   set_progress(pszPort, i);

}



aura_app nativeui_flags(aura_app::flag_do_not_install);



