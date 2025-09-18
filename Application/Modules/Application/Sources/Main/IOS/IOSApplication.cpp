#include "IOSApplication.h"

#include "NSString.h"
#include "UIApplicationMain.h"

CIOSApplication* CIOSApplication::_shared = nullptr;

CIOSApplication* CIOSApplication::Shared()
{
    return _shared;
}

int CIOSApplication::Run(int argc, char* argv[])
{
    _shared = this;
    
    NS::String* ApplicatoinClass = NS::String::string("IOSApplication", NS::UTF8StringEncoding);
    NS::String* ApplicationDelegateClass = NS::String::string("IOSAppDelegate", NS::UTF8StringEncoding);
    return UI::ApplicationMain(argc, argv, ApplicatoinClass, ApplicationDelegateClass);
}

