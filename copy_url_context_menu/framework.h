#pragma once


#include "aura/aura/aura/aura.h"

#include <unknwn.h> // for IClassFactory
#include <strsafe.h> // for StringCchXXX functions
#include <olectl.h> // for SELFREG_E_CLASS
#include <Shlwapi.h>
#include <Shobjidl.h>


#include "resource.h"
#include "registry.h"
#include "class_factory.h"
#include "copy_url_context_menu_application.h"

extern ::copy_url_context_menu::application * g_papp;