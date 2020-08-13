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

@class Dialog1;
@class Dialog2;

@interface MainWindow : NSWindowController
{
@public
   
   
   main_interface            * m_pmaininterface;
   main_interface::e_mode        m_emode;
   
}

// Dialog1
@property (strong) IBOutlet NSButton *radio_download;
@property (strong) IBOutlet NSButton *button_check_version;
@property (strong) IBOutlet NSTextField *label_version;
@property (strong) IBOutlet NSButton *button_download;

@property (strong) IBOutlet NSButton *radio_install_version;

@property (strong) IBOutlet NSTextField *label_download;

@property (strong) IBOutlet NSButton *radio_browse;
@property (strong) IBOutlet NSTextField *edit_browse;
@property (strong) IBOutlet NSButton *button_browse;


// Dialog2
@property (strong) IBOutlet NSTextField *label_status;
@property (strong) IBOutlet NSTextField *label_progress;
@property (strong) IBOutlet NSTextField *label_result;
@property (strong) IBOutlet NSProgressIndicator *progress_indicator;
@property (strong) IBOutlet NSButton *button_update2;
@property (strong) IBOutlet NSPopUpButton *combo_ports;

@property (strong) IBOutlet NSTextField *label_app_version;
@property (strong) IBOutlet NSTextField *label_app_update;
@property (strong) IBOutlet NSButton *button_app_update;


@property (strong) IBOutlet NSTextField *firmware_version2;

@property (strong) IBOutlet NSView *dialog1;
@property (strong) IBOutlet NSView *dialog2;
@property (strong) IBOutlet NSButton *button_restart;
@property (strong) IBOutlet NSButton *button_update;
@property (strong) IBOutlet NSButton *button_cancel;


@end

