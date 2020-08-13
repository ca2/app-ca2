#pragma once

void show_main_dialog();

namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_HELLOAURA application :
      virtual public ::nativeui::application,
      virtual public main_interface,
      virtual public ::double_scalar_source::listener
   {
   public:


      ::mutex                   m_mutexPickPort;

      __pointera(::serial::Serial)   m_seriala;


      string            m_strDetectedOrSelected;




      ::file::path m_pathBrowse;
      string       m_strBrowseVersion;
      ::file::path m_pathDownload;
      string       m_strDownloadVersion;

      ::file::path m_pathSend;
      memory   m_m;

      int                              m_iCom;
      ::serial::PortInfo               m_portinfoOverride;
      array < ::serial::PortInfo >     m_portinfoaUpdate;

      array < ::serial::PortInfo >     m_portinfoa;

      string                           m_strDownload;
      string                           m_strVersion;
      string                           m_strUrl;

      string                           m_strAppDownload;
      string                           m_strAppVersion;
      string                           m_strAppUrl;
      ::file::path                     m_pathAppNew;

      string                           m_strLatestDownloadedBinVersion;
      ::file::path                     m_pathLatestDownloadedBin;
      u32                         m_dwLastProgress;



      application();
      virtual ~application();

      virtual bool upgrade_cable_driver();
      virtual void list_serial_ports();

      virtual void remove_serial(::serial::PortInfo portinfo, __pointer(::serial::Serial) & pserial);

      virtual void manual_com();

      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(::create * pcreate) override;

      virtual void get_firmware_version_label(char * pszChar, int iSize) override;

      virtual i64 add_ref() override;
      virtual i64 dec_ref() override;

      //void list_serial_port(stringa & stra, stringa & straPath); //added fucntion to find the present serial ports of the system;

      virtual void app_check_version();
      virtual void update_app() override;

      virtual bool download_new_app();
      virtual void install_new_app();


      virtual void check_downloaded_bin();

      virtual void check_version() override;
      virtual void download() override;
      virtual void radio_download() override;
      virtual void radio_latest() override;
      virtual void radio_browse() override;
      virtual void on_browse(const char * pszPath, const char * pszVersion = NULL) override;


      virtual ::serial::PortInfo pick_port();
      virtual void start_update() override;
      virtual void update_port(::serial::PortInfo portinfo);
      virtual void stop_update() override;

      virtual int get_total_size() override;

      virtual void select_serial_and_update(int iIndex) override;


      virtual void list_connected_serial_port(stringa & stra, stringa & straPath); //added fucntion to find the present serial ports of the system

      virtual void show_log() override;

      virtual void on_set_scalar(double_scalar_source * psource, e_scalar escalar, double d, int iFlags) override;


   };


} // namespace grbl_merge_tool




