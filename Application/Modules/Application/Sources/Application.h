#pragma once

#include "Object/Object.h"
#include "Object/ClassMacros.h"
#include "Object/Properties/Property.h"

#include "ApplicationDelegate.h"

FORWARD_DECLARE_OBJECT(Application);

class CApplication : public CObject
{
public:
    PROPERTY(CApplicationDelegate, Delegate);

    APPLICATION_API CApplication() = default;
    APPLICATION_API virtual ~CApplication() = default;
    
    APPLICATION_API static CApplication* Shared();

    APPLICATION_API virtual int Run(int argc, char* argv[]);

private:
    static CApplication* _shared;
};
