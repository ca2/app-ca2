//
//  CustomView.m
//  Quasar Science Update Utility
//
//  Created by Camilo Sasuke Tsumanuma on 22/10/17.
//  Copyright © 2017 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#import "WhiteArea.h"

@interface WhiteArea ()

@end

@implementation WhiteArea

-(void)drawRect:(NSRect)dirtyRect
{
    
   [[NSColor whiteColor] setFill];
    
   NSRectFill(dirtyRect);
   
}

@end
