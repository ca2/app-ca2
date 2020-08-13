//
//  macos.m
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#import <Foundation/Foundation.h>

char * macos_get_version()
{
NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
 
   return strdup([appVersion UTF8String]);
}
