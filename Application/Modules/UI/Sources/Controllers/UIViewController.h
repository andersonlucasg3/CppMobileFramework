#pragma once

#include "Object/Object.h"

class CNativeViewController;

class CUIViewController : public CObject
{
public:
    UI_API CUIViewController();
    UI_API ~CUIViewController();

    UI_API CNativeViewController& NativeViewController();

private:
    TSharedPtr<CNativeViewController> _nativeViewController;
};