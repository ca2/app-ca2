//
//  Hello_Multiverse__Tests.m
//  Hello Multiverse!!Tests
//
//  Created by Thomas Borregaard Sørensen on 8/2/15.
//  Copyright (c) 2015 ca2 Desenvolvimento de Software Ltdal. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

@interface Hello_Multiverse__Tests : XCTestCase

@end

@implementation Hello_Multiverse__Tests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    // This is an example of a functional test case.
    XCTAssert(YES, @"Pass");
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
