#pragma once


namespace quasar_science_update_utility
{


   class main_dialog :
      virtual public ::win32::dialog
   {
   public:

      main_interface * m_pmaininterface;
      ::visual::dib_pointer     m_dib;

      HBRUSH               m_hbr;

      ::win32::font           m_fTitle1;
      ::win32::font           m_fTitle2;

      ::win32::font           m_fText1;

      ::win32::font           m_fText2;
      ::win32::window         m_restart;
      ::win32::window         m_update;
      ::win32::window         m_cancel;

      dialog1                 m_dlg1;
      dialog2                 m_dlg2;

      ::win32::window         m_labelApp;
      ::win32::window         m_updateApp;

      main_interface::e_mode  m_emode;


      main_dialog(::aura::application * papp);
      virtual ~main_dialog();

      virtual bool on_init_dialog();
      virtual void on_close();

      INT_PTR on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);

      void on_command(UINT nID);

   };

} // namespace quasar_science_update_utility



