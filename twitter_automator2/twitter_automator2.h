#pragma once


#include "app-ca2/appseed/twitter_automator/twitter_automator.h"



#ifdef _APP_CORE_TWITTER_AUTOMATOR2_LIBRARY
#define CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR2  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR2  CLASS_DECL_IMPORT
#endif

#include "simple_dialog.h"

namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace twitter_automator2
{

   class application;
   class document;
   class task_array;
   class main_form_list;

} // namespace flag


#define APP_TYPE ::twitter_automator2::application

//#include "simple_csv.h"

//#include "twitter_automator2_twitter.h"

//#include "twitter_automator2_task.h"

//#include "twitter_automator2_task_array.h"

#include "twitter_automator2_task_dialog.h"

#include "twitter_automator2_main_form_list.h"

#include "twitter_automator2_document.h"
#include "twitter_automator2_frame.h"
#include "twitter_automator2_main_frame.h"

#include "twitter_automator2_pane_view.h"

#include "twitter_automator2_application.h"

