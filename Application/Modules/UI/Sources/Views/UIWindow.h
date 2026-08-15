#pragma once

#include "Object/ClassMacros.h"

#include "Views/UIView.h"

#include "Controllers/UIViewController.h"

#include "Graphics/Rects.h"
#include "Graphics/Points.h"

FORWARD_DECLARE_OBJECT(UIWindow);

class CNativeWindow;

using namespace UI::Graphics;

class CUIWindow : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIWindow();
    UI_API CUIWindow(const SRectF& WindowRect);
    UI_API ~CUIWindow() override = default;

    UI_API void SetRootViewController(CUIViewController* InRootViewController);

    /// Platform-specific render entry point.
    /// On Android this locks the ANativeWindow, draws the view tree, and
    /// posts the buffer. On other platforms it is a no-op.
    UI_API void Render();

    /// Hit-test: returns the deepest view whose frame contains the point.
    /// Returns nullptr if no view is found.
    UI_API CUIView* HitTest(SPointF Point);

    /// Handle window resize: resize the root view and update scroll content.
    UI_API void HandleResize(int clientWidth, int clientHeight);

    /// Handle mouse wheel: scroll the CUIScrollView.
    UI_API void HandleWheel(short zDelta);

private:
    PROPERTY(CUIViewController, _rootViewController);

    static CUIView* HitTestView(CUIView* view, float x, float y, float ox, float oy);
};