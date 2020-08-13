#pragma once


#include "app-ca2/appseed/nativeui/nativeui.h"


#include "resource.h"


#ifdef _APP_CORE_HELLOAURA_LIBRARY
#define CLASS_DECL_APP_CORE_HELLOAURA  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_HELLOAURA  CLASS_DECL_IMPORT
#endif


namespace grbl_merge_tool
{

   class main_dialog;
   class application;


   ::file::path get_log_path();

} // namespace flag



#define APP_TYPE ::grbl_merge_tool::application


#ifdef LINUX

#define GDK_VERSION_MIN_REQUIRED GDK_VERSION_3_0

#include <gtk/gtk.h>

#include "linux_dialog.h"

class gdk_lock
{
public:


   gdk_lock()
   {

      gdk_threads_enter();

   }

   ~gdk_lock()
   {

      gdk_flush();

      gdk_threads_leave();

   }

};


#endif




#include "simple_log.h"

#include "version.h"

#include "main_interface.h"

#ifdef WINDOWS_DESKTOP

#include "win32.h"

#elif defined(MACOS)

#include "macos.h"

#elif defined(LINUX)


#include "linux_dialog.h"

#endif

#include "grbl_merge_tool_application.h"


