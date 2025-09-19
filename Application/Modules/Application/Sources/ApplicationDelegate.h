#pragma once

#include "Object/ClassMacros.h"
#include "Object/Object.h"

class CApplication;

FORWARD_DECLARE_OBJECT(ApplicationDelegate);

class CApplicationDelegate : public CObject
{
public:
    APPLICATION_API CApplicationDelegate() = default;
    APPLICATION_API virtual ~CApplicationDelegate() = default;

    APPLICATION_API virtual void DidFinishLaunching(CApplication* InApplication) = 0;
};