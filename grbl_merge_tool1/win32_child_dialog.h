#pragma once


namespace grbl_merge_tool
{

   class child_dialog :
      virtual public ::win32::dialog
   {
   public:

      grbl_merge_tool::main_dialog * m_pparent;

      ::win32::window      m_logo;

      INT_PTR on_dialog_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);

      virtual bool on_init_dialog();

   };

} // namespace grbl_merge_tool



