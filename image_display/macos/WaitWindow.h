//
//  MainWindow2.h
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "aura/aura/os/apple/NSObject+DDExtensions.h"

#import "../macos.h"
#import "../main_interface.h"

@class WaitDialog;

@interface WaitWindow : NSWindowController
{
@public
   
   
   main_interface            * m_pmaininterface;
   
}

@property (strong) IBOutlet NSView *wait_dialog;
@property (strong) IBOutlet NSTextField *label_alert;
@property (strong) IBOutlet NSProgressIndicator *progress_indicator;
@end
