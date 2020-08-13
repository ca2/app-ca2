//
//  IPMenuletExampleAppDelegate.m
//  IPMenuletExample
//
// Maintained and updated by Stan James
// https://github.com/wanderingstan/IPMenuletExample
//
// Originally  an implementation of the excellent menulet tutorial
// by Andrew Turner, published here:
// http://www.mactech.com/articles/mactech/Vol.22/22.02/Menulet/index.html

#import "app.h"
#import "aura/aura/node/macos/RoundWindowApp.h"
#import "aura/aura/os/apple/NSObject+DDExtensions.h"
#import "MainWindow.h"

void show_main_dialog()
{
   
   RoundWindowApp * papp = [[NSApplication sharedApplication] delegate];
   
   MainWindow* controller = [[MainWindow alloc] init];

   papp.windowcontroller = controller;
   
   [[controller dd_invokeOnMainThreadAndWaitUntilDone:FALSE] showWindow:papp];
   
}
