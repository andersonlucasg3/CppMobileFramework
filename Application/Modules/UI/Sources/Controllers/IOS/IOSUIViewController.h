#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Controllers/UIViewController.h"

#include <UIKit/UIKit.h>

@interface IOSViewController : UIViewController

- (instancetype)initWithOwner:(CUIViewController*)Owner;

- (void)viewDidLoad;
- (void)viewWillUnload;
- (void)viewDidUnload;

- (void)viewWillAppear:(BOOL)animated;
- (void)viewDidAppear:(BOOL)animated;
- (void)viewWillDisappear:(BOOL)animated;
- (void)viewDidDisappear:(BOOL)animated;

@end

class CNativeViewController : public TNativeInstance<IOSViewController> { };