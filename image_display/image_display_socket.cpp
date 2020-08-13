#include "framework.h"

namespace version
{

   string get_version();

} // namespace version


namespace image_display
{


   socket::socket(::sockets::base_socket_handler & h) :
      ::object(h.get_context_application()),
      base_socket(h),
      ::sockets::socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_base_socket(h),
      http_socket(h),
      httpd_socket(h),
      ::netserver::socket(h)
   {
      m_bSetCookie = true;
      //s_countNetnodeSocket++;
      //m_strCipherList = "ECDH";

   }



   socket::~socket()
   {

   }


   void socket::on_send_response()
   {

      string strUrl = m_request.attr("http_protocol").get_string() + "://" + m_request.header("host").get_string() + m_request.attr("request_uri").get_string();

      output_debug_string("request: " +  strUrl + "\"");

      string strScript = System.url().get_script(strUrl);

      if(strScript == "/version")
      {

         response().file().write("version: " + ::version::get_version());

      }
      else if(strScript == "/")
      {

         string strCommand = System.url().get_param(strUrl, "command");

         stringa stra;

         stra.explode(",", strCommand);

         string strImageId = stra[0];

         string x;

         string y;

         string w;

         string h;

         if(stra.get_count() >= 2)
         {

            x = stra[1];

         }

         if(stra.get_count() >= 3)
         {

            y = stra[2];

         }
         if(stra.get_count() >= 4)
         {

            w = stra[3];

         }

        if(stra.get_count() >= 5)
        {

          h = stra[4];

        }

        string x2;

         string y2;

         if(stra.get_count() >= 6)
         {

            x2 = stra[5];

         }

         if(stra.get_count() >= 7)
         {

            y2 = stra[6];

         }

         string cx2;

         string cy2;

         if(stra.get_count() >= 8)
         {

            cx2 = stra[7];

         }

         if(stra.get_count() >= 9)
         {

            cy2 = stra[8];

         }


         Application.m_strImageId = strImageId;

         rect r;

         Application.m_pwindow->GetWindowRect(r);

         if(x.has_char())
         {

            r.left = atoi(x);

         }

         if(y.has_char())
         {

            r.top = atoi(y);

         }

         if(w.has_char())
         {

            r.right = r.left+atoi(w);

         }

         if(h.has_char())
         {

            r.bottom = r.top+atoi(h);

         }

         fork([this,strImageId, r, x2, y2, cx2, cy2]()
         {

            string pathDots = file_as_string_dup(::dir::ca2_module() / "image_source.txt");

            pathDots.trim();

            pathDots.replace("%image%", strImageId);

            var varFile;

            varFile["url"] = pathDots;

            varFile["http_set"]["raw_http"] = true;

            varFile["http_set"]["disable_common_name_cert_check"] = true;

            synch_lock sl(Application.mutex());

            ::visual::dib_pointer dib(allocer());

            dib.load_from_file(varFile, false);

            Application.m_pwindow->m_dib  = dib;

            if(x2.has_char())
            {

               Application.m_pwindow->m_pt.x = atoi(x2);

            }

            if(y2.has_char())
            {

               Application.m_pwindow->m_pt.y = atoi(y2);

            }

            Application.m_pwindow->m_cx = atoi(cx2);

            Application.m_pwindow->m_cy = atoi(cy2);

            Application.m_pwindow->display(display_restore);

            Application.m_pwindow->SetWindowPos(ZORDER_TOP, r, SWP_SHOWWINDOW);

            Application.m_pwindow->set_need_redraw();

         });


      }
      else if(strScript == "/fullscreen")
      {

         string strCommand = System.url().get_param(strUrl, "command");

         stringa stra;

         stra.explode(",", strCommand);

         string strImageId = stra[0];

         rect r;

         Session.get_main_monitor(r);

         string x2;

         string y2;

         string cx2;

         string cy2;

         if(stra.get_count() >= 2)
         {

            x2 = stra[1];

         }

         if(stra.get_count() >= 3)
         {

            y2 = stra[2];

         }

         if(stra.get_count() >= 4)
         {

            cx2 = stra[3];

         }

         if(stra.get_count() >= 5)
         {

            cy2 = stra[4];

         }

         Application.m_strImageId = strImageId;

         fork([this,strImageId, r, x2, y2, cx2, cy2]()
         {

            string pathDots = file_as_string_dup(::dir::ca2_module() / "image_source.txt");

            pathDots.trim();

            pathDots.replace("%image%", strImageId);

            var varFile;

            varFile["url"] = pathDots;

            varFile["http_set"]["raw_http"] = true;

            varFile["http_set"]["disable_common_name_cert_check"] = true;

            ::visual::dib_pointer dib(allocer());

            dib.load_from_file(varFile, false);

            synch_lock sl(Application.mutex());

            Application.m_pwindow->m_dib  = dib;

            if(x2.has_char())
            {

               Application.m_pwindow->m_pt.x = atoi(x2);

            }

            if(y2.has_char())
            {

               Application.m_pwindow->m_pt.y = atoi(y2);

            }

            Application.m_pwindow->m_cx = atoi(cx2);

            Application.m_pwindow->m_cy = atoi(cy2);

            Application.m_pwindow->WfiFullScreen(r);

            Application.m_pwindow->set_need_redraw();

         });


      }


   }


} // namespace image_display




