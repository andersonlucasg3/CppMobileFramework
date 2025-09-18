#pragma once

#include "Object/Object.h"

class CApplication;

class CApplicationDelegate : public CObject
{
public:
    APPLICATION_API CApplicationDelegate() = default;
    APPLICATION_API virtual ~CApplicationDelegate() = default;

    APPLICATION_API virtual void DidFinishLaunching(CApplication* InApplication) = 0;
};