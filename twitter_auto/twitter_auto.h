#pragma once


#include "core/core/core/_.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/user/user.h"
#include "core/user/html/html/html.h"


#ifdef _APP_CORE_TWITTER_AUTO_LIBRARY
#define CLASS_DECL_APP_CORE_TWITTER_AUTO  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_TWITTER_AUTO  CLASS_DECL_IMPORT
#endif

#include "simple_dialog.h"

namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace twitter_auto
{

   class application;
   class document;
   class user_array;
   class main_form_list;

} // namespace flag


#define APP_TYPE ::twitter_auto::application

#include "simple_csv.h"

#include "twitter_auto_twitter.h"

#include "twitter_auto_container.h"

#include "twitter_auto_user.h"

#include "twitter_auto_user_array.h"

#include "twitter_auto_task_dialog.h"

#include "twitter_auto_main_form_list.h"

#include "twitter_auto_document.h"
#include "twitter_auto_frame.h"
#include "twitter_auto_main_frame.h"

#include "twitter_auto_pane_view.h"

#include "twitter_auto_application.h"

