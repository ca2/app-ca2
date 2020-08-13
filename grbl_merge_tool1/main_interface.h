#pragma once


#ifdef __OBJC__

@class MainWindow;

#elif defined(LINUX)

class main_window;

#endif

class main_interface
{
public:


#ifdef __OBJC__

   MainWindow * m_pmainwindow;

#elif defined(WINDOWS_DESKTOP)

   ::grbl_merge_tool::main_dialog * m_pmainwindow;

#elif defined(LINUX)

   ::main_window * m_pmainwindow;

#else

   void * m_pmainwindow; // cast to RoundWindow * to use it

#endif



   bool     m_bError;
   bool     m_bDetecting;
   bool     m_bMultiPort;
   bool     m_bUpdate;
   bool     m_bUpdating;
   int      m_iUpdating;
   bool     m_bCancel;
   bool		m_bInternetConnected;


   main_interface();


   virtual void check_version() = 0;

   virtual void on_check_version_response(const char * pszStatus, bool bEnable);


   virtual void download() = 0;

   virtual void on_download_response(const char * pszStatus, bool bEnable);

   virtual void on_app_installation_response(const char * pszStatus);


   virtual void radio_download() = 0;

   virtual void on_radio_download_response(bool bEnableDownload, bool bEnableUpdate);

   virtual void radio_latest() = 0;

   virtual void on_radio_latest_response(bool bEnableUpdate);

   virtual void radio_browse() = 0;

   virtual void on_radio_browse_response(bool bEnableUpdate);

   virtual void on_browse(const char * pszPath, const char * pszVersion = NULL) = 0;

   virtual void on_browse_response(bool bEnableUpdate);

   virtual void start_update() = 0;
   virtual void stop_update() = 0;

   virtual void update_app() = 0;
   void on_app_check_version(const char * pszChar);
   
   virtual void get_firmware_version_label(char * pszChar, int iSize);

   void quit_system();
   //void set_progress_status(const char * pszPort, const char * pszStatus);
   void set_result(const char * pszPort, const char * pszStatus);
   void set_progress(const char * pszPort, int iPercent);
   void set_progress(const char * pszPort, int iPercent, const char * pszStatus);
   void show_manual_com(const char **pszList, int iCount, int iSel);
   void hide_manual_com();
   void cancel_enable(bool bEnable);
   void update_enable(bool bEnable);
   void redraw_window();
   void to_close();
   void to_cancel();
   void total_progress(const char * pszPort, int iBytes);
   void total_progress(const char * pszPort, int iBytes, int iTotalSize);

   virtual void select_serial_and_update(int iIndex) = 0;

   virtual int get_total_size() = 0;

   virtual void on_disable_install_downloaded_bin();
   virtual void on_enable_install_downloaded_bin(const char * pszTitle);

   virtual void show_log() = 0;

   virtual int get_max_port_count();
   virtual void add_port(const char * pszPort);
   virtual void remove_port(const char * pszPort);

};







