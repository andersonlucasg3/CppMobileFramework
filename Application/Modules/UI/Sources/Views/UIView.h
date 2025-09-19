#pragma once

#include "Object/Object.h"

class CNativeView;

class CUIView : public CObject
{
public:
    UI_API CUIView();
    UI_API ~CUIView();

    UI_API CNativeView& NativeView();

private:
    TSharedPtr<CNativeView> _nativeView;
};