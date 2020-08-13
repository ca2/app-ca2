//
//  MainWindow2.m
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#import "WaitWindow.h"
#import "WhiteArea.h"


void quasar_science_update_utility_set_main_interface(main_interface ** pmaininterface);


@implementation WaitWindow


-(id) init
{
   self = [super initWithWindowNibName:@"WaitWindow"];
   quasar_science_update_utility_set_main_interface(&m_pmaininterface);
   
   
   m_pmaininterface->m_pwaitwindow = self;
   
   return self;
   
}



- (void) showWindow:(id)sender
{
   
   [super showWindow:sender];
   
   NSWindow * window = [self window];
   
   CGFloat xPos = NSWidth([[window screen] frame])/2 - NSWidth([window frame])/2;
   CGFloat yPos = NSHeight([[window screen] frame])* 2/3 - NSHeight([window frame])* 2/3;
   [window setFrame:NSMakeRect(xPos, yPos, NSWidth([window frame]), NSHeight([window frame])) display:YES];
   
   [[self progress_indicator] startAnimation:window];
   
   
}


@end




void main_interface::wait_dialog_alert(const char * pszAlert)
{
   
   NSString * strAlert = [[NSString alloc] initWithUTF8String: pszAlert];
   
   [[[m_pwaitwindow label_alert] dd_invokeOnMainThread] setStringValue:strAlert];
   
}

void main_interface::wait_dialog_stop_and_hide_animation()
{
   

   [[m_pwaitwindow progress_indicator] stopAnimation:m_pwaitwindow];

   [[[m_pwaitwindow progress_indicator] dd_invokeOnMainThread] setHidden:YES];

}
