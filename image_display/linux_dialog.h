#pragma once


#include "linux_os_dialog.h"


#define RESTART_AT_DESKTOP 1


class main_window;
class main_interface;

class screen_base :
virtual public object
{
public:


   int               m_iPrevious;
   ::os::window      m_buttonBack;
#if RESTART_AT_DESKTOP
   ::os::window      m_buttonRestartAtDesktop;
   ::os::window      m_buttonAdmin;
#endif
   ::os::window      m_buttonShutdown;
   main_window *     m_pmain;
   ::os::window      m_main;
GtkStyle * m_pstyleBottom;

   ::os::window      m_labelFirmwareVersion;

   screen_base()
   {

      m_iPrevious = -1;

   }

   virtual void on_show();
   virtual void on_hide();

   void create_back_button(GtkBox * pbox);
   void create_shutdown_button(GtkBox * pbox, bool bRightMost, bool bAdmin = false);

};

class main_window :
    virtual public ::os::window
{
public:


   main_interface *  m_pmain;

   ::os::window      m_button;
   ::os::window      m_win;
   ::os::window      m_vboxMain;
   ::os::window      m_hboxTop;
   ::os::window      m_hboxTitle;
   ::os::window      m_wLogo;
   ::os::window      m_wTitle;
   ::os::font        m_fTitle;
   ::os::font        m_fTextBig;
   ::os::font        m_fTitle2;
   ::os::font        m_fTitle2Bold;
   ::os::font        m_fText;
   ::os::font        m_fButton;

   ::os::window      m_labelTopRight;
   ::os::window      m_buttonStartOver;

   ::os::font        m_fSmall;


   screen_base *    m_pscreen;

   array < screen_base * >         m_screenptra;

   int              m_iCurrent;

   int               m_iBottomHeightRequest;

   int               m_w;

   int               m_h;


   main_window(::aura::application * papp);


   void reset_size();
   void show();

   void screen(int i);

   void open_file(string str);

   void do_connect_wifi();


};

