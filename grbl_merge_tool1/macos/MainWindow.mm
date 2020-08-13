//
//  MainWindow2.m
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#import "MainWindow.h"
#import "WhiteArea.h"

char * version_get_version();

NSString * ns_get_version()
{
 
   char * p = version_get_version();
   
   NSString * str = [[NSString alloc] initWithUTF8String:p];
   
   free(p);
   
   return str;
   
}

void grbl_merge_tool_set_main_interface(main_interface ** pmaininterface);

@implementation MainWindow

-(id) init
{
   self = [super initWithWindowNibName:@"MainWindow"];
   
 grbl_merge_tool_set_main_interface(&m_pmaininterface);
   
   m_pmaininterface->m_pmainwindow = self;

   return self;
   
}


- (void)windowDidLoad
{
   
   [super windowDidLoad];
   
}


- (void) showWindow:(id)sender
{
 
   [super showWindow:sender];
   
   NSWindow * window = [self window];
   
   CGFloat xPos = NSWidth([[window screen] frame])/2 - NSWidth([window frame])/2;
   CGFloat yPos = NSHeight([[window screen] frame])* 2/3 - NSHeight([window frame])* 2/3;
   [window setFrame:NSMakeRect(xPos, yPos, NSWidth([window frame]), NSHeight([window frame])) display:YES];
   
   [[self label_app_version] setStringValue:ns_get_version()];
   
   [[self progress_indicator] setUsesThreadedAnimation:YES];
   
   NSView * dialog1 = [self dialog1];
   NSView * dialog2 = [self dialog2];
   dialog2.frame = dialog1.frame;

}


- (IBAction)on_next:(id)sender
{

   NSView * dialog1 = [self dialog1];

   NSView * dialog2 = [self dialog2];
   
   if([dialog2 superview] == nil)
   {
      
//      m_dlg2.m_firmwareVersion.set_window_text(strFirmwareVersion);

      
      [[dialog1 superview] replaceSubview:dialog1 with:dialog2];

      [[self button_update] setEnabled: FALSE];
         
      [[self button_cancel] setEnabled: TRUE];
         
      [[self button_update] setTitle: @"Back"];
      
      [[self button_restart] setEnabled: FALSE];
      
      [[self button_restart] setHidden: FALSE];
      
      m_pmaininterface->start_update();
         
   }
   else
   {
         
      [[dialog2 superview] replaceSubview:dialog2 with:dialog1];
      
      [[self button_update] setEnabled: TRUE];
      
      [[self button_cancel] setEnabled: TRUE];
      
      [[self button_update] setTitle: @"Update"];

      [[self button_restart] setEnabled: TRUE];
      
      [[self button_restart] setHidden: TRUE];
      
   }

}


- (IBAction)on_cancel:(id)sender
{
   
   
   if (m_pmaininterface->m_bUpdating)
   {
      
      m_pmaininterface->m_bCancel = true;
      
   }
   else if (m_pmaininterface->m_iUpdating > 0)
   {
      
      m_pmaininterface->m_bCancel = true;
      
   }
   else if (m_pmaininterface->m_bDetecting)
   {
      
      m_pmaininterface->m_bCancel = true;
      
   }
   else
   {
      
      [[NSApplication sharedApplication] terminate:self];
      
   }
   
   
}


- (IBAction)on_radio_download:(id)sender
{
   
   [[self radio_browse] setState:NSControlStateValueOff];

   [[self radio_install_version] setState:NSControlStateValueOff];
   
   [[self button_check_version] setEnabled: TRUE];
   
   [[self label_version] setEnabled: TRUE];

   [[self button_browse] setEnabled: FALSE];
   
   [[self edit_browse] setEnabled: FALSE];

   m_pmaininterface->radio_download();
   
}


- (IBAction)on_install_version:(id)sender
{
   
   [[self radio_browse] setState:NSControlStateValueOff];
   
   [[self radio_download] setState:NSControlStateValueOff];
   
   [[self button_check_version] setEnabled: FALSE];
   
   [[self label_version] setEnabled: FALSE];
   
   [[self button_download] setEnabled: FALSE];
   
   [[self label_download] setEnabled: FALSE];
   
   [[self button_browse] setEnabled: FALSE];
   
   [[self edit_browse] setEnabled: FALSE];
   
   m_pmaininterface->radio_latest();
   
}


- (IBAction)on_radio_browse:(id)sender
{
   
   [[self radio_install_version] setState:NSControlStateValueOff];

   [[self radio_download] setState:NSControlStateValueOff];
   
   [[self button_browse] setEnabled: TRUE];
   
   [[self edit_browse] setEnabled: TRUE];

   [[self button_check_version] setEnabled: FALSE];
   
   [[self label_version] setEnabled: FALSE];

   [[self button_download] setEnabled: FALSE];
   
   [[self label_download] setEnabled: FALSE];
   
   m_pmaininterface->radio_browse();

}


- (IBAction)on_check_version:(id)sender
{

   NSLog(@"check_version pressed");
   
   [[self button_update] setEnabled:FALSE];
   self->m_pmaininterface->check_version();
}

- (IBAction)on_download:(id)sender
{
   
   self->m_pmaininterface->download();
   
}


- (IBAction)on_browse:(id)sender
{
   
   NSOpenPanel *panel = [NSOpenPanel openPanel];
   
   [panel setCanChooseFiles:YES];
   
   [panel setCanChooseDirectories:NO];
   
   [panel setAllowsMultipleSelection:NO]; // yes if more than one dir is allowed
   
   NSString * strHome = NSHomeDirectory();
   
   NSString * strDownloads = [NSString stringWithFormat:@"%@/Downloads",strHome];
   
   NSURL * url = [NSURL fileURLWithPath:strDownloads];
   
   [panel setDirectoryURL:url];
   
   NSInteger clicked = [panel runModal];
   
   if (clicked == NSFileHandlingPanelOKButton)
   {
   
      if([[panel URLs] count] == 1)
      {
         
         NSURL * url = [[panel URLs] objectAtIndex:0];
         
         NSString * path = [url path];
         
         [[self edit_browse] setStringValue:path];
      
         m_pmaininterface->on_browse([path UTF8String]);
         
      }
      
   }
   
}

- (IBAction)on_change_browse:(id)sender
{
   
   
   NSString * path = [[self edit_browse] stringValue];
   
   m_pmaininterface->on_browse([path UTF8String]);
   
}


- (IBAction)on_update_app:(id)sender
{
   m_pmaininterface->update_app();
}


- (void)controlTextDidChange:(NSNotification *)notification {
   NSTextField *textField = [notification object];
   if(textField == [self edit_browse])
   {
   [self on_change_browse:textField];
   }
   //NSLog(@"controlTextDidChange: stringValue == %@", [textField stringValue]);
}


- (IBAction)on_update2:(id)sender
{
   
   m_pmaininterface->select_serial_and_update((int)[[self combo_ports] indexOfSelectedItem]);
   
}

- (IBAction)on_show_log:(id)sender
{
    
    m_pmaininterface->show_log();
    
}


- (IBAction)on_restart:(id)sender
{
   
   self->m_pmaininterface->start_update();
   
}


@end


void main_interface::on_check_version_response(const char *pszStatus
                                            ,
                                            bool bEnable)
{
 
   NSTextField * text = [m_pmainwindow label_version];
   
   NSString * strText = [[NSString alloc] initWithUTF8String:pszStatus];
   
   [[text dd_invokeOnMainThread] setStringValue: strText];
   
   if(bEnable)
   {
      
      [[[m_pmainwindow button_download] dd_invokeOnMainThread]setEnabled:YES];

      [[[m_pmainwindow label_download] dd_invokeOnMainThread]setEnabled:YES];
   
   }
        
}


void main_interface::on_download_response(const char *pszStatus, bool bEnable)
{
   
   NSTextField * text = [m_pmainwindow label_download];
   
   NSString * strText = [[NSString alloc] initWithUTF8String:pszStatus];
   
   [[text dd_invokeOnMainThread]setStringValue: strText];
   
   [[[m_pmainwindow button_update]dd_invokeOnMainThread] setEnabled: bEnable ? YES : NO];
      
}


void main_interface::on_radio_download_response(bool bDownloadEnable, bool bUpdateEnable)
{
   
   [[[m_pmainwindow button_download] dd_invokeOnMainThread] setEnabled: bDownloadEnable ? YES : NO];
   
   [[[m_pmainwindow label_download] dd_invokeOnMainThread] setEnabled: bDownloadEnable ? YES : NO];

   [[[m_pmainwindow button_update] dd_invokeOnMainThread] setEnabled: bUpdateEnable ? YES : NO];

}

void main_interface::on_radio_browse_response(bool bUpdateEnable)
{
   
   [[[m_pmainwindow button_update] dd_invokeOnMainThread] setEnabled: bUpdateEnable ? YES : NO];
   
}


void main_interface::on_browse_response(bool bUpdateEnable)
{
   
   [[[m_pmainwindow button_update] dd_invokeOnMainThread] setEnabled: bUpdateEnable ? YES : NO];
   
}




//void main_interface::set_progress_status(const char * pszPort, const char * pszStatus)
//{
//   
//   NSString * strStatus = [[NSString alloc] initWithUTF8String: pszStatus];
//   
//   [[[m_pmainwindow label_status] dd_invokeOnMainThread] setStringValue:strStatus];
//   
//   if(m_bError)
//   {
//      [[[m_pmainwindow label_status] dd_invokeOnMainThread] setTextColor:[NSColor redColor]];
//   }
//   else
//   {
//      [[[m_pmainwindow label_status] dd_invokeOnMainThread] setTextColor:[NSColor controlTextColor]];
//   }
//   
//}


void main_interface::set_result(const char * pszPort, const char * pszStatus)
{
   
   NSString * strStatus = [[NSString alloc] initWithUTF8String: pszStatus];
   
   [[[m_pmainwindow label_result] dd_invokeOnMainThread] setStringValue:strStatus];
   
   if(m_bError)
   {
      [[[m_pmainwindow label_result] dd_invokeOnMainThread] setTextColor:[NSColor redColor]];
   }
   else
   {
      [[[m_pmainwindow label_result] dd_invokeOnMainThread] setTextColor:[NSColor controlTextColor]];
   }
   
}


void main_interface::set_progress(const char * pszPort, int iPercent, const char * pszStatus)
{
   
   [[[m_pmainwindow progress_indicator] dd_invokeOnMainThread] setDoubleValue:(double)iPercent];
   
   if(iPercent == 0)
   {
      [[[m_pmainwindow label_progress] dd_invokeOnMainThread] setStringValue:@""];
   }
   else
   {

      NSString * strStatus = [[NSString alloc] initWithUTF8String: pszStatus];
      
      [[[m_pmainwindow label_progress] dd_invokeOnMainThread] setStringValue:strStatus];
      
   }
   //            m_dlg2.m_progressControl.redraw_window();
   
   
}

//
//void main_interface::total_progress(int iBytes, int iTotalBytes)
//{
//   
//   int iMax = 70;
//   
//   int i = iMax * iBytes / iTotalBytes;
//   
//   set_progress(i);
//   
//}



void main_interface::show_manual_com(char * const * pszList, int iCount, int iSel)
{
   
   [[[m_pmainwindow combo_ports] dd_invokeOnMainThread] removeAllItems];
   
   
   for (int i = 0; i < iCount; i++)
   {
      
      NSString * strItem = [[NSString alloc] initWithUTF8String:pszList[i]];

      [[[m_pmainwindow combo_ports] dd_invokeOnMainThread] addItemWithTitle:strItem];

   }
   
   [[[m_pmainwindow combo_ports] dd_invokeOnMainThread] selectItemAtIndex:iSel];
   
   [[[m_pmainwindow combo_ports] dd_invokeOnMainThread] setHidden:NO];
   
   [[[m_pmainwindow button_update2] dd_invokeOnMainThread] setHidden:NO];
   
}

void main_interface::hide_manual_com()
{
   
   [[[m_pmainwindow combo_ports] dd_invokeOnMainThread] setHidden:YES];
   
   [[[m_pmainwindow button_update2] dd_invokeOnMainThread] setHidden:YES];
   
}


void main_interface::cancel_enable(bool bEnable)
{
   
   [[[m_pmainwindow button_cancel] dd_invokeOnMainThread] setEnabled:bEnable ? TRUE : FALSE];
   
}


void main_interface::update_enable(bool bEnable)
{
   
   [[[m_pmainwindow button_update] dd_invokeOnMainThread] setEnabled:bEnable ? TRUE : FALSE];
   
}

void main_interface::redraw_window()
{
   //  cancel_enable(bEnable);
   
   //      m_dlg2.m_progressControl.redraw_window();
   
   
   
}



void main_interface::to_close()
{
   
   [[[m_pmainwindow button_cancel] dd_invokeOnMainThread] setTitle:@"Close"];
   
   [[[m_pmainwindow button_restart] dd_invokeOnMainThread] setEnabled: TRUE];
   
}

void main_interface::to_cancel()
{
   
   [[[m_pmainwindow button_cancel] dd_invokeOnMainThread] setTitle:@"Cancel"];
   
   [[[m_pmainwindow button_restart] dd_invokeOnMainThread] setEnabled: FALSE];
   
}




void main_interface::on_app_check_version(const char * pszStatus)
{
   
   NSString * strStatus = [[NSString alloc] initWithUTF8String: pszStatus];
   
   [[[m_pmainwindow label_app_update] dd_invokeOnMainThread] setStringValue:strStatus];
   
      [[[m_pmainwindow label_app_update] dd_invokeOnMainThread] setHidden:NO];
   
      [[[m_pmainwindow button_app_update] dd_invokeOnMainThread] setHidden:NO];
}



void main_interface::quit_system()
{

}



void main_interface::on_app_installation_response(const char * pszStatus)
{
   
   NSString * strStatus = [[NSString alloc] initWithUTF8String: pszStatus];
   
   [[[m_pmainwindow label_app_update] dd_invokeOnMainThread] setStringValue:strStatus];
   
   //m_pmainwindow->m_labelApp.set_window_text(pszStatus);
   
}



void main_interface::on_enable_install_downloaded_bin(const char * strTitle)
{
   
   NSString * strStatus = [[NSString alloc] initWithUTF8String: strTitle];
   
   NSString * title = [NSString stringWithFormat:@"Install Version %@", strStatus];
   
   [[[m_pmainwindow radio_install_version] dd_invokeOnMainThread] setTitle: title];
   [[[m_pmainwindow radio_install_version] dd_invokeOnMainThread] setEnabled: TRUE];
   [[[m_pmainwindow radio_install_version] dd_invokeOnMainThread] setState:NSControlStateValueOn];
   [[[m_pmainwindow radio_browse] dd_invokeOnMainThread]  setState:NSControlStateValueOff];
   [[[m_pmainwindow radio_download] dd_invokeOnMainThread] setState:NSControlStateValueOff];
   [[[m_pmainwindow button_cancel] dd_invokeOnMainThread] setEnabled: TRUE];
   [[[m_pmainwindow button_update] dd_invokeOnMainThread] setEnabled: TRUE];
   
   
}




void main_interface::on_radio_latest_response(bool bUpdateEnable)
{
   
   //m_pmainwindow->m_update.enable_window(bUpdateEnable);
   
}


void main_interface::on_disable_install_downloaded_bin()
{
   
   //m_pmainwindow->m_dlg1.m_radioInstallVersion.enable_window(false);
   
}



