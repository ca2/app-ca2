#pragma once


#include "linux_os_font.h"


namespace os
{


   class window :
      virtual public ::object
   {
   public:


#ifdef WINDOWS
      HWND m_hwnd;
#else
      GtkWidget * m_pwidget;
#endif


      window();
      virtual ~window();


      void attach(GtkWidget * hwnd);

      void destroy_window();

      void set_dlg_item_font(int iItem, font & font);

      void set_dlg_item_text(int iItem, string str);

      void set_font(font & font);

      void set_multiline();

      void add_class(const char * psz);

      void set_focus();

      operator GtkWidget * () { return m_pwidget;}
      GtkWindow * win() { return GTK_WINDOW(m_pwidget);}
      GtkWidget * w() { return m_pwidget;}
      GtkLabel * l() { return GTK_LABEL(m_pwidget);}
      GtkButton * b() { return GTK_BUTTON(m_pwidget);}
      GtkBox * box() { return GTK_BOX(m_pwidget);}

      virtual void on_command(UINT nID);

      void send_message(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0);
      void post_message(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0);

      void set_margin_left(int left);
      void set_margin_top(int top);
      void set_margin_right(int right);
      void set_margin_bottom(int bottom);
      void set_margin(int left, int top, int right, int bottom);
      void set_margin(LPCRECT lpcrectMargin);
      void set_margin(int margin);

      void set_xalign(double x);
      void set_align_left();
      void set_align_right();
      void set_align_hcenter();

      void set_yalign(double y);
      void set_align_top();
      void set_align_bottom();
      void set_align_vcenter();

      void set_align_top_left();
      void set_align_center();

      void enable_window(bool bEnable = true);

      void set_window_text(const char * psz);

      void set_parent(window * pwindow);

      void show_window(int nCmdShow);

      string get_window_text();
      void redraw_window();
   };


} // namespace os


