#pragma once


#ifdef __OBJC__

@class MainWindow;
@class WaitWindow;

#elif defined(LINUX)

class main_window;

#endif

class main_interface
{
public:


   enum e_mode
   {

      mode_none,
      mode_normal,
      mode_beta,
      mode_browse

   };


#ifdef __OBJC__

   MainWindow * m_pmainwindow;
   WaitWindow * m_pwaitwindow;

#elif defined(WINDOWS_DESKTOP)

   ::quasar_science_update_utility::main_dialog * m_pmainwindow;

#elif defined(LINUX)

   ::main_window * m_pmainwindow;

#else

   void * m_pmainwindow; // cast to RoundWindow * to use it
   void * m_pwaitwindow; // cast to RoundWindow * to use it

#endif




   bool     m_bError;
   bool     m_bDetecting;
   bool     m_bMultiPort;
   bool     m_bUpdate;
   bool     m_bUpdating;
   int      m_iUpdating;
   bool     m_bCancel;
   bool		m_bInternetConnected;
   e_mode   m_emode;
   bool     m_bAppUpdateCancellable;
   bool     m_bAppUpdating;
   bool     m_bInstallBootloader;


   main_interface();


   virtual bool cancel_app_update() = 0;

   virtual bool has_normal_version() = 0;

   virtual bool has_beta_version() = 0;

   virtual void check_version() = 0;

   virtual void check_beta_version() = 0;

   virtual void on_check_version_response(const char * pszStatus, bool bEnable);


   virtual void download() = 0;

   virtual void on_download_response(const char * pszStatus, bool bEnable);

   virtual void on_download_bootloader_response(const char * pszStatus, bool bEnable);

   virtual void on_bootloader_check_version_on_show();

   virtual void on_app_installation_response(const char * pszStatus);

   virtual void on_app_update_cancellable(bool bCancellable);

   virtual void wait_dialog_alert(const char * psz);
   virtual void wait_dialog_stop_and_hide_animation();

   virtual void radio_download() = 0;

   virtual void on_radio_download_response(bool bEnableDownload, bool bEnableUpdate);

   virtual void radio_latest() = 0;

   virtual void on_radio_latest_response(bool bEnableUpdate);

   virtual void radio_browse() = 0;

   virtual void on_radio_browse_response(bool bEnableUpdate);

   virtual void on_browse(const char * pszPath, const char * pszVersion = NULL) = 0;

   virtual void on_browse_response(bool bEnableUpdate);

   virtual void start_update(e_mode emode) = 0;
   virtual void stop_update() = 0;

   virtual void update_app(bool bBeta) = 0;
   void on_app_check_version(const char * pszChar);
   void on_app_check_version_beta(const char * pszChar);
   void on_app_check_version_on_show();

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

   virtual int get_total_size(const char * pszPort) = 0;

   virtual void on_disable_install_downloaded_bin();
   virtual void on_enable_install_downloaded_bin(const char * pszTitle);

   virtual void show_log() = 0;

   virtual int get_max_port_count();
   virtual void add_port(const char * pszPort);
   virtual void remove_port(const char * pszPort);


   virtual void download_bootloader() = 0;

};







