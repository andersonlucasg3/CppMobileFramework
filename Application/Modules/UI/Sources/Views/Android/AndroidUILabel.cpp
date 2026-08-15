#include "Views/UILabel.h"

#include "AndroidNative.h"

#include "SmartPointer/MakeAndCasts.h"

// ---------------------------------------------------------------------------
// Forward-declared render trigger.
// ---------------------------------------------------------------------------
extern void AndroidWindow_RequestRender();

// ---------------------------------------------------------------------------
// CUILabel constructors
// ---------------------------------------------------------------------------

CUILabel::CUILabel()
{
    _nativeInstance = MakeShared<CNativeInstance>();
}

CUILabel::CUILabel(const SRectF& LabelRect)
{
    _nativeInstance = MakeShared<CNativeInstance>();
    SetFrame(LabelRect);
}

// ---------------------------------------------------------------------------
// OnAddedToSuperview — trigger render so the new label appears.
// ---------------------------------------------------------------------------

void CUILabel::OnAddedToSuperview()
{
    AndroidWindow_RequestRender();
}

// ---------------------------------------------------------------------------
// SetText / Text
// ---------------------------------------------------------------------------

void CUILabel::SetText(const CString& T)
{
    _text = T;
    AndroidWindow_RequestRender();
}

const CString& CUILabel::Text() const
{
    return _text;
}

// ---------------------------------------------------------------------------
// SetTextAlignment / TextAlignment
// ---------------------------------------------------------------------------

void CUILabel::SetTextAlignment(ETextAlignment A)
{
    _alignment = A;
    AndroidWindow_RequestRender();
}

ETextAlignment CUILabel::TextAlignment() const
{
    return _alignment;
}

// ---------------------------------------------------------------------------
// SetTextColor / TextColor
// ---------------------------------------------------------------------------

void CUILabel::SetTextColor(const SColorF& Color)
{
    _textColor = Color;
    AndroidWindow_RequestRender();
}

SColorF CUILabel::TextColor() const
{
    return _textColor;
}

// ---------------------------------------------------------------------------
// SetTextSize / TextSize
// ---------------------------------------------------------------------------

void CUILabel::SetTextSize(Float PixelSize)
{
    _textSize = PixelSize;
    AndroidWindow_RequestRender();
}

Float CUILabel::TextSize() const
{
    return _textSize;
}
