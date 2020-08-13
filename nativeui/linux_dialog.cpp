#include "framework.h"

#include <gtk/gtk.h>
#include <libudev.h>
#include <blkid/blkid.h>
#include "list_wifi.h"

#define FONT_APP FONT_SANS_EX
#include "aura/aura/os/linux/linux_user_impl.h"

void helloWorld (GtkWidget *wid, GtkWidget *win);

void on_start(GtkWidget *wid, main_window * pmain);

void connect_wifi(GtkWidget *wid, main_window * pmain);

void install_version(GtkWidget *wid, main_window * pmain);

void screen_download(GtkWidget *wid, main_window * pmain);

void screen_browse(GtkWidget *wid, main_window * pmain);

void browse_for_file(GtkWidget *wid, main_window * pmain);

void screen_start_update(GtkWidget *wid, main_window * pmain);

void screen_log(GtkWidget *wid, main_window * pmain);

void screen_start_update_back(GtkWidget *wid, main_window * pmain);

void screen_update(GtkWidget *wid, main_window * pmain);

void restart_update(GtkWidget *wid, main_window * pmain);

void on_check_version(GtkWidget *wid, main_window * pmain);

void on_download(GtkWidget *wid, main_window * pmain);

void on_update_app(GtkWidget *wid, main_window * pmain);

void main_dialog_quit (GtkWidget *wid, main_window *pmain);

void restart_at_desktop(GtkWidget *wid, main_window *pmain);

void on_connect_wifi(GtkWidget *wid, main_window * pmain);

void on_back(GtkWidget *wid, main_window * pmain);

class main_window;


void screen_base::create_back_button(GtkBox * pbox)
{

   m_buttonBack.attach(gtk_button_new_with_label("Back"));

   m_buttonBack.set_font(m_pmain->m_fButton);

   m_buttonBack.set_margin(0, 0, 2, 0);

   m_buttonBack.add_class("wizard_button");

   g_object_set(m_buttonBack.m_pwidget, "height-request", m_pmain->m_iBottomHeightRequest, NULL);

   g_signal_connect (G_OBJECT (m_buttonBack.w()), "clicked", G_CALLBACK (on_back), (gpointer) m_pmain);

   gtk_box_pack_start (pbox, m_buttonBack, true, true, 0);

}


void screen_base::create_shutdown_button(GtkBox * pbox, bool bRightMost)
{

#if RESTART_AT_DESKTOP

   if(file_exists(::dir::home()/".quasar_science_desktop"))
   {

      m_buttonRestartAtDesktop.attach(gtk_button_new_with_label("Back to Desktop"));

      m_buttonRestartAtDesktop.set_font(m_pmain->m_fSmall);

      m_buttonRestartAtDesktop.set_margin(2, 0, 2, 0);

      g_signal_connect (G_OBJECT (m_buttonRestartAtDesktop.w()), "clicked", G_CALLBACK (restart_at_desktop), (gpointer) m_pmain);

      gtk_box_pack_start (pbox, m_buttonRestartAtDesktop, false, false, 0);

   }

#endif


   m_buttonShutdown.attach(gtk_button_new_with_label("Shutdown"));

   m_buttonShutdown.set_font(m_pmain->m_fButton);

   m_buttonShutdown.set_margin(2, 0, bRightMost ? 0 : 2, 0);

   g_object_set(m_buttonShutdown.m_pwidget, "height-request", m_pmain->m_iBottomHeightRequest, NULL);

   g_signal_connect (G_OBJECT (m_buttonShutdown.w()), "clicked", G_CALLBACK (main_dialog_quit), (gpointer) m_pmain);

   gtk_box_pack_start (pbox, m_buttonShutdown, true, true, 0);


}


class screen1 : virtual public screen_base
{
public:


   string               m_strAppStatus;

   ::os::window        m_vboxL;
   ::os::window        m_label;

   ::os::window        m_hboxUpdate;
   ::os::window        m_labelVersion;
   ::os::window        m_labelAppUpdate;
   ::os::window        m_buttonAppUpdate;

   ::os::window        m_vboxR;
   ::os::window        m_buttonDownload;
   ::os::window        m_buttonInstallVersion;
   ::os::window        m_buttonBrowse;

   bool                 m_bFirstShow;

   bool                 m_bUpdate;
   string               m_strAppUpdateMessage;


   screen1(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;

};


class screen2 : virtual public screen_base,
   virtual public wifi
{
public:


   GtkListStore *       m_store;
   ::os::window         m_tree;
   ::os::window         m_treescroll;
   ::os::window         m_buttonConnect;

   ::os::window         m_hboxControl;



   screen2(main_window * pmain);


   virtual void on_show() override;

   virtual void on_hide() override;

   virtual void on_wifi_update(stringa straDel, stringa straNew) override;

   virtual void on_wifi(string str);


   virtual void wifi_activating() override;

   virtual void wifi_activated() override;

   virtual void wifi_on_activate() override;

   virtual void wifi_on_activation_error() override;


};


class screen21 : virtual public screen_base
{
public:


   screen2 *            m_pscreen2;

   ::os::window         m_label;
   ::os::window         m_labelPassword;
   ::os::window         m_password;
   ::os::window         m_connect;


   ::os::window         m_hboxControl;



   screen21(screen2 * pscreen2);


   virtual void on_show() override;

   virtual void on_hide() override;

   virtual void start(string strSSID);

   virtual void do_connect_wifi();

};


class screen3 : virtual public screen_base
{
public:


   ::os::window         m_hboxVersion;
   ::os::window         m_buttonCheckVersion;
   ::os::window         m_labelVersion;

   ::os::window         m_hboxDownload;
   ::os::window         m_buttonDownload;
   ::os::window         m_labelDownload;

   ::os::window         m_hboxControl;
   ::os::window         m_buttonNext;


   screen3(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;
};


class screen4 : virtual public screen_base
{
public:


   ::file::patha        m_pathaRoot;
   ::file::patha        m_pathaMedia;
   ::file::patha        m_pathaRootExpanded;

   ::os::window         m_buttonBrowse;
   ::os::window         m_labelFile;

   ::os::window         m_hboxControl;
   ::os::window         m_buttonNext;


   ::os::window         m_hboxBrowse;
   GtkListStore *       m_storeFolder;
   ::os::window         m_treeFolder;
   ::os::window         m_treescrollFolder;
   GtkListStore *       m_storeFile;
   ::os::window         m_treeFile;
   ::os::window         m_treescrollFile;

   ::file::patha        m_patha;
   ::file::path         m_pathFolder;
   ::file::path         m_path;


   screen4(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;

   virtual void browse_folder(string str);
   virtual void list_file(string str);

};


class screen5 : virtual public screen_base
{
public:


   ::os::window         m_label;
   ::os::window         m_labelVersion;
   ::os::window         m_labelFirmwareVersion;

   ::os::window        m_hboxUpdate;
   ::os::window        m_labelAppUpdate;
   ::os::window        m_buttonAppUpdate;


   ::os::window         m_hboxControl;
   ::os::window         m_buttonUpdate;


   screen5(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;
};


class screen6 : virtual public screen_base
{
public:


   ::os::window         m_label;
   //::os::window         m_labelProgressStatus[4];

   ::os::window         m_hspan;

   ::os::window         m_column[2];

   stringa              m_straPort;

   ::os::window         m_hboxProgress[4];
   ::os::window         m_progress[4];
   ::os::window         m_labelProgress[4];

   ::os::window         m_hboxError[4];
   ::os::window         m_labelError[4];


   ::os::window         m_buttonShowLog;

   ::os::window         m_hboxControl;
   ::os::window         m_buttonRestartUpdate;

   screen6(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;

   virtual void add_port(const char * pszPort);
   virtual void remove_port(const char * pszPort);
   virtual int get_port_index(const char * pszPort);


};



class screen8 : virtual public screen_base
{
public:


   ::os::window         m_label;
   ::os::window         m_log;
   ::os::window         m_logscroll;

   ::os::window         m_hboxControl;


   screen8(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;

};






void main_interface::on_check_version_response(const char * pszStatus, bool bEnable)
{

   string strStatus(pszStatus);

   ::gdk_fork([=]()
   {

      __pointer(screen3) pscreen3 = m_pmainwindow->m_screenptra[30];

      pscreen3->m_labelVersion.set_window_text(strStatus);

      if (bEnable)
      {

         pscreen3->m_buttonDownload.enable_window(true);

         pscreen3->m_labelDownload.enable_window(true);

      }

   });

}


void main_interface::on_app_installation_response(const char * pszStatus)
{

   string strStatus(pszStatus);

   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = m_pmainwindow->m_screenptra[10];

      pscreen1->m_labelVersion.set_window_text(strStatus);

      __pointer(screen5) pscreen5 = m_pmainwindow->m_screenptra.element_at_grow(50);

      if(pscreen5.is_set())
      {

         pscreen5->m_labelVersion.set_window_text(strStatus);

      }

   });

}


void main_interface::on_radio_download_response(bool bDownloadEnable, bool bUpdateEnable)
{

//   m_pmainwindow->m_dlg1.m_download.enable_window(bDownloadEnable);
//
//   m_pmainwindow->m_dlg1.m_downloadProgress.enable_window(bDownloadEnable);
//
//   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


void main_interface::on_radio_browse_response(bool bUpdateEnable)
{

//   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


void main_interface::on_download_response(const char * pszStatus, bool bUpdateEnable)
{

   string strStatus(pszStatus);

   ::gdk_fork([=]()
   {

      __pointer(screen3) pscreen3 = m_pmainwindow->m_screenptra[30];

      pscreen3->m_labelDownload.set_window_text(strStatus);

      pscreen3->m_buttonNext.enable_window(bUpdateEnable);

   });

}


void main_interface::on_browse_response(bool bUpdateEnable)
{

   //gdk_lock l;
   ::gdk_fork([=]()
   {

      m_pmainwindow->screen(50);

   });

}


//void main_interface::set_progress_status(const char * pszPort, const char * psz)
//{
//
//   string strStatus(psz);
//
//   ::gdk_fork([=]()
//   {
//
//      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];
//
//      int iPort = pscreen6->get_port_index(pszPort);
//
//      if(iPort >= 0)
//      {
//
//      if(m_bError)
//      {
//
//         gtk_label_set_markup (pscreen6->m_labelProgressStatus[iPort].l(), string("<span foreground=\"red\" >") + strStatus + string("</span>"));
//
//      }
//      else
//      {
//
//         pscreen6->m_labelProgressStatus[iPort].set_window_text(strStatus);
//
//      }
//
//      }
//   });
//
//}


void main_interface::set_result(const char * pszPort, const char * psz)
{

   string strPort(pszPort);

   string str(psz);

   ::gdk_fork([=]()
   {

      string strStatus;

      string strShortPort(strPort);

      ::str::begins_eat_ci(strShortPort, "/dev/");

      if(::str::begins_ci(strShortPort, "ttyusb"))
      {

         ::str::begins_eat_ci(strShortPort, "tty");

      }
      else if(::str::begins_ci(strShortPort, "ttyusb"))
      {

         ::str::begins_eat_ci(strShortPort, "tty");

         strShortPort = "serial" + strShortPort;

      }

      strStatus = strShortPort + ": " + str;

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      int iPort = pscreen6->get_port_index(strPort);

      if(iPort >= 0)
      {

         if(m_bError)
         {

            gtk_label_set_markup (pscreen6->m_labelError[iPort].l(), "<span foreground=\"red\" >" + strStatus + "</span>");

         }
         else
         {

            pscreen6->m_labelError[iPort].set_window_text(strStatus);

         }

      }

   });

}


void main_interface::set_progress(const char * pszPort, int iPercent, const char * psz)
{

   string strPort(pszPort);

   string str(psz);

   ::gdk_fork([=]()
   {

      string strStatus;

      strStatus = str;

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      int iPort = pscreen6->get_port_index(pszPort);

      if(iPort >= 0)
      {

         if(pscreen6.is_set())
         {

            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pscreen6->m_progress[iPort].w()), (double) iPercent / 100.0);

            if(iPercent == 0)
            {

               pscreen6->m_labelProgress[iPort].set_window_text("0%");

            }
            else
            {

               pscreen6->m_labelProgress[iPort].set_window_text(strStatus);

            }

         }
         else
         {

         }

      }

   });

}


void main_interface::add_port(const char * pszPort)
{

   string strPort(pszPort);

   ::gdk_fork([=]()
   {

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      pscreen6->add_port(pszPort);

   });

}


void main_interface::remove_port(const char * pszPort)
{

   string strPort(pszPort);

   ::gdk_fork([=]()
   {

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      pscreen6->remove_port(pszPort);

   });

}

int screen6::get_port_index(const char * pszPort)
{

   for(index i = 0; i < m_straPort.get_count(); i++)
   {

      if(m_straPort[i].compare_ci(pszPort) == 0)
      {

         return i;

      }

   }

   return -1;
}

void main_interface::show_manual_com(char * const * pszList, int iCount, int iSel)
{

//   m_pmainwindow->m_dlg2.m_combo.send_message(CB_RESETCONTENT);
//
//   for (int i = 0; i < iCount; i++)
//   {
//
//      m_pmainwindow->m_dlg2.m_combo.send_message(CB_ADDSTRING, 0, (LPARAM) pszList[i]);
//
//   }
//
//   m_pmainwindow->m_dlg2.m_combo.send_message(CB_SETCURSEL, iSel);
//
//   m_pmainwindow->m_dlg2.m_combo.show_window(SW_SHOW);
//
//   m_pmainwindow->m_dlg2.m_update2.show_window(SW_SHOW);

}


void main_interface::hide_manual_com()
{

//   m_pmainwindow->m_dlg2.m_combo.show_window(SW_HIDE);
//   m_pmainwindow->m_dlg2.m_update2.show_window(SW_HIDE);
//   m_pmainwindow->m_dlg2.m_combo.send_message(CB_RESETCONTENT);

}

void main_interface::cancel_enable(bool bEnable)
{

//   m_pmainwindow->m_cancel.enable_window(bEnable);

}


void main_interface::update_enable(bool bEnable)
{

//   m_pmainwindow->m_update.enable_window(bEnable);

}

void main_interface::redraw_window()
{

//   m_pmainwindow->m_dlg2.m_progressControl.redraw_window();

}


void main_interface::to_close()
{

//   m_pmainwindow->m_cancel.set_window_text("Close");

}

void main_interface::to_cancel()
{

//   m_pmainwindow->m_cancel.set_window_text("Cancel");

}




void main_interface::on_app_check_version(const char * pszChar)
{

   //gdk_lock l;
   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = m_pmainwindow->m_screenptra[10];

      pscreen1->m_bUpdate = true;

      pscreen1->m_strAppUpdateMessage = pszChar;

      if(m_pmainwindow->m_iCurrent == 10)
      {

         pscreen1->on_show();

      }
      else if(m_pmainwindow->m_iCurrent == 50)
      {

         __pointer(screen5) pscreen5 = m_pmainwindow->m_screenptra[50];

         pscreen5->on_show();

      }

   });

}


main_window::main_window(::aura::application * papp) :
   ::object(papp)
{

   m_iCurrent = -1;
   m_pscreen = NULL;

}


void main_window::open_file(string str)
{

   __pointer(screen4) pscreen4 = m_screenptra[40];

   pscreen4->m_labelFile.set_window_text(str);

   m_pmain->on_browse(str);

}


void main_window::reset_size ()
{

   int w = m_w;

   int h = m_h;

   gtk_widget_set_size_request(m_vboxMain.w(), w, h);

   gtk_window_resize(m_vboxMain.win(), w, h);

failed_get_screen_size:;

   GdkGeometry geom;

   geom.base_width = w;

   geom.base_height = h;

   geom.max_width = w;

   geom.max_height = h;

   geom.min_width = w;

   geom.min_height = h;

   geom.win_gravity = GDK_GRAVITY_CENTER;

   gtk_widget_set_size_request(this->w(), w, h);

   gtk_window_set_geometry_hints(win(), NULL, &geom,   (GdkWindowHints) (GDK_HINT_MIN_SIZE  | GDK_HINT_MAX_SIZE |  GDK_HINT_BASE_SIZE  | GDK_HINT_WIN_GRAVITY));

   gtk_window_set_default_size(win(), w, h);

   gtk_window_resize(win(), w, h);



}


void main_window::show ()
{

   {

      int w = 480;

      int h = 320;

      {

         GdkDisplay * pdisplayDefault = gdk_display_get_default();

         if(pdisplayDefault == NULL)
         {

            goto failed_get_screen_size;

         }

         GdkMonitor * pmonitorPrimary = gdk_display_get_primary_monitor(pdisplayDefault);

         if(pmonitorPrimary == NULL)
         {

            goto failed_get_screen_size;

         }

         GdkRectangle rectangleGeometry;

         gdk_monitor_get_geometry(pmonitorPrimary, &rectangleGeometry);

         w = MAX(w, rectangleGeometry.width);

         h = MAX(h, rectangleGeometry.height);

         if(w >= 1024)
         {

            w = 480;

            h = 320;

         }

      }

failed_get_screen_size:;

      m_w = w;

      m_h = h;

   }

   m_iBottomHeightRequest = m_h * 9 / 50;

//   GApplication * papp = g_application_get_default ();

   m_pwidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);

//  gtk_application_add_window(GTK_APPLICATION(papp), GTK_WINDOW(m_pwidget));

   gtk_widget_realize (m_pwidget);

   GtkCssProvider * provider = gtk_css_provider_get_default ();


   GError * perror = NULL;

   const char * pdata = "style \"wizard_button\" {\"" \
                        "padding-top: 10px;" \
                        "padding-bottom: 10px;" \
                        "}";

   gtk_css_provider_load_from_data (provider,
                                    pdata,
                                    -1,
                                    &perror);


//   gtk_style_context_add_provider_for_screen
//                               (gdk_screen_get_default(),
//                                provider,
//                                 GTK_STYLE_PROVIDER_PRIORITY_USER);
//
//
   gtk_container_set_border_width (GTK_CONTAINER (m_pwidget), 8);

   gtk_window_set_title (GTK_WINDOW (m_pwidget), "Quasar Science Update Utility");

//  gtk_window_maximize(GTK_WINDOW (m_pwidget));

//  gtk_window_set_default_size(win(), 480, 320);

//  gtk_widget_set_size_request(w(), 480, 320);
//
//  gtk_window_resize(win(), 480, 320);
//
//  gtk_window_set_resizable (win(), false);

   gtk_window_set_decorated(win(), false);

   g_signal_connect (m_pwidget, "destroy", G_CALLBACK (main_dialog_quit), (gpointer) this);

   m_vboxMain.attach(gtk_vbox_new (FALSE, 6));

   gtk_container_add (GTK_CONTAINER(m_pwidget), m_vboxMain);

   m_hboxTop.attach(gtk_hbox_new (FALSE, 6));

   gtk_widget_set_vexpand(m_hboxTop, false);

   gtk_box_pack_start (GTK_BOX (m_vboxMain.w()), m_hboxTop, false, false, 0);

   m_hboxTitle.attach(gtk_hbox_new (FALSE, 6));

   gtk_widget_set_vexpand(m_hboxTitle, false);

   gtk_widget_set_hexpand(m_hboxTitle, false);

   gtk_box_pack_start (GTK_BOX (m_hboxTop.w()), m_hboxTitle, false, false, 0);

   m_wLogo.attach(gtk_image_new_from_file("icon_logo.png"));

   gtk_widget_set_vexpand(m_wLogo, false);

   gtk_widget_set_hexpand(m_wLogo, false);

   gtk_box_pack_start (GTK_BOX (m_hboxTitle.w()), m_wLogo, false, false, 0);

   m_fSmall.create_point_font(150/2, FONT_APP, 400);

   m_fTitle.create_point_font(350/2, FONT_APP, 800);

   m_fTitle2.create_point_font(210/2, FONT_APP, 400);

   m_fTextBig.create_point_font(240/2, FONT_APP, 800);

   m_fTitle2Bold.create_point_font(210/2, FONT_APP, 800);

// m_wTitle.attach(gtk_label_new("Quasar Science Update Utility"));

// m_wTitle.set_font(m_fTitle);

   m_wTitle.attach(gtk_image_new_from_file("text.png"));

   gtk_widget_set_vexpand(m_wTitle, false);

   gtk_widget_set_hexpand(m_wTitle, false);

   m_fText.create_point_font(190/2, FONT_APP, 400);

   m_fButton.create_point_font(230/2, FONT_APP, 400);

   gtk_box_pack_start (GTK_BOX (m_hboxTitle.w()), m_wTitle, false, false, 0);

   m_labelTopRight.attach(gtk_label_new(""));

   gtk_widget_set_hexpand(m_labelTopRight, true);

   gtk_widget_set_vexpand(m_labelTopRight, false);

   gtk_box_pack_start (GTK_BOX (m_hboxTop.w()), m_labelTopRight, true, true, 0);

   m_buttonStartOver.attach(gtk_button_new_with_label("Start Over"));

   m_buttonStartOver.set_font(m_fButton);

   gtk_widget_set_hexpand(m_buttonStartOver, true);

   gtk_widget_set_vexpand(m_buttonStartOver, false);

   m_buttonStartOver.set_margin(2, 0, 0, 0);

   g_signal_connect (G_OBJECT (m_buttonStartOver.w()), "clicked", G_CALLBACK (on_start), (gpointer) this);

   screen(10);

   gtk_widget_show_all (w());

   gtk_window_set_position (win(), GTK_WIN_POS_CENTER_ALWAYS);

   //gtk_window_fullscreen(win());

   reset_size();

}


void main_window::screen(int i)
{

   if(m_screenptra.get_size() <= i)
   {

      m_screenptra.set_size(i+1);

   }

   screen_base * pscreen = m_screenptra[i];

   if(pscreen == NULL)
   {

      switch(i)
      {
      case 10:
      {

         pscreen = new screen1(this);

      }
      break;
      case 20:
      {

         pscreen = new screen2(this);

      }
      break;
      case 21:
      {

         pscreen = new screen21(dynamic_cast < screen2 * > (m_screenptra[20]));

      }
      break;
      case 30:
      {

         pscreen = new screen3(this);

      }
      break;

      case 40:
      {

         pscreen = new screen4(this);

      }
      break;
      case 50:
      {

         pscreen = new screen5(this);

      }
      break;
      case 60:
      {

         pscreen = new screen6(this);

      }
      break;
      case 80:
      {

         pscreen = new screen8(this);

      }
      break;
      }

      if(pscreen == NULL)
      {

         return;

      }

      m_screenptra[i] = pscreen;

   }

   int iPrevious = m_iCurrent;

   m_iCurrent = i;

   if(m_pscreen != NULL)
   {

      m_pscreen->on_hide();

      g_object_ref(m_pscreen->m_main);

      m_pscreen->m_main.show_window(SW_HIDE);

      gtk_container_remove(GTK_CONTAINER(m_vboxMain.w()), m_pscreen->m_main);

   }

   m_pscreen = pscreen;

   if(iPrevious < i)
   {

      m_pscreen->m_iPrevious = iPrevious;

   }

   gtk_box_pack_start (GTK_BOX (m_vboxMain.w()), m_pscreen->m_main, true, true, 0);

   //gtk_widget_set_vexpand(m_pscreen->m_main, true);

   pscreen->on_show();

   reset_size();

}


void screen_base::on_show()
{

   if(gtk_widget_get_parent(m_pmain->m_labelTopRight) != NULL)
   {

      g_object_ref(m_pmain->m_labelTopRight);

      m_pmain->m_labelTopRight.show_window(SW_HIDE);

      gtk_container_remove(GTK_CONTAINER(m_pmain->m_vboxMain.w()), m_pmain->m_labelTopRight);

   }

   if(gtk_widget_get_parent(m_pmain->m_buttonStartOver) != NULL)
   {

      g_object_ref(m_pmain->m_buttonStartOver);

      m_pmain->m_buttonStartOver.show_window(SW_HIDE);

      gtk_container_remove(GTK_CONTAINER(m_pmain->m_vboxMain.w()), m_pmain->m_buttonStartOver);

   }

}


void screen_base::on_hide()
{



}

screen1::screen1(main_window * pmain) :
   object(pmain->get_context_application())
{

   m_bUpdate = false;

   m_bFirstShow = true;

   m_pmain = pmain;

   m_main.attach(gtk_hbox_new (true, 6));

   m_vboxL.attach(gtk_vbox_new (false, 6));

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_vboxL, true, true, 0);

   m_label.attach(gtk_label_new("This utility will update the firmware on your Q-Rainbow Linear Lamp."));

   m_label.set_font(m_pmain->m_fText);

   m_label.set_margin(0, 4, 4, 0);

   m_label.set_multiline();

   m_label.set_align_top_left();

   gtk_box_pack_start (GTK_BOX(m_vboxL.w()), m_label, true, true, 0);




   m_hboxUpdate.attach(gtk_hbox_new (false, 6));

   m_hboxUpdate.set_margin(0, 0, 4, 0);

   gtk_box_pack_start (GTK_BOX (m_vboxL.w()), m_hboxUpdate, false, false, 0);

   string strVersion = ::version::get_version();

   m_labelVersion.attach(gtk_label_new(strVersion));

   m_labelVersion.set_font(m_pmain->m_fSmall);

   m_labelVersion.set_align_left();

   m_labelVersion.set_align_bottom();

   gtk_box_pack_start (GTK_BOX(m_hboxUpdate.w()), m_labelVersion, false, false, 0);

   m_labelAppUpdate.attach(gtk_label_new(""));

   m_labelAppUpdate.set_font(m_pmain->m_fSmall);

   m_labelAppUpdate.set_align_left();

   m_labelAppUpdate.set_align_bottom();


   m_buttonAppUpdate.attach(gtk_button_new_with_label("Update"));

   m_buttonAppUpdate.set_font(m_pmain->m_fSmall);

   g_signal_connect (G_OBJECT (m_buttonAppUpdate.w()), "clicked", G_CALLBACK (on_update_app), (gpointer) m_pmain);



   m_vboxR.attach(gtk_vbox_new (TRUE, 0));

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_vboxR, TRUE, TRUE, 0);

   m_buttonDownload.attach(gtk_button_new_with_label("Download Latest Version From Web"));

   ::os::window w;

   w.attach(gtk_bin_get_child(GTK_BIN(m_buttonDownload.w())));

   w.set_multiline();

   m_buttonDownload.set_margin(0, 4, 0, 2);

   g_signal_connect (G_OBJECT (m_buttonDownload.w()), "clicked", G_CALLBACK (connect_wifi), (gpointer) m_pmain);

   gtk_box_pack_start (GTK_BOX (m_vboxR.w()), m_buttonDownload, TRUE, TRUE, 0);

   m_buttonDownload.set_font(m_pmain->m_fButton);



   m_buttonInstallVersion.attach(gtk_button_new_with_label("Install Version"));

   w.attach(gtk_bin_get_child(GTK_BIN(m_buttonInstallVersion.w())));

   w.set_multiline();

   m_buttonInstallVersion.set_margin(0, 2, 0, 2);

   g_signal_connect (G_OBJECT (m_buttonInstallVersion.w()), "clicked", G_CALLBACK (install_version), (gpointer) m_pmain);

   gtk_box_pack_start (GTK_BOX (m_vboxR.w()), m_buttonInstallVersion, TRUE, TRUE, 0);

   m_buttonInstallVersion.set_font(m_pmain->m_fButton);





   m_buttonBrowse.attach(gtk_button_new_with_label ("Choose Existing File on Device"));


   ::os::window wBrowse;

   wBrowse.attach(gtk_bin_get_child(GTK_BIN(m_buttonBrowse.w())));

   wBrowse.set_multiline();

   m_buttonBrowse.set_margin(0, 2, 0, 0);

   g_signal_connect (G_OBJECT (m_buttonBrowse.w()), "clicked", G_CALLBACK (screen_browse), (gpointer) m_pmain);

   gtk_box_pack_start (GTK_BOX (m_vboxR.w()), m_buttonBrowse, TRUE, TRUE, 0);

   m_buttonBrowse.set_font(m_pmain->m_fButton);

}


void screen1::on_show()
{

#ifdef RASPBIAN

   //::system("sudo mount -a");

#endif

   if(m_bFirstShow)
   {

      m_bFirstShow = false;

      Application.app_check_version();

      Application.check_downloaded_bin();

   }

   screen_base::on_show();

   gtk_widget_show(m_main);

   gtk_widget_show(m_vboxL);

   gtk_widget_show(m_label);

   gtk_widget_show(m_hboxUpdate);

   gtk_widget_show(m_labelVersion);

   gtk_widget_show_all(m_vboxR);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.set_window_text("");

   m_pmain->m_labelTopRight.show_window(SW_SHOW);

   if(m_bUpdate)
   {

      m_labelAppUpdate.set_window_text(m_strAppUpdateMessage);

      m_labelAppUpdate.show_window(SW_SHOW);

      m_buttonAppUpdate.show_window(SW_SHOW);

      gtk_box_pack_start (GTK_BOX(m_hboxUpdate.w()), m_labelAppUpdate, true, true, 0);
      gtk_box_pack_start (GTK_BOX(m_hboxUpdate.w()), m_buttonAppUpdate, false, false, 0);

   }
   else
   {

      m_labelAppUpdate.show_window(SW_HIDE);

      m_buttonAppUpdate.show_window(SW_HIDE);

      gtk_container_remove(GTK_CONTAINER(m_hboxUpdate.w()), m_labelAppUpdate);
      gtk_container_remove(GTK_CONTAINER(m_hboxUpdate.w()), m_buttonAppUpdate);

   }

}

void screen1::on_hide()
{

   screen_base::on_hide();

}


void list_data_func (GtkTreeViewColumn * col, GtkCellRenderer * renderer, GtkTreeModel * model, GtkTreeIter * iter, gpointer user_data)
{

   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, NULL);

}

void tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{

   GtkTreeIter iter;
   GtkTreeModel * model;
   gchar * ssid;

   if (gtk_tree_selection_get_selected (selection, &model, &iter))
   {

      gtk_tree_model_get (model, &iter, 0, &ssid, -1);

      screen2 * pscreen2 = (screen2 *) data;

      string strSSID = ssid;

      g_free (ssid);

      //pscreen2->get_context_application()->fork([=]()
      {

         pscreen2->on_wifi(strSSID);

      }

      //);

   }

}


screen2::screen2(main_window * pmain) :
   object(pmain->get_context_application()),
   wifi(pmain->get_context_application())
{

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 0));

   m_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_INT);

   m_treescroll.attach(gtk_scrolled_window_new(NULL, NULL));

   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(m_treescroll.w()), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

   m_tree.attach(gtk_tree_view_new_with_model (GTK_TREE_MODEL (m_store)));

   g_object_unref(m_store);

   GtkCellRenderer * renderer = gtk_cell_renderer_text_new();

   GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes ("Wi-Fi Network", renderer, "text", 0, "weight", PANGO_WEIGHT_BOLD, NULL);

   gtk_tree_view_column_set_min_width(column, 150);

   gtk_tree_view_column_set_expand (column, true);

   gtk_tree_view_column_set_cell_data_func(column, renderer, list_data_func, NULL, NULL);

   gtk_tree_view_append_column (GTK_TREE_VIEW (m_tree.w()), column);

   column = gtk_tree_view_column_new_with_attributes ("Strength (%)", renderer, "text", 1, "weight", PANGO_WEIGHT_BOLD, NULL);

   gtk_tree_view_column_set_max_width(column, 150);

   gtk_tree_view_column_set_min_width(column, 80);

   gtk_tree_view_column_set_expand (column, false);

   gtk_tree_view_column_set_cell_data_func(column, renderer, list_data_func, NULL, NULL);

   GtkTreeSelection * pselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_tree.w()));

   gtk_tree_selection_set_mode(pselection, GTK_SELECTION_BROWSE);

   g_signal_connect (G_OBJECT (pselection), "changed", G_CALLBACK (tree_selection_changed_cb), (gpointer)this);


   gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(m_store), 1, GTK_SORT_DESCENDING);

   gtk_tree_view_append_column (GTK_TREE_VIEW (m_tree.w()), column);

   //gtk_tree_view_set_fixed_height_mode(GTK_TREE_VIEW(m_tree.w()), true);

   gtk_container_add(GTK_CONTAINER(m_treescroll.w()), m_tree.w());

   gtk_widget_set_vexpand(m_tree.w(), true);

   gtk_widget_set_vexpand(m_treescroll.w(), true);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_treescroll.w(), true, true, 0);

   m_tree.set_margin(0, 4, 0, 4);


   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box(), true);

}

void screen2::on_wifi_update(stringa straDel, stringa straNew)
{

   for(index i = straDel.get_upper_bound(); i >= 0; i--)
   {

      GtkTreeIter iter;

      if(!gtk_tree_model_get_iter_first (GTK_TREE_MODEL(m_store), &iter))
      {

         break;

      }

      do
      {

         gchararray place_string_here = NULL;

         gtk_tree_model_get (GTK_TREE_MODEL(m_store), &iter, 0, &place_string_here, -1);

         if(!strcasecmp(place_string_here, straDel[i]))
         {

            gtk_list_store_remove(m_store, &iter);

            continue;

         }

         g_free(place_string_here);

      }
      while(gtk_tree_model_iter_next (GTK_TREE_MODEL(m_store), &iter));

   }


   for(auto strSSID : straNew)
   {

      GtkTreeIter iter;

      gtk_list_store_append (m_store, &iter);  /* Acquire an iterator */

      gtk_list_store_set(m_store, &iter, 0,  strSSID.c_str(), 1, m_mapStrength[strSSID], -1);

   }

}


void screen2::on_show()
{

   //string strSSID = file_as_string_dup(::dir::home()/".wifissid");

   //if(strSSID.has_char())
   //{

   //   on_wifi(strSSID);

   //   return;

   //}

   m_bScan = true;

   screen_base::on_show();

   m_strPass.Empty();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);

}

void screen2::on_hide()
{

   screen_base::on_hide();

   m_bScan = false;

}


void screen2::on_wifi(string strSSID)
{

   m_pmain->screen(21);

   screen21 * pscreen21 = dynamic_cast < screen21 * > (m_pmain->m_screenptra[21]);

   pscreen21->start(strSSID);

}


void screen2::wifi_activating()
{

   //gdk_lock l;

   ::gdk_fork([=]()
   {

      screen21 * pscreen21 = dynamic_cast < screen21 * > (m_pmain->m_screenptra[21]);

      pscreen21->m_label.set_window_text(m_strSSID + ": Connecting");

   });

}

void screen2::wifi_activated()
{

   //gdk_lock l;
   ::gdk_fork([=]()
   {


      screen21 * pscreen21 = dynamic_cast < screen21 * > (m_pmain->m_screenptra[21]);

      pscreen21->m_label.set_window_text(m_strSSID + ": Connected");

      ::system("sudo systemctl restart network-manager");

   });

}

void screen2::wifi_on_activate()
{

   //gdk_lock l;
   ::gdk_fork([=]()
   {

      m_pmain->screen(30);

   });

}

void screen2::wifi_on_activation_error()
{

   screen21 * pscreen21 = dynamic_cast < screen21 * > (m_pmain->m_screenptra[21]);

   gtk_label_set_markup (pscreen21->m_label.l(), string("<span foreground=\"red\" >") + m_strSSID + ": Failed to connect." + string("</span>"));

   ::system("matchbox-keyboard &");

   ::Sleep(800);

   ::system("wmctrl -r Keyboard -e 0,0,0,-1,-1");

   Sleep(400);

   pscreen21->m_password.set_focus();


}


screen21::screen21(screen2 * pscreen2) :
   object(pscreen2->get_context_application()),
   m_pscreen2(pscreen2)
{

   m_pmain = pscreen2->m_pmain;

   m_main.attach(gtk_vbox_new (false, 0));

   m_label.attach(gtk_label_new(""));

   m_label.set_font(m_pmain->m_fTitle2Bold);

   m_label.set_margin(0, 4, 0, 2);

   gtk_label_set_xalign(m_label.l(), 0.0);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_label, true, true, 0);


   m_labelPassword.attach(gtk_label_new("Wi-fi Network Passphrase"));

   m_labelPassword.set_font(m_pmain->m_fTitle2);

   m_labelPassword.set_margin(0, 2, 0, 2);

   gtk_label_set_xalign(m_labelPassword.l(), 0.00);


   gtk_box_pack_start (GTK_BOX (m_main.w()), m_labelPassword, false, false, 0);

   m_password.attach(gtk_entry_new());

   //gtk_entry_set_visibility(GTK_ENTRY(m_password.w()), false);

   //gtk_entry_set_input_purpose(GTK_ENTRY(m_password.w()), GTK_INPUT_PURPOSE_PASSWORD);

   m_password.set_margin(0, 2, 0, 2);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_password, false, false, 0);





   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box(), false);

   m_connect.attach(gtk_button_new_with_label("Connect"));

   m_connect.set_margin(2, 0, 0, 0);

   gtk_box_pack_start (GTK_BOX (m_hboxControl.w()), m_connect, true, true, 0);


   g_signal_connect (G_OBJECT (m_connect.w()), "clicked", G_CALLBACK (on_connect_wifi), (gpointer) m_pmain);


   gtk_label_set_label(m_pmain->m_labelTopRight.l(), "Wi-fi Credentials");

   m_pmain->m_labelTopRight.set_font(m_pmain->m_fTitle2);

   m_pmain->m_labelTopRight.set_multiline();






}





void screen21::on_show()
{

   //string strPass = file_as_string_dup(::dir::home()/".wifipass");

   //if(strPass.has_char())
   //{

   //   m_pscreen2->on_wifi(strPass);

   //   return;

   //}


   m_password.set_focus();

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);

}

void screen21::on_hide()
{

   ::system("killall -9 matchbox-keyboard");

   screen_base::on_hide();

}


void screen21::start(string strSSID)
{

   string strPass = file_as_string_dup(::dir::home() / ".wifipass");

   stringa straPass;

   straPass.add_lines(strPass);

   string strPassword = straPass._007GetLine(strSSID);

   m_pscreen2->m_strPass = strPassword;

   gtk_entry_set_text(GTK_ENTRY(m_password.w()), strPassword);

   m_label.set_window_text(strSSID);

   m_pscreen2->m_strSSID = strSSID;

   if(m_pscreen2->connect_wifi(true))
   {

      ::system("killall -9 matchbox-keyboard");

   }
   else
   {

      ::system("matchbox-keyboard &");

      ::Sleep(800);

      ::system("wmctrl -r Keyboard -e 0,0,0,-1,-1");

      Sleep(400);


   }

}

void screen21::do_connect_wifi()
{

   string strPass = gtk_entry_get_text (GTK_ENTRY(m_password.w()));

   m_label.set_window_text(m_pscreen2->m_strSSID);

   m_pscreen2->m_strPass = strPass;

   ::system("killall -9 matchbox-keyboard");

   m_pscreen2->connect_wifi(false);

}

screen3::screen3(main_window * pmain)
{


   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 0));



   m_hboxVersion.attach(gtk_hbox_new(TRUE, 0));

   m_buttonCheckVersion.attach(gtk_button_new_with_label("Check Version"));

   m_buttonCheckVersion.set_font(m_pmain->m_fButton);

   m_buttonCheckVersion.set_margin(0, 4, 4, 2);


   gtk_box_pack_start (m_hboxVersion.box(), m_buttonCheckVersion, TRUE, TRUE, 0);

   g_signal_connect (G_OBJECT (m_buttonCheckVersion.w()), "clicked", G_CALLBACK (on_check_version), (gpointer) m_pmain);

   m_labelVersion.attach(gtk_label_new(""));

   m_labelVersion.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_hboxVersion.box(), m_labelVersion, TRUE, TRUE, 0);

   gtk_box_pack_start (m_main.box(), m_hboxVersion, TRUE, TRUE, 0);



   m_hboxDownload.attach((gtk_hbox_new(TRUE, 0)));

   m_buttonDownload.attach(gtk_button_new_with_label("Download"));

   m_buttonDownload.set_font(m_pmain->m_fButton);

   m_buttonDownload.enable_window(false);

   m_buttonDownload.set_margin(0, 2, 4, 4);

   gtk_box_pack_start (m_hboxDownload.box(), m_buttonDownload, TRUE, TRUE, 0);

   g_signal_connect (G_OBJECT (m_buttonDownload.w()), "clicked", G_CALLBACK (on_download), (gpointer) m_pmain);

   m_labelDownload.attach(gtk_label_new("0%"));

   m_labelDownload.set_font(m_pmain->m_fText);

   m_labelDownload.enable_window(false);

   gtk_box_pack_start (m_hboxDownload.box(), m_labelDownload, TRUE, TRUE, 0);

   gtk_box_pack_start (m_main.box(), m_hboxDownload, TRUE, TRUE, 0);


   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box(), false);

   m_buttonNext.attach(gtk_button_new_with_label("Next"));

   m_buttonNext.set_font(m_pmain->m_fButton);


   m_buttonNext.enable_window(false);

   m_buttonNext.set_margin(2, 0, 0, 0);

   g_signal_connect (G_OBJECT (m_buttonNext.w()), "clicked", G_CALLBACK (screen_start_update), (gpointer) m_pmain);

   gtk_box_pack_start (m_hboxControl.box(), m_buttonNext, true, true, 0);



}


void screen3::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);
   gtk_label_set_label(m_pmain->m_labelTopRight.l(), "Download Firmware From Web");

   m_pmain->m_labelTopRight.set_font(m_pmain->m_fTitle2);
   m_pmain->m_labelTopRight.set_multiline();

}

void screen3::on_hide()
{

   screen_base::on_hide();

}


void on_check_version(GtkWidget *wid, main_window * pmain)
{

   pmain->m_pmain->check_version();

}


void on_download(GtkWidget *wid, main_window * pmain)
{

   pmain->m_pmain->download();

}

void on_connect_wifi(GtkWidget *wid, main_window * pmain)
{

   pmain->do_connect_wifi();

}

void folder_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{

   GtkTreeIter iter;
   GtkTreeModel * model;
   gchar * folder;

   if(gtk_tree_selection_count_selected_rows(selection) == 1)
   {

      if (gtk_tree_selection_get_selected (selection, &model, &iter))
      {

         gtk_tree_model_get (model, &iter, 0, &folder, -1);

         screen4 * pscreen4 = (screen4 *) data;

         string strFolder = folder;

         g_free (folder);

         pscreen4->m_pmain->get_context_application()->fork([=]()
         {

            Sleep(200);

            ::gdk_fork([=]()
            {
               //gdk_lock l;

               ::file::path pathFolder = pscreen4->m_pathFolder;

               if(pscreen4->m_pathFolder == "/")
               {
                  int iFind = pscreen4->m_pathaRootExpanded.
                  pred_find_first([=](auto & path){ return path.name() == strFolder;});

                  if(iFind >= 0)
                  {

                     pathFolder = pscreen4->m_pathaRootExpanded[iFind];

                  }
                  else
                  {

                     pathFolder = "/";

                  }

               }
               else if(strFolder == "..")
               {

                  pathFolder = pathFolder.folder();

                  if(pathFolder.is_empty())
                  {

                     pathFolder = "/";

                  }

               }
               else
               {

                  pathFolder /= strFolder;

               }

               pscreen4->browse_folder(pathFolder);

            });

         });

      }

   }

}


void file_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{

   GtkTreeIter iter;
   GtkTreeModel * model;
   gchar * file;

   if(gtk_tree_selection_count_selected_rows(selection) == 1)
   {

      if (gtk_tree_selection_get_selected (selection, &model, &iter))
      {

         gtk_tree_model_get (model, &iter, 0, &file, -1);

         screen4 * pscreen4 = (screen4 *) data;

         string strFile = file;

         g_free (file);

         Sleep(200);

         ::file::path pathFile = pscreen4->m_pathFolder;

         pathFile /= strFile;

         pscreen4->m_pmain->m_pmain->on_browse(pathFile);

      }

   }

}


screen4::screen4(main_window * pmain) :
object(pmain->get_context_application())
{

   m_pathaRoot.add(Context.dir().download());

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 4));

   m_storeFolder = gtk_list_store_new (1, G_TYPE_STRING);

   m_hboxBrowse.attach(gtk_hbox_new (true, 4));

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_hboxBrowse, true, true, 0);

   m_treescrollFolder.attach(gtk_scrolled_window_new(NULL, NULL));

   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(m_treescrollFolder.w()), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   m_treeFolder.attach(gtk_tree_view_new_with_model (GTK_TREE_MODEL (m_storeFolder)));

   GtkCellRenderer * renderer = gtk_cell_renderer_text_new();

   GtkTreeViewColumn * column = gtk_tree_view_column_new_with_attributes ("Folder", renderer, "text", 0, NULL);

   gtk_tree_view_column_set_min_width(column, 150);

   gtk_tree_view_column_set_expand (column, false);

   gtk_tree_view_append_column (GTK_TREE_VIEW (m_treeFolder.w()), column);

   GtkTreeSelection * pselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_treeFolder.w()));

   gtk_tree_selection_set_mode(pselection, GTK_SELECTION_SINGLE);

   g_signal_connect (G_OBJECT (pselection), "changed", G_CALLBACK (folder_selection_changed_cb), (gpointer)this);

   gtk_container_add(GTK_CONTAINER(m_treescrollFolder.w()), m_treeFolder.w());

   gtk_box_pack_start (GTK_BOX (m_hboxBrowse.w()), m_treescrollFolder.w(), true, true, 0);


   m_storeFile = gtk_list_store_new (1, G_TYPE_STRING);

   m_treescrollFile.attach(gtk_scrolled_window_new(NULL, NULL));

   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(m_treescrollFile.w()), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   m_treeFile.attach(gtk_tree_view_new_with_model (GTK_TREE_MODEL (m_storeFile)));

   renderer = gtk_cell_renderer_text_new();

   column = gtk_tree_view_column_new_with_attributes ("File", renderer, "text", 0, NULL);

   gtk_tree_view_column_set_min_width(column, 150);

   gtk_tree_view_column_set_expand (column, false);

   gtk_tree_view_append_column (GTK_TREE_VIEW (m_treeFile.w()), column);

   pselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_treeFile.w()));

   gtk_tree_selection_set_mode(pselection, GTK_SELECTION_SINGLE);

   g_signal_connect (G_OBJECT (pselection), "changed", G_CALLBACK (file_selection_changed_cb), (gpointer)this);

   gtk_container_add(GTK_CONTAINER(m_treescrollFile.w()), m_treeFile.w());

   gtk_box_pack_start (GTK_BOX (m_hboxBrowse.w()), m_treescrollFile.w(), true, true, 0);

   m_hboxBrowse.set_margin(0, 4, 0, 0);


   m_labelFile.attach(gtk_label_new(""));

   m_labelFile.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_main.box(), m_labelFile, false, false, 0);



   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box(), true);




   gtk_label_set_label(m_pmain->m_labelTopRight.l(), "Choose Existing File on Device");

   m_pmain->m_labelTopRight.set_font(m_pmain->m_fTitle2);

   browse_folder("/");

}




void screen4::browse_folder(string str)
{

   g_object_unref(m_storeFolder);

   GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);

   ::file::path pathMedia;

   #ifdef RASPBIAN

   pathMedia = "/media/pi";

   #else
   //if(bUbuntu)
   //{

   string strMedia = Context.dir().home();

   if(::str::begins_eat_ci(strMedia, "/home/"))
   {

      strMedia = "/media/" + strMedia;

   }

   pathMedia = strMedia;

   //}

   #endif

   m_pathaMedia.remove_all();

   ::dir::ls_dir(m_pathaMedia, pathMedia);

   m_pathaMedia.sort_ci();

   m_pathaRootExpanded.remove_all();

   m_pathaRootExpanded.append(m_pathaRoot);

   m_pathaRootExpanded.append(m_pathaMedia);

   m_patha.remove_all();

   bool bRoot;

   if(str.is_empty() || str == "/"
    ||
    // str does not begin with any root path
    !m_pathaRootExpanded.pred_contains([=](auto & path)
    {
      return ::str::begins_ci(str, path);
    })
    )
   {

      m_patha = m_pathaRootExpanded;

      bRoot = true;

   }
   else
   {

      ::dir::ls_dir(m_patha, str);

      GtkTreeIter iter;

      gtk_list_store_append (store, &iter);  /* Acquire an iterator */

      gtk_list_store_set(store, &iter, 0,  "..", -1);

      bRoot = false;

      m_patha.sort_ci();

   }

   for(auto path : m_patha)
   {

      GtkTreeIter iter;

      gtk_list_store_append (store, &iter);  /* Acquire an iterator */

      gtk_list_store_set(store, &iter, 0,  path.name(), -1);

   }


   bool bListBlockDevices = false;

   if(bListBlockDevices)
   {

      ::file::patha pathaBlock;

      ::file::patha patha;

      ::dir::ls(patha, "/dev");

      for(int c = 'a'; c <= 'z'; c++)
      {

         char blk_name[20];

         sprintf(blk_name, "/dev/sd%c", c);

         blkid_probe pr = blkid_new_probe_from_filename(blk_name);

         if (!pr)
         {

            output_debug_string("device probe failed");

            continue;

         }

         printf("Block path: %s\n", blk_name);

         blkid_partlist ls;

         int nparts, i;

         ls = blkid_probe_get_partitions(pr);

         nparts = blkid_partlist_numof_partitions(ls);

         printf("Number of partitions:%d\n", nparts);

         if (nparts <= 0)
         {

            printf("Please enter correct device name! e.g. \"/dev/sdc\"\n");

            return;

         }

         const char * uuid;

         const char * label;

         const char * type;

         for (i = 0; i < nparts; i++)
         {

            char dev_name[20];

            sprintf(dev_name, "%s%d", blk_name, (i+1));

            printf("Device name: %s", dev_name);

            pr = blkid_new_probe_from_filename(dev_name);

            int result = blkid_do_probe(pr);

            if(result <= 0)
            {

               printf(" Failed\n");

            }
            else
            {

               printf(" OK\n");

               blkid_probe_lookup_value(pr, "UUID", &uuid, NULL);

               blkid_probe_lookup_value(pr, "LABEL", &label, NULL);

               blkid_probe_lookup_value(pr, "TYPE", &type, NULL);

               char text[1024];

               sprintf(text, "%s,%s,%s,%s", dev_name, uuid, label, type);

               printf("Name=%s, UUID=%s, LABEL=%s, TYPE=%s\n", dev_name, uuid, label, type);

            }

            blkid_free_probe(pr);

         }

      }

   }

   GtkTreeSelection * pselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_treeFolder.w()));

   gtk_tree_selection_unselect_all(pselection);

   gtk_tree_view_set_model(GTK_TREE_VIEW(m_treeFolder.w()), GTK_TREE_MODEL(store));

   m_storeFolder = store;

   if(bRoot)
   {

      m_pathFolder = "/";

   }
   else
   {

      m_pathFolder = str;

   }

   m_labelFile.set_window_text("Folder: " + m_pathFolder);

   m_labelFile.set_align_left();

   m_labelFile.set_margin(0, 4, 4, 0);

   list_file(m_pathFolder);

}

void screen4::list_file(string str)
{

   g_object_unref(m_storeFile);

   GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);

   m_patha.remove_all();

   if(str.is_empty() || str == "/"
    ||
    // str does not begin with any root path
    !m_pathaRootExpanded.pred_contains([=](auto & path)
    {
      return ::str::begins_ci(str, path);
    })
    )
   {

   }
   else
   {

      ::dir::ls_file(m_patha, str);

   }

   m_patha.pred_remove([](auto & item)
   {

      return ::str::begins(item.name(), ".");

   });

   m_patha.sort_ci();

   for(auto path : m_patha)
   {

      GtkTreeIter iter;

      gtk_list_store_append (store, &iter);  /* Acquire an iterator */

      gtk_list_store_set(store, &iter, 0,  path.name(), -1);

   }

   GtkTreeSelection * pselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_treeFile.w()));

   gtk_tree_selection_unselect_all(pselection);

   gtk_tree_view_set_model(GTK_TREE_VIEW(m_treeFile.w()), GTK_TREE_MODEL(store));

   m_storeFile = store;




}


void screen4::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);

}

void screen4::on_hide()
{

   screen_base::on_hide();

}




screen5::screen5(main_window * pmain)
{

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 0));

   m_labelFirmwareVersion.attach(gtk_label_new("Firmware Version: "));

   m_labelFirmwareVersion.set_font(m_pmain->m_fTextBig);

   m_labelFirmwareVersion.set_margin(0, 4, 0, 2);

   m_labelFirmwareVersion.set_align_left();

   gtk_box_pack_start (m_main.box(), m_labelFirmwareVersion, TRUE, TRUE, 0);

   m_label.attach(gtk_label_new("Plug in your USB Update Cable."));

   m_label.set_font(m_pmain->m_fButton);

   m_label.set_margin(10);

   gtk_box_pack_start (m_main.box(), m_label, TRUE, TRUE, 0);

   m_labelVersion.attach(gtk_label_new(""));

   m_labelVersion.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_main.box(), m_labelVersion, TRUE, TRUE, 0);








   m_hboxUpdate.attach(gtk_hbox_new (false, 0));

   m_hboxUpdate.set_margin(0, 4, 0, 4);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_hboxUpdate, false, false, 0);

   m_labelAppUpdate.attach(gtk_label_new(""));

   m_labelAppUpdate.set_font(m_pmain->m_fSmall);

   m_labelAppUpdate.set_align_left();

   m_labelAppUpdate.set_align_bottom();


   m_buttonAppUpdate.attach(gtk_button_new_with_label("Update"));

   m_buttonAppUpdate.set_font(m_pmain->m_fSmall);

   g_signal_connect (G_OBJECT (m_buttonAppUpdate.w()), "clicked", G_CALLBACK (on_update_app), (gpointer) m_pmain);





   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box(), false);

   m_buttonUpdate.attach(gtk_button_new_with_label("Update"));

   m_buttonUpdate.set_font(m_pmain->m_fButton);

   m_buttonUpdate.set_margin(2, 0, 0, 0);

   g_signal_connect (G_OBJECT (m_buttonUpdate.w()), "clicked", G_CALLBACK (screen_update), (gpointer) m_pmain);

   gtk_box_pack_start (m_hboxControl.box(), m_buttonUpdate, true, true, 0);

}


void screen5::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);

   string strFirmwareVersion;

   strFirmwareVersion.Format("Firmware Version: %s", App(m_pmain->get_context_application()).m_strVersion);

   m_labelFirmwareVersion.set_window_text(strFirmwareVersion);


   __pointer(screen1) pscreen1 = m_pmain->m_screenptra[10];

   if(pscreen1->m_bUpdate)
   {

      m_labelAppUpdate.set_window_text(pscreen1->m_strAppUpdateMessage);

      m_labelAppUpdate.show_window(SW_SHOW);

      m_buttonAppUpdate.show_window(SW_SHOW);

      gtk_box_pack_start (GTK_BOX(m_hboxUpdate.w()), m_labelAppUpdate, true, true, 0);
      gtk_box_pack_start (GTK_BOX(m_hboxUpdate.w()), m_buttonAppUpdate, false, false, 0);

   }
   else
   {

      m_labelAppUpdate.show_window(SW_HIDE);

      m_buttonAppUpdate.show_window(SW_HIDE);

      gtk_container_remove(GTK_CONTAINER(m_hboxUpdate.w()), m_labelAppUpdate);
      gtk_container_remove(GTK_CONTAINER(m_hboxUpdate.w()), m_buttonAppUpdate);

   }

}

void screen5::on_hide()
{

   screen_base::on_hide();

}


screen6::screen6(main_window * pmain)
{

   int iMaxPortCount = pmain->m_pmain->get_max_port_count();

   m_straPort.set_size(iMaxPortCount);

   int iColCount = 1;

   if(iMaxPortCount >= 3)
   {

      iColCount = 2;

   }

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 0));

   m_labelFirmwareVersion.attach(gtk_label_new("Firmware Version: "));

   m_labelFirmwareVersion.set_font(m_pmain->m_fTextBig);

   m_labelFirmwareVersion.set_margin(0, 2, 0, 2);

   m_labelFirmwareVersion.set_align_left();

   gtk_box_pack_start (m_main.box(), m_labelFirmwareVersion, TRUE, TRUE, 0);

   m_label.attach(gtk_label_new("Turn off your Q-Rainbow lamp. Press and hold the Enter button and turn the lamp on."));

   m_label.set_multiline();

   m_label.set_font(m_pmain->m_fButton);

   m_label.set_margin(0, 2, 0, 2);

   gtk_label_set_xalign(m_label.l(), 0.00);

   gtk_box_pack_start (m_main.box(), m_label, true, true, 0);

   if(iColCount >= 2)
   {

      m_hspan.attach(gtk_hbox_new (true, 4));

   }

   int i = 0;

   for(int iCol = 0; iCol < iColCount; iCol++)
   {

      GtkBox * pbox;

      if(iColCount == 1)
      {

         pbox = m_main.box();

      }
      else
      {

         pbox = GTK_BOX(gtk_vbox_new (false, 0));

         m_column[iCol].attach(GTK_WIDGET(pbox));

         if(iCol == 0)
         {
            m_column[iCol].set_margin(0, 0, 2, 0);
         }
         else
         {
           m_column[iCol].set_margin(2, 0, 0, 0);
         }

      }

      int iUBound = (iCol + 1) * 2;

      for(; i < iUBound; i++)
      {

         if(i >= iMaxPortCount)
         {

            break;

         }

//         m_labelProgressStatus[i].attach(gtk_label_new("Device Detected."));

//         gtk_box_pack_start (pbox, m_labelProgressStatus[i], false, false, 0);

         m_hboxProgress[i].attach(gtk_hbox_new (false, 4));

         gtk_box_pack_start (pbox, m_hboxProgress[i], false, false, 0);

         m_progress[i].attach(gtk_progress_bar_new());

         gtk_box_pack_start (m_hboxProgress[i].box(), m_progress[i], true, true, 0);

         gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(m_progress[i].w()),  0.0);

         gtk_widget_set_hexpand(m_progress[i], true);

         m_labelProgress[i].attach(gtk_label_new("0%"));

         gtk_label_set_yalign(m_labelProgress[i].l(), 0.5);

         m_labelProgress[i].set_font(m_pmain->m_fText);

         gtk_box_pack_start (m_hboxProgress[i].box(), m_labelProgress[i], false, false, 0);

         m_hboxError[i].attach(gtk_hbox_new (false, 0));

         gtk_box_pack_start (pbox, m_hboxError[i], false, false, 0);

         m_labelError[i].attach(gtk_label_new(""));

         m_labelError[i].set_font(m_pmain->m_fText);

         gtk_box_pack_start (m_hboxError[i].box(), m_labelError[i], true, true, 0);

         gtk_widget_set_hexpand(m_labelError[i], true);

//         gtk_label_set_xalign(m_labelProgressStatus[i].l(), 0.9);

  //       m_labelProgressStatus[i].set_font(m_pmain->m_fText);

      }

      if(iColCount >= 2)
      {

         gtk_box_pack_start (m_hspan.box(), m_column[iCol], true, true, 0);

      }

      if(i >= iMaxPortCount)
      {

         break;

      }

   }

   if(iColCount >= 2)
   {

      gtk_box_pack_start (m_main.box(), m_hspan, false, false, 0);

   }

   m_buttonShowLog.attach(gtk_button_new_with_label("Show Log"));

   m_buttonShowLog.set_font(m_pmain->m_fText);

   m_buttonShowLog.set_margin(0, 8, 0, 8);

   gtk_box_pack_start (m_main.box(), m_buttonShowLog, false, false, 0);

   g_signal_connect (G_OBJECT (m_buttonShowLog.w()), "clicked", G_CALLBACK (screen_log), (gpointer) m_pmain);


   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   m_buttonRestartUpdate.attach(gtk_button_new_with_label("Restart Update"));

   m_buttonRestartUpdate.set_font(m_pmain->m_fButton);

   m_buttonRestartUpdate.set_margin(0, 0, 2, 0);

   g_signal_connect (G_OBJECT (m_buttonRestartUpdate.w()), "clicked", G_CALLBACK (restart_update), (gpointer) m_pmain);

   gtk_box_pack_start (m_hboxControl.box(), m_buttonRestartUpdate, true, true, 0);

   create_shutdown_button(m_hboxControl.box(), true);


}


void screen6::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_buttonStartOver, true, true, 0);

   string strFirmwareVersion;

   strFirmwareVersion.Format("Firmware Version: %s", App(m_pmain->get_context_application()).m_strVersion);

   m_labelFirmwareVersion.set_window_text(strFirmwareVersion);


   m_pmain->m_buttonStartOver.show_window(SW_SHOW);

}


void screen6::on_hide()
{

   m_pmain->m_pmain->stop_update();

   screen_base::on_hide();

}


void screen6::add_port(const char * pszPort)
{

   index iFound = -1;

   for(index i = 0; i < m_straPort.get_count(); i++)
   {

      if(m_straPort[i].is_empty())
      {

         iFound = i;

         break;


      }

   }

   if(iFound >= 0)
   {

      m_straPort[iFound] = pszPort;

   }

}

void screen6::remove_port(const char * pszPort)
{

   for(index i = 0; i < m_straPort.get_count(); i++)
   {

      if(m_straPort[i].compare_ci(pszPort) == 0)
      {

         m_straPort[i].Empty();

         break;


      }

   }

}


//screen7::screen7(main_window * pmain)
//{
//
//   m_pmain = pmain;
//
//   m_main.attach(gtk_vbox_new (TRUE, 6));
//
//}
//
//
//void screen7::on_show()
//{
//
//
//   gtk_widget_show_all(m_main);
//
//}
//
//
//
//
//void screen7::on_hide()
//{
//
//   screen_base::on_hide();
//
//}


screen8::screen8(main_window * pmain)
{

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 4));

   m_label.attach(gtk_label_new("Log"));

   gtk_label_set_xalign(m_label.l(), 0.0);

   gtk_box_pack_start (m_main.box(), m_label, false, false, 0);

   m_log.attach(gtk_text_view_new ());

   m_logscroll.attach(gtk_scrolled_window_new(NULL, NULL));

   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(m_logscroll.w()), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   gtk_container_add(GTK_CONTAINER(m_logscroll.w()), m_log.w());

   gtk_box_pack_start (m_main.box(), m_logscroll, true, true, 0);



   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 0));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box(), true);

}

void screen8::on_show()
{

   gtk_widget_show_all(m_main);

   GtkTextBuffer * buffer;

   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (m_log.w()));


   ::file::path path;

#ifdef LINUX
   path = ::dir::home() / ".config/nativeui/log.txt";
#elif defined(MACOS)
   path = ::dir::home() / "update_utility/log.txt";
#else
   path = ::dir::system() / "update_utility/log.txt";
#endif


   gtk_text_buffer_set_text (buffer, file_as_string_dup(path), -1);

}


void screen8::on_hide()
{
   ::gdk_fork([=]()
   {

      screen_base::on_hide();

   });
}


void on_start(GtkWidget *wid, main_window * pmain)
{

   ::gdk_fork([=]()
   {

      pmain->screen(10);

   });

}


void connect_wifi(GtkWidget *wid, main_window * pmain)
{

   ::gdk_fork([=]()
   {

      if(pmain->m_pmain->m_bInternetConnected)
      {

         pmain->screen(30);

      }
      else
      {


         //::system("sudo systemctl stop wicd");
//::system("sudo systemctl stop wpa_supplicant");
         //                ::system("sudo systemctl restart network-manager");


         pmain->screen(20);

      }

   });

}


void install_version(GtkWidget *wid, main_window * pmain)
{

   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = pmain->m_screenptra[10];

      ::file::path path = App(pmain->get_context_application()).m_pathLatestDownloadedBin;

      string strVersion = App(pmain->get_context_application()).m_strLatestDownloadedBinVersion;

      pmain->m_pmain->on_browse(path, strVersion);

   });

}


void screen_download(GtkWidget *wid, main_window * pmain)
{
   ::gdk_fork([=]()
   {

      pmain->screen(30);


   });

}


void screen_browse(GtkWidget *wid, main_window * pmain)
{
   ::gdk_fork([=]()
   {

      pmain->screen(40);


   });

}


void screen_start_update(GtkWidget *wid, main_window * pmain)
{

   ::gdk_fork([=]()
   {

      pmain->screen(50);

   });

}


void screen_log(GtkWidget *wid, main_window * pmain)
{

   ::gdk_fork([=]()
   {

      pmain->screen(80);


   });

}

void screen_update(GtkWidget *wid, main_window * pmain)
{

   ::gdk_fork([=]()
   {

      pmain->screen(60);

      pmain->m_pmain->start_update();

   });

}

void restart_update(GtkWidget *wid, main_window * pmain)
{

   pmain->m_pmain->start_update();

}



void helloWorld (GtkWidget *wid, GtkWidget *win)
{
   ::gdk_fork([=]()
   {

      GtkWidget *dialog = NULL;

      dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Quasar Science Update Utility");

      gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);

      gtk_dialog_run (GTK_DIALOG (dialog));

      gtk_widget_destroy (dialog);


   });

}

void on_update_app(GtkWidget *wid, main_window * pmain)
{
   ::gdk_fork([=]()
   {

      pmain->enable_window(false);

      App(pmain->get_context_application()).update_app();


   });

}



void on_back (GtkWidget *wid, main_window * pmain)
{
   ::gdk_fork([=]()
   {

      pmain->screen(pmain->m_pscreen->m_iPrevious);


   });
}



void main_dialog_quit (GtkWidget *wid, main_window * pmain)
{

   GtkWindow * pwndParent = pmain->win();

   ::gdk_fork([=]()
   {

      GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

      GtkWidget * dialog = gtk_message_dialog_new (pwndParent,
                           flags,
                           GTK_MESSAGE_ERROR,
                           GTK_BUTTONS_YES_NO,
                           "Are you sure you want to shutdown device?");
      gint result = gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);

      if(result == GTK_RESPONSE_YES)
      {

         gtk_widget_hide(pmain->w());

         ::system("sudo shutdown -h now");

      }


   });
}


void main_interface::quit_system()
{
   ::gdk_fork([=]()
   {

      m_pmainwindow->show_window(SW_HIDE);

      gtk_main_quit();


   });
}

void restart_at_desktop(GtkWidget *wid, main_window * pmain)
{

   GtkWindow * pwndParent = pmain->win();

   ::gdk_fork([=]()
   {

      GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

      GtkWidget * dialog = gtk_message_dialog_new (pwndParent,
                           flags,
                           GTK_MESSAGE_ERROR,
                           GTK_BUTTONS_YES_NO,
                           "Confirm 'Back to Desktop'?");
      gint result = gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);

      if(result == GTK_RESPONSE_YES)
      {

         gtk_widget_hide(pmain->w());
         system("cp /home/pi/.config/lxsession/LXDE-pi/autostart.original /home/pi/.config/lxsession/LXDE-pi/autostart");

         ::system("sudo reboot");

      }


   });
}

void browse_for_file(GtkWidget *wid, main_window * pmain)
{
   ::gdk_fork([=]()
   {

      //GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
      GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;

      ::os::window dlg;

      dlg.attach(gtk_file_chooser_dialog_new ("Open File", pmain->win(), action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL));

      //gtk_window_set_decorated(dlg.win(), false);

      GdkGeometry geom;

      geom.base_width = 480;
      geom.base_height = 320;

      geom.max_width = 480;
      geom.max_height = 320;

      geom.min_width = 480;
      geom.min_height = 320;

      geom.win_gravity = GDK_GRAVITY_CENTER;

      gtk_window_set_geometry_hints(dlg.win(), NULL, &geom,   (GdkWindowHints) (GDK_HINT_MIN_SIZE  | GDK_HINT_MAX_SIZE |  GDK_HINT_BASE_SIZE  | GDK_HINT_WIN_GRAVITY));


      gtk_file_chooser_set_use_preview_label(GTK_FILE_CHOOSER(dlg.w()), false);
      gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dlg.w()), false);
      gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dlg.w()), true);

      gint res = gtk_dialog_run (GTK_DIALOG (dlg.w()));



      if (res == GTK_RESPONSE_ACCEPT)
      {

         char * filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dlg.w()));

         pmain->open_file (filename);

         g_free (filename);

      }

      gtk_widget_destroy (dlg.w());

   });
}






void main_window::do_connect_wifi()
{

   ::gdk_fork([=]()
   {

      __pointer(screen21) pscreen21 = m_screenptra[21];

      pscreen21->do_connect_wifi();

   });

}



void main_interface::on_disable_install_downloaded_bin()
{

   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = m_pmainwindow->m_screenptra[10];

      pscreen1->m_buttonInstallVersion.enable_window(false);

   });

}

void main_interface::on_enable_install_downloaded_bin(const char * pszTitle)
{

   string strTitle(pszTitle);

   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = m_pmainwindow->m_screenptra[10];

      pscreen1->m_buttonInstallVersion.set_window_text("Install Version " + strTitle);
      pscreen1->m_buttonInstallVersion.enable_window(true);

   });

}



void main_interface::on_radio_latest_response(bool bUpdateEnable)
{

//   m_pmainwindow->m_update.enable_window(bUpdateEnable);

}


BEGIN_EXTERN_C


gboolean linux_main_window(gpointer pdata)
{

   ::aura::application * papp = (::aura::application *) pdata;

   App(papp).m_pmainwindow = new class main_window(&App(papp));

   App(papp).m_pmainwindow->m_pmain = &App(papp);

   App(papp).m_pmainwindow->show();

   //set_finish();

   //System.set_finish();

   return FALSE;

}


END_EXTERN_C

