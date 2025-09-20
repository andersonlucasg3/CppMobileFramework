#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Controllers/UIViewController.h"

#include <UIKit/UIKit.h>

@interface IOSViewController : UIViewController

- (instancetype)initWithOwner:(CUIViewController*)Owner;

- (void)viewDidLoad;

@end

class CNativeViewController : public TNativeInstance<IOSViewController> { };