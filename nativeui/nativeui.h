#pragma once


#include "aura/aura/aura.h"


#ifdef _APP_CA2_NATIVEUI_LIBRARY
#define CLASS_DECL_APP_CA2_NATIVEUI  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CA2_NATIVEUI  CLASS_DECL_IMPORT
#endif


namespace nativeui
{

   class main_dialog;
   class application;


} // namespace flag



#define APP_TYPE ::nativeui::application


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

#ifdef WINDOWS_DESKTOP

#include "win32.h"

#elif defined(MACOS)

#include "macos.h"

#elif defined(LINUX)


#include "linux_dialog.h"

#endif

#include "nativeui_application.h"


