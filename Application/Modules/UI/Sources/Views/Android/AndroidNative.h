#pragma once

#include <android/native_window.h>

#include "Graphics/Rects.h"

using namespace UI::Graphics;

// ---------------------------------------------------------------------------
// CNativeInstance — platform-specific native state for Android rendering.
//
// For CUIWindow:  holds the ANativeWindow*.
// For CUIView:    stores frame rect and background colour (ARGB).
//
// GetNativePointer() returns the ANativeWindow* (CUIWindow) or nullptr.
// ---------------------------------------------------------------------------

class CNativeInstance
{
public:
    CNativeInstance()
        : _window(nullptr)
        , _bgColor(0xFF000000)  // opaque black default
        , _frame(0, 0, 0, 0)
    {}

    // ----- Window -------------------------------------------------------

    void  SetWindow(ANativeWindow* w) { _window = w; }
    ANativeWindow* GetWindow() const  { return _window; }

    // ----- Background colour (ARGB) -------------------------------------

    void     SetBackgroundColor(uint32_t c) { _bgColor = c; }
    uint32_t GetBackgroundColor() const     { return _bgColor; }

    // ----- Frame --------------------------------------------------------

    void         SetFrame(const SRectF& f) { _frame = f; }
    SRectF       GetFrame() const          { return _frame; }

    // ----- Generic native pointer ---------------------------------------

    void* Native() const { return (void*)_window; }

private:
    ANativeWindow*  _window;
    uint32_t        _bgColor;
    SRectF          _frame;
};
