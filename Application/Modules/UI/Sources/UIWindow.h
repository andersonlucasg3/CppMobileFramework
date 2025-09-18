#pragma once

#include "Object/Object.h"

#include "Graphics/Rects.h"

using namespace UI::Graphics;

class CNativeWindow;

class CUIWindow : public CObject
{
    using Super = CObject;

public:
    UI_API CUIWindow(const SRectF& WindowRect);
    UI_API ~CUIWindow();

    UI_API void Show();

private:
    TSharedPtr<CNativeWindow> _nativeWindow;
};