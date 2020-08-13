#pragma once


namespace win32
{


   class window :
      virtual public ::object
   {
   public:


#ifdef WINDOWS
      HWND m_hwnd;
#endif


      window();
      virtual ~window();


      void attach(HWND hwnd);

      void destroy_window();

      void set_dlg_item_font(int iItem, font & font);

      void set_dlg_item_text(int iItem, string str);

      void set_font(font & font);

      template < typename WINDOW >
      void get_dlg_item(WINDOW & w, int iItem)
      {

         w.set_app(get_context_application());
         w.attach(::GetDlgItem(m_hwnd, iItem));

      }

      virtual void on_command(UINT nID);

      iptr send_message(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0);
      void post_message(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0);


      void enable_window(bool bEnable = true);

      void enable_children(bool bEnable = true);

      void set_window_text(const char * psz);

      void set_parent(window * pwindow);

      void show_window(int nCmdShow);

      string get_window_text();
      void redraw_window();

      iptr get_combo_sel();
   };


} // namespace win32


