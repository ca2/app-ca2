//
//  mac_main.cpp
//  Hello Multiverse!!
//
//  Created by Camilo Sasuke Tsumanuma on 8/2/15.
//  Copyright (c) 2015 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//




//
//  main.m
//  app
//
//  Created by Camilo Sasuke Tsumanuma on 1/19/13.
//  Copyright (c) 2013 Camilo Sasuke Tsumanuma. All rights reserved.
//

#include "framework.h"
#include "aura/aura/app/acid.inl"

extern
::aura::PFN_GET_NEW_APP g_pfnNewApp;

::aura::application * get_acid_app(::aura::application * pappParent)
{
   return new ::grbl_merge_tool::application();
}
   
//int main(int argc, char *argv[])
//{
//
//   if(!defer_aura_init())
//      return -1;
//
//   g_pfnNewApp = &get_acid_app;
//
//   int iResult = aura_main_command_line(": app=acid", argc, argv);
//
//   try
//   {
//
//      defer_aura_term();
//
//   }
//   catch(...)
//   {
//
//   }
//
//   return iResult;
//
//}


