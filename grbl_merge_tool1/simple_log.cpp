//
//  simple_log.cpp
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#include "framework.h"
#include "logprintf.h"
#include <sys/stat.h>

CLASS_DECL_AURA void file_add_contents_raw(const char * path, const char * psz);


#undef RASPBERRY
// #define RASPBERRY

string & simple_log_line()
{

   // suggestiong implement register_auto_delete at::aura::system
   // static string * pstr = ::aura::system::g_p->register_auto_delete(new string());
   static string * pstr = new string(); // intentional memory leak (just one string for entire process life time)

   return *pstr;

}


#define FONT_APP "Tahoma"

#define SIMPLE_LOG
//#undef SIMPLE_LOG

void simple_log_no_new_line(string str)
{

#ifdef SIMPLE_LOG

   ::file::path path = ::grbl_merge_tool::get_log_path();

   if (!file_exists(path))
   {

      file_put_contents_dup(path, "");

#ifdef LINUX

      chmod(path, 0666);

#endif

   }

   file_add_contents_raw(path, str);

#endif

}

void simple_log(string str)
{

   simple_log_no_new_line(str + "\r\n");

}

void simple_log_putf(void * p, char ch)
{

   simple_log_line() += string(ch);

   if (simple_log_line().contains('\r') || simple_log_line().contains('\n'))
   {

      simple_log_no_new_line(simple_log_line());

      simple_log_line().Empty();

   }

}
