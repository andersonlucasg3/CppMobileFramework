#pragma once

#include "Object/ClassMacros.h"
#include "Views/UIView.h"

#include "Graphics/Rects.h"

FORWARD_DECLARE_OBJECT(UIWindow);

class CNativeWindow;
class CUIViewController;

using namespace UI::Graphics;

class CUIWindow : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIWindow();
    UI_API CUIWindow(const SRectF& WindowRect);
    UI_API ~CUIWindow() override = default;

    UI_API void Show();

    UI_API void SetRootViewController(CUIViewController* InRootViewController);
};