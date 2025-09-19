#pragma once

#include "Object/Object.h"

#include "Graphics/Rects.h"

class CNativeScreen;

using namespace UI::Graphics;

class CUIScreen : public CObject
{
public:
    UI_API ~CUIScreen() = default;

    UI_API static CUIScreen* MainScreen();

    UI_API SRectF Bounds() const;

    UI_API CNativeScreen& NativeScreen();

protected:
    UI_API CUIScreen() = default;

private:
    TSharedPtr<CNativeScreen> _nativeScreen;
};