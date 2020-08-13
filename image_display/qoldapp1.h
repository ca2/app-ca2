#pragma once

void show_main_dialog();

namespace quasar_science_update_utility
{


   class CLASS_DECL_APP_CORE_HELLOAURA application :
      virtual public ::aura::application,
      virtual public main_interface,
      virtual public ::double_scalar_source::listener
   {
   public:

      static ::serial::Serial * m_pserial;
      string            m_strDetectedOrSelected;




      ::file::path m_pathBrowse;
      ::file::path m_pathDownload;

      ::file::path m_pathSend;
      memory   m_m;

      int                              m_iCom;

      ::serial::PortInfo               m_portinfo;

      array < ::serial::PortInfo >     m_portinfoa;

      string                           m_strDownload;
      string                           m_strVersion;
      string                           m_strUrl;

      string                           m_strAppDownload;
      string                           m_strAppVersion;
      string                           m_strAppUrl;
      ::file::path                     m_pathAppNew;

      ::file::path                     m_pathLatestDownloadedBin;



      application();
      virtual ~application();


      virtual void list_serial_ports();

      virtual void manual_com();

      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(::create * pcreate) override;

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
      virtual void on_browse(const char * pszPath) override;


      virtual void detect() override;
      virtual void fork_update() override;
      virtual void update() override;

      virtual int get_total_size() override;

      virtual void select_serial_and_update(int iIndex) override;


      virtual void list_connected_serial_port(stringa & stra, stringa & straPath); //added fucntion to find the present serial ports of the system

      virtual void show_log() override;

      virtual void on_set_scalar(double_scalar_source * psource, e_scalar escalar, double d, int iFlags) override;


   };


} // namespace quasar_science_update_utility



