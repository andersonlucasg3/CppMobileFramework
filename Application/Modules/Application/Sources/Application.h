#pragma once

#include "Object/Object.h"
#include "Object/ClassMacros.h"
#include "Object/Properties/Property.h"

#include "ApplicationDelegate.h"
#include "SmartPointer/SharedPointer.h"

FORWARD_DECLARE_OBJECT(Application);

class CNativeApplication;

class CApplication : public CObject
{
public:
    PROPERTY(CApplicationDelegate, Delegate);

    APPLICATION_API CApplication() = default;
    APPLICATION_API ~CApplication() = default;
    
    APPLICATION_API int Run(int argc, char* argv[]);
    
    APPLICATION_API static CApplication* SharedApp();

private:
    TSharedPtr<CNativeApplication> _nativeApplication;

    friend class CNativeApplication;
};
