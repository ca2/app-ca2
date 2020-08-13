#include "framework.h"


namespace os
{


   window::window()
   {

      m_pwidget = NULL;

   }


   window::~window()
   {


   }


   void window::attach(GtkWidget * pwidget)
   {

      m_pwidget = pwidget;

   }


   void window::destroy_window()
   {

      if (m_pwidget == NULL)
      {

         return;

      }

      //::DestroyWindow(m_pwidget);

      m_pwidget = NULL;

   }


   void window::set_dlg_item_font(int iItem, font & font)
   {

      window window;
//      get_dlg_item(window, iItem);
      window.set_font(font);

   }

void window::set_multiline()
{

   gtk_label_set_line_wrap(l(), true);
   gtk_label_set_single_line_mode(l(), false);
   gtk_label_set_lines(l(), -1);

}

   void window::set_dlg_item_text(int iItem, string str)
   {

      window window;
  //    get_dlg_item(window, iItem);
      window.set_window_text(str);

   }

   void window::set_font(font & font)
   {

      GtkLabel * plabel = NULL;

      if(GTK_IS_BUTTON(m_pwidget))
      {

         plabel = GTK_LABEL(gtk_bin_get_child(GTK_BIN(m_pwidget)));

      }
      else if(GTK_IS_LABEL(m_pwidget))
      {

         plabel = GTK_LABEL(m_pwidget);

      }

      if(plabel != NULL)
      {

         PangoAttribute * pattribute = pango_attr_font_desc_new(font.m_pdescription);

         PangoAttrList * plist = pango_attr_list_new();

         pango_attr_list_insert(plist, pattribute);

         gtk_label_set_attributes(plabel, plist);

         pango_attr_list_unref(plist);

      }

   }


   void window::on_command(UINT nID)
   {

   }

   void window::send_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
//      ::SendMessage(m_hwnd, uiMessage, wparam, lparam);
   }

   void window::post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
//      ::PostMessage(m_hwnd, uiMessage, wparam, lparam);
   }

   void window::enable_window(bool bEnable)
   {

      gtk_widget_set_sensitive(w(), bEnable);

   }


   void window::set_window_text(const char * psz)
   {

      GtkLabel * plabel = NULL;

      if(GTK_IS_BUTTON(m_pwidget))
      {

         plabel = GTK_LABEL(gtk_bin_get_child(GTK_BIN(m_pwidget)));

      }
      else if(GTK_IS_LABEL(m_pwidget))
      {

         plabel = GTK_LABEL(m_pwidget);

      }

      if(plabel != NULL)
      {

         gtk_label_set_text(plabel, psz);

      }

   }


   void window::redraw_window()
   {

//      RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

   }
   string window::get_window_text()
   {

//      strsize s = ::GetWindowTextLengthW(m_hwnd);
//
//      wstring wstr;
//
//      unichar * p = wstr.alloc(s+1);
//
//      GetWindowTextW(m_hwnd, p, s+1);
//
//      wstr.release_buffer();

//      return wstr;

        return "";

   }

   void window::set_parent(window * pwindow)
   {
      if (pwindow == NULL)
      {

//         SetParent(m_hwnd, NULL);
      }
      else
      {

//         SetParent(m_hwnd, pwindow->m_hwnd);

      }


   }

   void window::show_window(int nCmdShow)
   {

      if(nCmdShow == SW_HIDE)
      {

         gtk_widget_hide(m_pwidget);

      }
      else
      {

         gtk_widget_show(m_pwidget);

      }

   }


   void window::set_margin_left(int left)
   {

      gtk_widget_set_margin_left(w(), left);

   }


   void window::set_margin_top(int top)
   {

      gtk_widget_set_margin_top(w(), top);

   }


   void window::set_margin_right(int right)
   {

      gtk_widget_set_margin_end(w(), right);

   }


   void window::set_margin_bottom(int bottom)
   {

      gtk_widget_set_margin_bottom(w(), bottom);

   }


   void window::set_margin(int left, int top, int right, int bottom)
   {

      set_margin_left(left);
      set_margin_top(top);
      set_margin_right(right);
      set_margin_bottom(bottom);

   }


   void window::set_margin(LPCRECT lpcrect)
   {

      set_margin(lpcrect->left, lpcrect->top, lpcrect->right, lpcrect->bottom);

   }

   void window::set_margin(int margin)
   {

      set_margin(margin, margin, margin, margin);

   }

   void window::set_xalign(double x)
   {

      gtk_label_set_xalign(l(), x);

   }

   void window::set_align_left()
   {

      set_xalign(0.0);

   }


   void window::set_align_right()
   {

      set_xalign(1.0);

   }


   void window::set_align_hcenter()
   {

      set_xalign(0.5);

   }


   void window::set_yalign(double y)
   {

      gtk_label_set_yalign(l(), y);

   }


   void window::set_align_top()
   {

      set_yalign(0.0);

   }


   void window::set_align_bottom()
   {

      set_yalign(1.0);

   }


   void window::set_align_vcenter()
   {

      set_yalign(0.5);

   }

   void window::set_align_top_left()
   {

      set_align_left();
      set_align_top();

   }

   void window::set_align_center()
   {

      set_align_hcenter();
      set_align_vcenter();

   }


   void window::set_focus()
   {

      gtk_widget_grab_focus(w());

   }


   void window::add_class(const char * psz)
   {

	   GtkStyleContext * pcontext = gtk_widget_get_style_context(w());

	   gtk_style_context_add_class(pcontext, psz);

   }


} // namespace os


