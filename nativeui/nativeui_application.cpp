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

      ::file::patha patha;

      ::dir::ls(patha, "/dev");

      for(auto & path : patha)
      {

         string strName = path.name();

         if(strName.length() == 4)
         {

            if(strName[0] == 's'
                  && strName[1] == 'd'
                  && isalpha_dup(strName[2])
                  && strName[3] == '1')
            {

               output_debug_string("Found device : " + path);

            }


         }


      }

      g_papp = this;

      m_strAppName = "app/nativeui";
      m_strLibraryName = "app/nativeui";
      m_strBaseSupportId = "ca2_flag";
      m_bLicense = false;

   }


   application::~application()
   {
   }


   bool application::init_instance()
   {

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

   void application::on_request(::create * pcreate)
   {


   }





   i64 application::add_ref()
   {

      return ::object::add_ref();

   }

   i64 application::dec_ref()
   {

      return ::object::dec_ref();

   }




} // namespace nativeui



