#include "framework.h"

#include <gtk/gtk.h>
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

   m_buttonBack.set_margin(m_pmain->m_rectWidgetMargin);

   g_signal_connect (G_OBJECT (m_buttonBack.w()), "clicked", G_CALLBACK (on_back), (gpointer) m_pmain);

   gtk_box_pack_start (pbox, m_buttonBack, true, true, 0);

}


void screen_base::create_shutdown_button(GtkBox * pbox)
{

#if RESTART_AT_DESKTOP

   if(file_exists(::dir::home()/".quasar_science_desktop"))
   {

      m_buttonRestartAtDesktop.attach(gtk_button_new_with_label("Back to Desktop"));

      m_buttonRestartAtDesktop.set_font(m_pmain->m_fSmall);

      g_signal_connect (G_OBJECT (m_buttonRestartAtDesktop.w()), "clicked", G_CALLBACK (restart_at_desktop), (gpointer) m_pmain);

      gtk_box_pack_start (pbox, m_buttonRestartAtDesktop, false, false, 0);

   }

#endif


   m_buttonShutdown.attach(gtk_button_new_with_label("Shutdown"));

   m_buttonShutdown.set_font(m_pmain->m_fButton);

   m_buttonShutdown.set_margin(m_pmain->m_rectWidgetMargin);

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
   ::os::window         m_labelProgressStatus;

   ::os::window         m_hboxProgress;
   ::os::window         m_progress;
   ::os::window         m_labelProgress;

   ::os::window         m_hboxError;
   ::os::window         m_labelError;
   ::os::window         m_buttonShowLog;

   ::os::window         m_hboxControl;
   ::os::window         m_buttonRestartUpdate;

   screen6(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;
};


class screen7 : virtual public screen_base
{
public:


   ::os::window         m_label;
   ::os::window         m_labelProgressStatus;

   ::os::window         m_hboxProgress;
   ::os::window         m_progress;
   ::os::window         m_labelProgress;

   ::os::window         m_hboxError;
   ::os::window         m_labelError;
   ::os::window         m_labelShowLog;

   ::os::window         m_hboxControl;
   ::os::window         m_buttonRestartUpdate;

   screen7(main_window * pmain);

   virtual void on_show() override;

   virtual void on_hide() override;
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

   //gdk_lock l;
   ::gdk_fork([=]()
   {

      __pointer(screen3) pscreen3 = m_pmainwindow->m_screenptra[30];

      pscreen3->m_labelVersion.set_window_text(pszStatus);

      if (bEnable)
      {

         pscreen3->m_buttonDownload.enable_window(true);

         pscreen3->m_labelDownload.enable_window(true);

      }

   });

}


void main_interface::on_app_installation_response(const char * pszStatus)
{

   //gdk_lock l;
   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = m_pmainwindow->m_screenptra[10];

      pscreen1->m_labelVersion.set_window_text(pszStatus);

      __pointer(screen5) pscreen5 = m_pmainwindow->m_screenptra.element_at_grow(50);

      if(pscreen5.is_set())
      {

         pscreen5->m_labelVersion.set_window_text(pszStatus);

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

   //gdk_lock l;

   ::gdk_fork([=]()
   {

      __pointer(screen3) pscreen3 = m_pmainwindow->m_screenptra[30];

      pscreen3->m_labelDownload.set_window_text(pszStatus);

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


void main_interface::set_progress_status(const char * psz)
{

   //gdk_lock l;

   ::gdk_fork([=]()
   {

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      if(m_bError)
      {

         gtk_label_set_markup (pscreen6->m_labelProgressStatus.l(), string("<span foreground=\"red\" >") + psz + string("</span>"));

      }
      else
      {

         pscreen6->m_labelProgressStatus.set_window_text(psz);

      }

   });

}


void main_interface::set_result(const char * psz)
{

   //gdk_lock l;
   ::gdk_fork([=]()
   {

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      if(m_bError)
      {

         gtk_label_set_markup (pscreen6->m_labelError.l(), string("<span foreground=\"red\" >") + psz + string("</span>"));

      }
      else
      {

         pscreen6->m_labelError.set_window_text(psz);

      }

   });

}


void main_interface::set_progress(int iPercent, const char * pszStatus)
{


   //gdk_lock l;
   ::gdk_fork([=]()
   {

      __pointer(screen6) pscreen6 = m_pmainwindow->m_screenptra[60];

      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pscreen6->m_progress.w()), (double) iPercent / 100.0);

      if(iPercent == 0)
      {

         pscreen6->m_labelProgress.set_window_text("");

      }
      else
      {

         pscreen6->m_labelProgress.set_window_text(pszStatus);

      }


   });

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

   m_rectWidgetMargin.left = 10;
   m_rectWidgetMargin.top = 10;
   m_rectWidgetMargin.right = 10;
   m_rectWidgetMargin.bottom = 10;

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

/*   GdkGeometry geom;

   geom.base_width = 480;
   geom.base_height = 320;

   geom.max_width = 480;
   geom.max_height = 320;

   geom.min_width = 480;
   geom.min_height = 320;

   geom.win_gravity = GDK_GRAVITY_CENTER;

   gtk_window_set_geometry_hints(win(), NULL, &geom,   (GdkWindowHints) (GDK_HINT_MIN_SIZE  | GDK_HINT_MAX_SIZE |  GDK_HINT_BASE_SIZE  | GDK_HINT_WIN_GRAVITY));
*/
}


void main_window::show ()
{

   GApplication * papp = g_application_get_default ();

   m_pwidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);

   gtk_application_add_window(GTK_APPLICATION(papp), GTK_WINDOW(m_pwidget));

   gtk_widget_realize (m_pwidget);

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

   m_buttonStartOver.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonStartOver.w()), "clicked", G_CALLBACK (on_start), (gpointer) this);

   screen(10);

   gtk_widget_show_all (w());

   //gtk_window_set_position (win(), GTK_WIN_POS_CENTER_ALWAYS);

   gtk_window_fullscreen(win());



   //gtk_main();

//  output_debug_string("exit");

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
      case 70:
      {

         pscreen = new screen7(this);

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

   m_label.set_margin(10);

   m_label.set_multiline();

   m_label.set_align_top_left();

   gtk_box_pack_start (GTK_BOX(m_vboxL.w()), m_label, true, true, 0);




   m_hboxUpdate.attach(gtk_hbox_new (false, 6));

   m_hboxUpdate.set_margin(10);

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



   m_vboxR.attach(gtk_vbox_new (TRUE, 6));

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_vboxR, TRUE, TRUE, 0);

   m_buttonDownload.attach(gtk_button_new_with_label("Download Latest Version From Web"));

   ::os::window w;

   w.attach(gtk_bin_get_child(GTK_BIN(m_buttonDownload.w())));

   w.set_multiline();

   m_buttonDownload.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonDownload.w()), "clicked", G_CALLBACK (connect_wifi), (gpointer) m_pmain);

   gtk_box_pack_start (GTK_BOX (m_vboxR.w()), m_buttonDownload, TRUE, TRUE, 0);

   m_buttonDownload.set_font(m_pmain->m_fButton);



   m_buttonInstallVersion.attach(gtk_button_new_with_label("Install Version"));

   w.attach(gtk_bin_get_child(GTK_BIN(m_buttonInstallVersion.w())));

   w.set_multiline();

   m_buttonInstallVersion.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonInstallVersion.w()), "clicked", G_CALLBACK (install_version), (gpointer) m_pmain);

   gtk_box_pack_start (GTK_BOX (m_vboxR.w()), m_buttonInstallVersion, TRUE, TRUE, 0);

   m_buttonInstallVersion.set_font(m_pmain->m_fButton);





   m_buttonBrowse.attach(gtk_button_new_with_label ("Choose Existing File on Device"));


   ::os::window wBrowse;

   wBrowse.attach(gtk_bin_get_child(GTK_BIN(m_buttonBrowse.w())));

   wBrowse.set_multiline();

   m_buttonBrowse.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonBrowse.w()), "clicked", G_CALLBACK (screen_browse), (gpointer) m_pmain);

   gtk_box_pack_start (GTK_BOX (m_vboxR.w()), m_buttonBrowse, TRUE, TRUE, 0);

   m_buttonBrowse.set_font(m_pmain->m_fButton);

}


void screen1::on_show()
{

   if(m_bFirstShow)
   {

      m_bFirstShow = false;

      Application.app_check_version();

   }

   Application.check_downloaded_bin();

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

   m_main.attach(gtk_vbox_new (false, 6));

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

   m_tree.set_margin(10);


   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box());

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

   Sleep(400);

   pscreen21->m_password.set_focus();


}


screen21::screen21(screen2 * pscreen2) :
   object(pscreen2->get_context_application()),
   m_pscreen2(pscreen2)
{

   m_pmain = pscreen2->m_pmain;

   m_main.attach(gtk_vbox_new (false, 6));

   m_label.attach(gtk_label_new(""));

   m_label.set_font(m_pmain->m_fTitle2Bold);

   m_label.set_margin(10);

   gtk_label_set_xalign(m_label.l(), 0.0);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_label, true, true, 0);


   m_labelPassword.attach(gtk_label_new("Wi-fi Network Passphrase"));

   m_labelPassword.set_font(m_pmain->m_fTitle2);

   m_labelPassword.set_margin(10);

   gtk_label_set_xalign(m_labelPassword.l(), 0.00);


   gtk_box_pack_start (GTK_BOX (m_main.w()), m_labelPassword, false, false, 0);

   m_password.attach(gtk_entry_new());

   //gtk_entry_set_visibility(GTK_ENTRY(m_password.w()), false);

   //gtk_entry_set_input_purpose(GTK_ENTRY(m_password.w()), GTK_INPUT_PURPOSE_PASSWORD);

   m_password.set_margin(10);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_password, false, false, 0);





   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box());

   m_connect.attach(gtk_button_new_with_label("Connect"));

   m_connect.set_margin(10);

   gtk_box_pack_start (GTK_BOX (m_hboxControl.w()), m_connect, true, true, 0);


   g_signal_connect (G_OBJECT (m_connect.w()), "clicked", G_CALLBACK (on_connect_wifi), (gpointer) m_pmain);


   gtk_label_set_label(m_pmain->m_labelTopRight.l(), "Wi-fi Credentials");

   m_pmain->m_labelTopRight.set_font(m_pmain->m_fTitle2);

   m_pmain->m_labelTopRight.set_multiline();






}





void screen21::on_show()
{

   ::system("matchbox-keyboard &");

   Sleep(400);

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

   m_label.set_window_text(strSSID);

   m_pscreen2->m_strSSID = strSSID;

   m_pscreen2->connect_wifi(true);

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

   m_main.attach(gtk_vbox_new (false, 6));



   m_hboxVersion.attach(gtk_hbox_new(TRUE, 6));

   m_buttonCheckVersion.attach(gtk_button_new_with_label("Check Version"));

   m_buttonCheckVersion.set_font(m_pmain->m_fButton);

   m_buttonCheckVersion.set_margin(10);


   gtk_box_pack_start (m_hboxVersion.box(), m_buttonCheckVersion, TRUE, TRUE, 0);

   g_signal_connect (G_OBJECT (m_buttonCheckVersion.w()), "clicked", G_CALLBACK (on_check_version), (gpointer) m_pmain);

   m_labelVersion.attach(gtk_label_new(""));

   m_labelVersion.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_hboxVersion.box(), m_labelVersion, TRUE, TRUE, 0);

   gtk_box_pack_start (m_main.box(), m_hboxVersion, TRUE, TRUE, 0);



   m_hboxDownload.attach((gtk_hbox_new(TRUE, 6)));

   m_buttonDownload.attach(gtk_button_new_with_label("Download"));

   m_buttonDownload.set_font(m_pmain->m_fButton);

   m_buttonDownload.enable_window(false);

   m_buttonDownload.set_margin(10);

   gtk_box_pack_start (m_hboxDownload.box(), m_buttonDownload, TRUE, TRUE, 0);

   g_signal_connect (G_OBJECT (m_buttonDownload.w()), "clicked", G_CALLBACK (on_download), (gpointer) m_pmain);

   m_labelDownload.attach(gtk_label_new("0%"));

   m_labelDownload.set_font(m_pmain->m_fText);

   m_labelDownload.enable_window(false);

   gtk_box_pack_start (m_hboxDownload.box(), m_labelDownload, TRUE, TRUE, 0);

   gtk_box_pack_start (m_main.box(), m_hboxDownload, TRUE, TRUE, 0);


   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box());

   m_buttonNext.attach(gtk_button_new_with_label("Next"));

   m_buttonNext.set_font(m_pmain->m_fButton);


   m_buttonNext.enable_window(false);

   m_buttonNext.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonNext.w()), "clicked", G_CALLBACK (screen_start_update), (gpointer) m_pmain);

   gtk_box_pack_start (m_hboxControl.box(), m_buttonNext, true, true, 0);

   gtk_label_set_label(m_pmain->m_labelTopRight.l(), "Download Latest Version From Web");

   m_pmain->m_labelTopRight.set_font(m_pmain->m_fTitle2);
   m_pmain->m_labelTopRight.set_multiline();


}


void screen3::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);

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

               if(strFolder == "..")
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


screen4::screen4(main_window * pmain)
{

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 6));

   m_storeFolder = gtk_list_store_new (1, G_TYPE_STRING);

   m_hboxBrowse.attach(gtk_hbox_new (true, 3));

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

   m_hboxBrowse.set_margin(10);


   m_labelFile.attach(gtk_label_new(""));

   m_labelFile.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_main.box(), m_labelFile, false, false, 0);



   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box());




   gtk_label_set_label(m_pmain->m_labelTopRight.l(), "Choose Existing File on Device");

   m_pmain->m_labelTopRight.set_font(m_pmain->m_fTitle2);

   browse_folder(::dir::home() / "Downloads");

}




void screen4::browse_folder(string str)
{

   g_object_unref(m_storeFolder);

   GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);

   m_patha.remove_all();

   ::file::patha patha1;

#ifdef RASPBIAN

   ::dir::ls_dir(patha1, "/media/pi");

#endif

   ::file::patha patha2;

   ::dir::ls_dir(patha2, str);

   m_patha = patha1;

   m_patha.append(patha2);

   m_patha.pred_remove([](auto & item)
   {

      return ::str::begins(item.name(), ".");

   });

   m_patha.sort_ci();

   if(str.has_char() && str != "/")
   {

      GtkTreeIter iter;

      gtk_list_store_append (store, &iter);  /* Acquire an iterator */

      gtk_list_store_set(store, &iter, 0,  "..", -1);

   }

   for(auto path : m_patha)
   {

      GtkTreeIter iter;

      gtk_list_store_append (store, &iter);  /* Acquire an iterator */

      gtk_list_store_set(store, &iter, 0,  path.name(), -1);

   }

   GtkTreeSelection * pselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(m_treeFolder.w()));

   gtk_tree_selection_unselect_all(pselection);

   gtk_tree_view_set_model(GTK_TREE_VIEW(m_treeFolder.w()), GTK_TREE_MODEL(store));

   m_storeFolder = store;

   if(str.is_empty())
   {

      m_pathFolder = "/";

   }
   else
   {

      m_pathFolder = str;

   }

   m_labelFile.set_window_text("Folder: " + m_pathFolder);

   m_labelFile.set_align_left();

   m_labelFile.set_margin_left(10);

   list_file(m_pathFolder);


}

void screen4::list_file(string str)
{

   g_object_unref(m_storeFile);

   GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);

   m_patha.remove_all();

   ::dir::ls_file(m_patha, str);

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

   m_main.attach(gtk_vbox_new (false, 6));

   m_label.attach(gtk_label_new("Plug in your USB Update Cable."));

   m_label.set_font(m_pmain->m_fButton);

   m_label.set_margin(10);

   gtk_box_pack_start (m_main.box(), m_label, TRUE, TRUE, 0);

   m_labelVersion.attach(gtk_label_new(""));

   m_labelVersion.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_main.box(), m_labelVersion, TRUE, TRUE, 0);








   m_hboxUpdate.attach(gtk_hbox_new (false, 6));

   m_hboxUpdate.set_margin(10);

   gtk_box_pack_start (GTK_BOX (m_main.w()), m_hboxUpdate, false, false, 0);

   m_labelAppUpdate.attach(gtk_label_new(""));

   m_labelAppUpdate.set_font(m_pmain->m_fSmall);

   m_labelAppUpdate.set_align_left();

   m_labelAppUpdate.set_align_bottom();


   m_buttonAppUpdate.attach(gtk_button_new_with_label("Update"));

   m_buttonAppUpdate.set_font(m_pmain->m_fSmall);

   g_signal_connect (G_OBJECT (m_buttonAppUpdate.w()), "clicked", G_CALLBACK (on_update_app), (gpointer) m_pmain);





   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box());

   m_buttonUpdate.attach(gtk_button_new_with_label("Update"));

   m_buttonUpdate.set_font(m_pmain->m_fButton);

   m_buttonUpdate.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonUpdate.w()), "clicked", G_CALLBACK (screen_update), (gpointer) m_pmain);

   gtk_box_pack_start (m_hboxControl.box(), m_buttonUpdate, true, true, 0);

}


void screen5::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_labelTopRight, true, true, 0);

   m_pmain->m_labelTopRight.show_window(SW_SHOW);


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

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 0));

   m_label.attach(gtk_label_new("Turn off your Q-Rainbow lamp. Press and hold the Enter button and turn the lamp on."));

   m_label.set_multiline();

   m_label.set_font(m_pmain->m_fButton);

   m_label.set_margin(10);

   gtk_box_pack_start (m_main.box(), m_label, true, true, 0);


   m_labelProgressStatus.attach(gtk_label_new("Device Detected."));

   gtk_label_set_xalign(m_labelProgressStatus.l(), 0.9);

   m_labelProgressStatus.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_main.box(), m_labelProgressStatus, false, false, 0);



   m_hboxProgress.attach(gtk_hbox_new (false, 6));

   gtk_box_pack_start (m_main.box(), m_hboxProgress, false, false, 0);

   m_progress.attach(gtk_progress_bar_new());

   gtk_box_pack_start (m_hboxProgress.box(), m_progress, true, true, 0);

   gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(m_progress.w()),  0.0);

   gtk_widget_set_hexpand(m_progress, true);

   m_labelProgress.attach(gtk_label_new("0%"));

   gtk_label_set_yalign(m_labelProgress.l(), 0.5);

   m_labelProgress.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_hboxProgress.box(), m_labelProgress, false, false, 0);




   m_hboxError.attach(gtk_hbox_new (false, 6));

   gtk_box_pack_start (m_main.box(), m_hboxError, false, false, 0);

   m_labelError.attach(gtk_label_new(""));

   m_labelError.set_font(m_pmain->m_fText);

   gtk_box_pack_start (m_hboxError.box(), m_labelError, true, true, 0);

   gtk_widget_set_hexpand(m_labelError, true);

   m_buttonShowLog.attach(gtk_button_new_with_label("Show Log"));

   m_buttonShowLog.set_font(m_pmain->m_fButton);

   m_buttonShowLog.set_margin(10);

   gtk_box_pack_start (m_hboxError.box(), m_buttonShowLog, false, false, 0);

   g_signal_connect (G_OBJECT (m_buttonShowLog.w()), "clicked", G_CALLBACK (screen_log), (gpointer) m_pmain);


   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   m_buttonRestartUpdate.attach(gtk_button_new_with_label("Restart Update"));

   m_buttonRestartUpdate.set_font(m_pmain->m_fButton);

   m_buttonRestartUpdate.set_margin(10);

   g_signal_connect (G_OBJECT (m_buttonRestartUpdate.w()), "clicked", G_CALLBACK (screen_update), (gpointer) m_pmain);

   gtk_box_pack_start (m_hboxControl.box(), m_buttonRestartUpdate, true, true, 0);

   create_shutdown_button(m_hboxControl.box());


}


void screen6::on_show()
{

   screen_base::on_show();

   gtk_widget_show_all(m_main);

   gtk_box_pack_start (m_pmain->m_hboxTop.box(), m_pmain->m_buttonStartOver, true, true, 0);

   m_pmain->m_buttonStartOver.show_window(SW_SHOW);

}


void screen6::on_hide()
{

   screen_base::on_hide();

}


screen7::screen7(main_window * pmain)
{

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (TRUE, 6));

}


void screen7::on_show()
{


   gtk_widget_show_all(m_main);

}


screen8::screen8(main_window * pmain)
{

   m_pmain = pmain;

   m_main.attach(gtk_vbox_new (false, 6));

   m_label.attach(gtk_label_new("Log"));

   gtk_label_set_xalign(m_label.l(), 0.0);

   gtk_box_pack_start (m_main.box(), m_label, false, false, 0);

   m_log.attach(gtk_text_view_new ());

   m_logscroll.attach(gtk_scrolled_window_new(NULL, NULL));

   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(m_logscroll.w()), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   gtk_container_add(GTK_CONTAINER(m_logscroll.w()), m_log.w());

   gtk_box_pack_start (m_main.box(), m_logscroll, true, true, 0);



   m_hboxControl.attach(gtk_hbox_new (!file_exists(::dir::home()/".quasar_science_desktop"), 6));

   gtk_box_pack_start (m_main.box(), m_hboxControl, false, false, 0);

   create_back_button(m_hboxControl.box());

   create_shutdown_button(m_hboxControl.box());

}


void screen7::on_hide()
{

   screen_base::on_hide();

}



void screen8::on_show()
{

   gtk_widget_show_all(m_main);

   GtkTextBuffer * buffer;

   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (m_log.w()));


   ::file::path path;

#ifdef MACOS
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

         pmain->screen(20);

      }

   });

}

void install_version(GtkWidget *wid, main_window * pmain)
{
   ::gdk_fork([=]()
   {

      __pointer(screen1) pscreen1 = pmain->m_screenptra[10];

      pmain->m_pmain->on_browse(App(pmain->get_context_application()).m_pathLatestDownloadedBin);


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

      pmain->m_pmain->detect();

   });

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
   ::gdk_fork([=]()
   {

      gtk_widget_hide(pmain->w());

//   gtk_main_quit();

      ::system("sudo shutdown -h now");


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
   ::gdk_fork([=]()
   {


      gtk_widget_hide(pmain->w());

      system("cp /home/pi/.config/lxsession/LXDE-pi/autostart.original /home/pi/.config/lxsession/LXDE-pi/autostart");

      ::system("sudo reboot");


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

void main_interface::on_enable_install_downloaded_bin(string strTitle)
{

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

