#pragma once


namespace quasar_science_update_utility
{


   class dialog2 :
      virtual public child_dialog
   {
   public:

      //int m_iCom;
      //::os::window      m_checkVersion;
      //::os::window      m_versionStatus;
      ::win32::window      m_firmwareVersion;
      ::win32::window      m_progressControl;
      ::win32::window      m_progressStatus;
      ::win32::window      m_uploadProgress;
      ::win32::window      m_result;
      ::win32::window      m_combo;
      ::win32::window      m_update2;

      dialog2(::aura::application * papp);
      virtual ~dialog2();


      virtual bool on_init_dialog();

      void on_command(UINT nID);
      void on_notify(NMHDR * p);
      INT_PTR on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);

   };


} // namespace quasar_science_update_utility



