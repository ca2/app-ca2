#pragma once


#include "core/core/core/_.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/user/user.h"
#include "core/user/html/html/html.h"


#ifdef _APP_CORE_TWITTER_AUTOMATOR_LIBRARY
#define CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR  CLASS_DECL_IMPORT
#endif




namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace twitter_automator
{

   class application;
   class document;
   class task_array;
   class main_form_list;
   CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR string get_file_open(::user::frame * pframe, string strFolder);
   CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR string get_file_save(::user::frame * pframe, string strFolder);

} // namespace flag


#define APP_TYPE ::twitter_automator::application

#include "simple_csv.h"

#include "twitter_automator_transformers.h"

#include "twitter_automator_twitter.h"

#include "twitter_automator_task.h"

#include "twitter_automator_container.h"

#include "twitter_automator_task_array.h"

#include "twitter_automator_main_form_list.h"

#include "twitter_automator_document.h"
#include "twitter_automator_frame.h"
#include "twitter_automator_main_frame.h"

#include "twitter_automator_pane_view.h"

#include "twitter_automator_application.h"

