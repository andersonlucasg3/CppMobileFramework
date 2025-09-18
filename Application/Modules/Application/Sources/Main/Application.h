#pragma once

#include "Object/Object.h"
#include "Object/ClassMacros.h"
#include "Object/Properties/Property.h"

FORWARD_DECLARE_OBJECT(Application);

class CApplicationDelegate : public CObject
{
public:
    APPLICATION_API CApplicationDelegate() = default;
    APPLICATION_API virtual ~CApplicationDelegate() = default;

    APPLICATION_API virtual void DidFinishLaunching();
};

class CApplication : public CObject
{
public:
    PROPERTY(CApplicationDelegate, Delegate);

    APPLICATION_API CApplication() = default;
    APPLICATION_API virtual ~CApplication() = default;

    APPLICATION_API virtual int Run(int argc, char* argv[]) = 0;
};
