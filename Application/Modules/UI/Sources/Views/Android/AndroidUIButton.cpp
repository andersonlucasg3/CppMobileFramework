#include "Views/UIButton.h"

#include "AndroidNative.h"

#include "SmartPointer/MakeAndCasts.h"

extern void AndroidWindow_RequestRender();

// ---------------------------------------------------------------------------
CUIButton::CUIButton()
{
    _nativeInstance = MakeShared<CNativeInstance>();
    _textColor = SColorF::White();
}

CUIButton::CUIButton(const SRectF& Rect)
{
    _nativeInstance = MakeShared<CNativeInstance>();
    _textColor = SColorF::White();
    SetFrame(Rect);
}

// ---------------------------------------------------------------------------
void CUIButton::OnAddedToSuperview()
{
    AndroidWindow_RequestRender();
}

// ---------------------------------------------------------------------------
void CUIButton::SetText(const CString& InText)
{
    _text = InText;
    AndroidWindow_RequestRender();
}

const CString& CUIButton::Text() const
{
    return _text;
}

// ---------------------------------------------------------------------------
void CUIButton::SetTextColor(const SColorF& Color)
{
    _textColor = Color;
    AndroidWindow_RequestRender();
}

SColorF CUIButton::TextColor() const { return _textColor; }

// ---------------------------------------------------------------------------
void CUIButton::SetOnClickListener(TFunction<void()>&& Callback)
{
    _onClick = std::move(Callback);
}

// ---------------------------------------------------------------------------
bool CUIButton::OnTouchEvent(EUITouchPhase Phase, SPointF Point)
{
    SRectF f = Frame();

    bool bInside = (Point.X >= f.Origin.X && Point.Y >= f.Origin.Y &&
                    Point.X < f.Origin.X + f.Size.Width &&
                    Point.Y < f.Origin.Y + f.Size.Height);

    switch (Phase)
    {
    case EUITouchPhase::Began:
        if (bInside)
        {
            _bPressed = true;
            AndroidWindow_RequestRender();
            return true;
        }
        break;

    case EUITouchPhase::Moved:
        {
            bool bWasPressed = _bPressed;
            _bPressed = bInside;
            if (bWasPressed != _bPressed)
                AndroidWindow_RequestRender();
            return true;
        }

    case EUITouchPhase::Ended:
        if (_bPressed && bInside)
        {
            _bPressed = false;
            AndroidWindow_RequestRender();
            if (_onClick)
                _onClick();
            return true;
        }
        _bPressed = false;
        AndroidWindow_RequestRender();
        break;

    case EUITouchPhase::Cancelled:
        _bPressed = false;
        AndroidWindow_RequestRender();
        break;
    }

    return false;
}
