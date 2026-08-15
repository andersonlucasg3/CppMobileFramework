#include "Views/UIButton.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"

// ---------------------------------------------------------------------------
void WindowsButton_FireClick(CUIButton* btn)
{
    if (btn && btn->_onClick)
        btn->_onClick();
}

// ---------------------------------------------------------------------------
CUIButton::CUIButton()
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
    _textColor = SColorF::White();
}

CUIButton::CUIButton(const SRectF& Rect)
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
    _textColor = SColorF::White();
}

// ---------------------------------------------------------------------------
void CUIButton::OnAddedToSuperview()
{
    if (ViewHWND(this)) return;
    CUIView* P = Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 120; F.Size.Height = 40; }

    HWND h = CreateWindowExW(0, L"BUTTON", L"",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        (int)F.Origin.X, (int)F.Origin.Y, (int)F.Size.Width, (int)F.Size.Height,
        hParent, NULL, GetModuleHandle(NULL), NULL);

    _nativeInstance->SetHWND(h);
    ViewSetUserData(h, this);

    // Propagate brush
    COLORREF cr = _nativeInstance->GetBgColorRef();
    if (cr != 0xFFFFFFFF) SetHWNDBrushColor(h, cr);

    if (Text().Len() > 0)
        SetWindowTextA(h, *Text());
}

// ---------------------------------------------------------------------------
void CUIButton::SetText(const CString& InText)
{
    _text = InText;
    HWND h = ViewHWND(this);
    if (h) SetWindowTextA(h, *InText);
}

const CString& CUIButton::Text() const { return _text; }

// ---------------------------------------------------------------------------
void CUIButton::SetTextColor(const SColorF& Color)
{
    _textColor = Color;
    HWND h = ViewHWND(this);
    if (h) InvalidateRect(h, NULL, TRUE);
}

SColorF CUIButton::TextColor() const { return _textColor; }

// ---------------------------------------------------------------------------
void CUIButton::SetOnClickListener(TFunction<void()>&& Callback)
{
    _onClick = std::move(Callback);
}

// ---------------------------------------------------------------------------
bool CUIButton::OnTouchEvent(EUITouchPhase /*Phase*/, SPointF /*Point*/)
{
    return false;
}
