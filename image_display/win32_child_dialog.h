#pragma once


namespace quasar_science_update_utility
{

   class child_dialog :
      virtual public ::win32::dialog
   {
   public:

      quasar_science_update_utility::main_dialog * m_pparent;

      ::win32::window      m_logo;

      INT_PTR on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);

      virtual bool on_init_dialog();

   };

} // namespace quasar_science_update_utility



