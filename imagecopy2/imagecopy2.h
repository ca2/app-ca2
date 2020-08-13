#pragma once


#include "core/core/core/_.h"
#include "core/user/user/user.h"
#include "core/user/html/html/html.h"
#include "core/user/userex/userex.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/simple/simple.h"


#ifdef _APP_CA2_IMAGECOPY2_LIBRARY
#define CLASS_DECL_APP_CA2_IMAGECOPY2  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CA2_IMAGECOPY2  CLASS_DECL_IMPORT
#endif

#include "simple_dialog.h"
#include "exif.h"
namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace imagecopy2
{

   class application;
   class document;
   class task_array;
   class main_form_list;

} // namespace flag


#define APP_TYPE ::imagecopy2::application

#include "imagecopy2_task_dialog.h"

#include "imagecopy2_main_form_list.h"
#include "imagecopy2_info_view.h"
#include "imagecopy2_left_view.h"
#include "imagecopy2_main_view.h"

#include "imagecopy2_document.h"
#include "imagecopy2_frame.h"
#include "imagecopy2_main_frame.h"

#include "imagecopy2_pane_view.h"

#include "imagecopy2_application.h"

