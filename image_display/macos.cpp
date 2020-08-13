//
//  macos.cpp
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#include "framework.h"


namespace version
{

   
   string get_version()
   {

      string strVersion;
      
      char * pszVersion = macos_get_version();

      strVersion = pszVersion;
      
      free(pszVersion);
      
      return strVersion;
      
   }
   
   
} // namespace version


char * version_get_version()
{
 
   return strdup(::version::get_version());
   
}
