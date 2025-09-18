#include "ObjCIOSApplication.h"

#if __OBJC__

#import "ObjCIOSAppDelegate.h"

#import <UIKit/UIKit.h>

@implementation IOSApplication

- (instancetype)init 
{
    self = [super init];
    if (self) 
    {
        self.delegate = [[IOSAppDelegate alloc] init];
    }
    return self;
}

- (void)dealloc 
{
    [self.delegate release];
    self.delegate = nil;

    [super dealloc];
}

@end

#endif