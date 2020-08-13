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


#define FONT_APP "Tahoma"

#define SIMPLE_LOG
//#undef SIMPLE_LOG


void simple_log_no_new_line(string str)
{

   quasar_science_update_utility::g_papp->simple_log_no_new_line(str);

}


void simple_log(string str)
{

   simple_log_no_new_line(str + "\r\n");

}


void simple_log_putf(void * p, char ch)
{

   quasar_science_update_utility::g_papp->simple_log_putf(p, ch);

}


