#include "Views/UIView.h"

#include "AndroidNative.h"

#include "SmartPointer/MakeAndCasts.h"

// ---------------------------------------------------------------------------
// Forward-declared render trigger (defined in AndroidUIWindow.cpp).
// Every mutation that affects the screen calls this.
// ---------------------------------------------------------------------------
extern void AndroidWindow_RequestRender();

// ---------------------------------------------------------------------------
// CUIView — base view constructors
// ---------------------------------------------------------------------------

CUIView::CUIView()
{
    _nativeInstance = MakeShared<CNativeInstance>();
}

CUIView::CUIView(const SRectF& ViewRect)
{
    _nativeInstance = MakeShared<CNativeInstance>();
    SetFrame(ViewRect);
}

CUIView::CUIView(CNativeViewController* Owner)
{
    _nativeInstance = MakeShared<CNativeInstance>();
}

// ---------------------------------------------------------------------------
// Add / Remove subview
// ---------------------------------------------------------------------------

void CUIView::AddSubview(CUIView* Subview)
{
    _subviews.Add(Subview);
    Subview->_superview = this;

    // Notify the child so it can create its native widget if needed.
    Subview->OnAddedToSuperview();

    AndroidWindow_RequestRender();
}

void CUIView::RemoveFromSuperview()
{
    if (_superview)
    {
        _superview->_subviews.Remove(this);
        _superview = nullptr;
    }
    AndroidWindow_RequestRender();
}

// ---------------------------------------------------------------------------
// OnAddedToSuperview — no native widget needed for base CUIView on Android
// ---------------------------------------------------------------------------
void CUIView::OnAddedToSuperview()
{
}

// ---------------------------------------------------------------------------
// Background color
// ---------------------------------------------------------------------------

void CUIView::SetBackgroundColor()
{
    _nativeInstance->SetBackgroundColor(0xFF888888); // grey
    AndroidWindow_RequestRender();
}

void CUIView::SetBackgroundColor(const SColorF& Color)
{
    uint8_t r = (uint8_t)(Color.R() * 255.0f);
    uint8_t g = (uint8_t)(Color.G() * 255.0f);
    uint8_t b = (uint8_t)(Color.B() * 255.0f);
    uint8_t a = (uint8_t)(Color.A() * 255.0f);
    uint32_t argb = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    _nativeInstance->SetBackgroundColor(argb);
    AndroidWindow_RequestRender();
}

SColorF CUIView::BackgroundColor() const
{
    uint32_t argb = _nativeInstance->GetBackgroundColor();
    return SColorF(
        ((argb >> 16) & 0xFF) / 255.0f,
        ((argb >>  8) & 0xFF) / 255.0f,
        ((argb >>  0) & 0xFF) / 255.0f,
        ((argb >> 24) & 0xFF) / 255.0f
    );
}

// ---------------------------------------------------------------------------
// Touch event (base — returns false)
// ---------------------------------------------------------------------------

bool CUIView::OnTouchEvent(EUITouchPhase /*Phase*/, SPointF /*Point*/)
{
    return false;
}

// ---------------------------------------------------------------------------
// Frame get/set
// ---------------------------------------------------------------------------

SRectF CUIView::Frame() const
{
    return _nativeInstance->GetFrame();
}

void CUIView::SetFrame(const SRectF& F)
{
    _nativeInstance->SetFrame(F);
    AndroidWindow_RequestRender();
}

// ---------------------------------------------------------------------------
// Superview / Native pointer
// ---------------------------------------------------------------------------

CUIView* CUIView::Superview() const
{
    return _superview.Get();
}

void* CUIView::GetNativePointer() const
{
    return _nativeInstance->Native();
}
