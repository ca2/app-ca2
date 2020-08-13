#pragma once


#include "aura/aura/aura.h"


#include "resource.h"


#ifdef _APP_CORE_HELLOAURA_LIBRARY
#define CLASS_DECL_APP_CORE_HELLOAURA  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_HELLOAURA  CLASS_DECL_IMPORT
#endif

#define STANDALONE "app/image_display"


namespace image_display
{


   class application;



} // namespace image_display



#define APP_TYPE ::image_display::application

#include "aura/net/netserver/netserver.h"

#include "image_display_socket.h"

#include "image_display_socket_thread.h"

#include "image_display_window.h"

#include "image_display_application.h"



