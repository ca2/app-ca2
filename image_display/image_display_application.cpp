#include "framework.h"
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

void screen_saver_suspend(bool bSuspend);

namespace image_display
{

   application * g_papp = NULL;

   application::application()
   {

      g_papp = this;

      m_strAppName = STANDALONE;
      m_strLibraryName = STANDALONE;
      m_strBaseSupportId = "image_display";
      m_bLicense = false;

   }

   application::~application()
   {
   }



   bool application::init_instance()
   {

      screen_saver_suspend(true);

      return ::aura::application::init_instance();

   }


   void application::term_instance()
   {

      screen_saver_suspend(false);

      ::aura::application::term_instance();

   }


   void application::on_request(::create * pcreate)
   {

      ::file::path pathDeviceId = ::dir::config() / "mywindowdressings_image_display" / "device_id.txt";

      string strId;

      strId = file_as_string_dup(pathDeviceId);

      if(strId.is_empty())
      {

         strId = System.math().random_string(32);

         file_put_contents_dup(pathDeviceId, strId);

      }

      {

         __pointer(socket_thread) psocketthread = canew(socket_thread(this));

         psocketthread->m_iPort = 80;
         psocketthread->m_iSsl = -1;
         psocketthread->m_strCat = "";

         psocketthread->begin();

         m_socketthreada.add(psocketthread);

      }

      {

         __pointer(socket_thread) psocketthread = canew(socket_thread(this));

         psocketthread->m_iPort = 443;
         psocketthread->m_iSsl = -1;
         psocketthread->m_strCat = (::dir::ca2_module() / "image_display.cat");

         psocketthread->begin();

         m_socketthreada.add(psocketthread);

      }


      m_pwindow = new window(this);

      rect r(100, 100, 500, 500);

      m_pwindow->create_window("", "Image Display", 0, null_rect(), NULL, "image_display");

      m_pwindow->SetWindowPos(ZORDER_TOP, r, SWP_SHOWWINDOW);

      struct ifaddrs * ifAddrStruct=NULL;

      struct ifaddrs * ifa=NULL;

      void * tmpAddrPtr=NULL;

      getifaddrs(&ifAddrStruct);

      property_set set;

      set["raw_http"] = true;

      set["disable_common_name_cert_check"] = true;

      for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
      {

         if (!ifa->ifa_addr)
         {

            continue;

         }

         char addressBuffer[MAX(INET6_ADDRSTRLEN,INET_ADDRSTRLEN)];

         // check it is IP4
         if (ifa->ifa_addr->sa_family == AF_INET)
         {

            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;

            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);

        }
        // check it is IP6
        else if (ifa->ifa_addr->sa_family == AF_INET6)
        {

            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;

            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);

        }

        string strUrl = file_as_string_dup(::dir::ca2_module() / "register_device.txt");

        strUrl.trim();

        strUrl.replace("$id", strId);

        strUrl.replace("$address", string(addressBuffer));

        output_debug_string(strUrl + "\n");

        Context.http().get(strUrl, set);

      }

      if (ifAddrStruct!=NULL)
      {

         freeifaddrs(ifAddrStruct);

      }


   }


   i64 application::add_ref()
   {

      return ::object::add_ref();

   }


   i64 application::dec_ref()
   {

      return ::object::dec_ref();

   }




} // namespace image_display




extern "C"
::aura::library * app_image_display_get_new_library(::aura::application * papp)
{

   return new ::aura::single_application_library < ::image_display::application >(papp, STANDALONE);

}




aura_app image_display_flags(aura_app::flag_do_not_install);





