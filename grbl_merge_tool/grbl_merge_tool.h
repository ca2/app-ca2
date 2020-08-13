#pragma once


#include "core/core/core/_.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/user/user.h"
#include "core/user/html/html/html.h"


#include "windows_itemidlist.h"
#include "windows_browse_folder.h"


#ifdef _APP_CORE_GRBL_MERGE_TOOL_LIBRARY
#define CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL  CLASS_DECL_IMPORT
#endif

#include "simple_dialog.h"

namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace grbl_merge_tool
{

   class application;
   class document;
   class user_array;
   class main_form_list;

   enum ::estatus    
   {

      UNKNOWN = 0,
      IDLE = 1,
      ALARM = 2,
      RUN = 3,
      HOME = 4,
      HOLD = 5,
      QUEUE = 6,
      CHECK = 7,
      DOOR = 8

   };


   enum e_mode
   {
      mode_none,
      mode_file,
      mode_library,

   };
   enum e_hint
   {

      update_open_file = 16384,
      update_open_library,
      update_start,
      update_stop,
      update_reset

   };


} // namespace flag


#define APP_TYPE ::grbl_merge_tool::application

#include "simple_csv.h"

#include "grbl_merge_tool_top_frame.h"

#include "grbl_merge_tool_top_pane_view.h"

#include "grbl_merge_tool_command.h"

#include "grbl_merge_tool_control.h"

#include "grbl_merge_tool_control_view.h"

#include "grbl_merge_tool_main_view.h"

#include "grbl_merge_tool_task_dialog.h"

#include "grbl_merge_tool_main_form_list.h"

#include "grbl_merge_tool_document.h"
#include "grbl_merge_tool_frame.h"
#include "grbl_merge_tool_main_frame.h"

#include "grbl_merge_tool_pane_view.h"

#include "grbl_merge_tool_application.h"

