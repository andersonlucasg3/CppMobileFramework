#pragma once

#include "Object/ClassMacros.h"
#include "Object/Object.h"

#include "SmartPointer/SharedPointer.h"

#include "Views/UIWindow.h"

FORWARD_DECLARE_OBJECT(Application);

class CNativeApplication;

class CApplication : public CObject
{
public:
    APPLICATION_API CApplication() = default;
    APPLICATION_API ~CApplication() = default;
    
    APPLICATION_API static CApplication* SharedApp();

    //// Begin Events
    APPLICATION_API virtual CUIWindow* CreateMainWindow() = 0;
    APPLICATION_API virtual void DidLaunch() {}
    //// End Events
    
    APPLICATION_API int Run(int argc, char* argv[]);

private:
    TSharedPtr<CNativeApplication> _nativeApplication;

    friend class CNativeApplication;
};
