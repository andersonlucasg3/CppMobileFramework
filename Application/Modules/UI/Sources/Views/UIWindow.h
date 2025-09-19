#pragma once

#include "Views/UIView.h"

#include "Graphics/Rects.h"

using namespace UI::Graphics;

class CNativeWindow;
class CUIViewController;

class CUIWindow : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIWindow(const SRectF& WindowRect);
    UI_API ~CUIWindow();

    UI_API void Show();

    UI_API void SetRootViewController(CUIViewController* InRootViewController);

    UI_API CNativeWindow& NativeWindow();

private:
    TSharedPtr<CNativeWindow> _nativeWindow;
};