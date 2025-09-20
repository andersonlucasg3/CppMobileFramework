#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Graphics/IOS/IOSGraphics.h"

#include "Devices/UIScreen.h"

#include "Views/IOS/IOSUIView.h"

#include <objc/NSObject.h>

template<typename TNativeClass, class TUIClass, class TNativeInstanceClass = TNativeInstance<TNativeClass>>
inline TNativeClass* CreateView(TUIClass* Owner, const SRectF& ViewRect)
{
    TNativeClass* View = [[[TNativeClass alloc] initWithOwner:Owner] autorelease];
    [View setTranslatesAutoresizingMaskIntoConstraints:NO];
    [View setFrame:ToCGRect(ViewRect)];
    return View;
}

template<typename TNativeClass, class TUIClass, class TNativeInstanceClass = TNativeInstance<TNativeClass>>
inline TNativeClass* CreateView(TUIClass* Owner)
{
    return CreateView<TNativeClass, TUIClass, TNativeInstanceClass>(Owner, CUIScreen::MainScreen()->Bounds());
}