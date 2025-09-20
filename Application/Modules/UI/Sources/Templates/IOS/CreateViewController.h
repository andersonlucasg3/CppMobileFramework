#pragma once

#include "Templates/IOS/NativeInstance.h"

template<typename TNativeClass, class TControllerClass, class TNativeInstanceClass = TNativeInstance<TNativeClass>>
inline TNativeClass* CreateViewController(TControllerClass* Owner)
{
    return [[[TNativeClass alloc] initWithOwner:Owner] autorelease];
}

template<typename TNativeClass, class TControllerClass, class TNativeInstanceClass = TNativeInstance<TNativeClass>>
inline TNativeClass* CreateNavigationController(TControllerClass* Owner)
{
    return [[[TNativeClass alloc] initWithOwner:Owner] autorelease];
}

template<typename TNativeClass, typename TNativeControllerClass, class TControllerClass, class TNativeInstanceClass = TNativeInstance<TNativeClass>>
inline TNativeClass* CreateNavigationController(TControllerClass* Owner, TNativeControllerClass* RootViewController)
{
    return [[[TNativeClass alloc] initWithOwner:Owner andRootViewController:RootViewController] autorelease];
}