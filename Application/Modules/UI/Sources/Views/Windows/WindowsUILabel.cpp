#include "Views/UILabel.h"
#include "WindowsNative.h"
#include <Windows.h>

// ---------------------------------------------------------------------------
CUILabel::CUILabel()
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
    _textColor = SColorF::White();
}

CUILabel::CUILabel(const SRectF& LabelRect)
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
    _textColor = SColorF::White();
}

void CUILabel::OnAddedToSuperview()
{
    if (ViewHWND(this)) return;
    CUIView* P = Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 200; F.Size.Height = 40; }

    DWORD Style = WS_CHILD | WS_VISIBLE;
    switch (TextAlignment())
    {
    case ETextAlignment::Left:   Style |= SS_LEFT; break;
    case ETextAlignment::Center: Style |= SS_CENTER; break;
    case ETextAlignment::Right:  Style |= SS_RIGHT; break;
    }

    HWND h = CreateWindowExW(0, L"STATIC", L"", Style,
        (int)F.Origin.X, (int)F.Origin.Y, (int)F.Size.Width, (int)F.Size.Height,
        hParent, NULL, GetModuleHandle(NULL), NULL);
    _nativeInstance->SetHWND(h);
    ViewSetUserData(h, this);

    // Propagate brush
    COLORREF cr = _nativeInstance->GetBgColorRef();
    if (cr != 0xFFFFFFFF) SetHWNDBrushColor(h, cr);

    if (Text().Len() > 0)
        SetWindowTextA(h, *Text());

    // Apply font size if set
    if (TextSize() > 0.0f)
    {
        HFONT hFont = CreateFontA(
            -(int)TextSize(), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, nullptr);
        if (hFont)
        {
            _nativeInstance->SetFont(hFont);
            SendMessageA(h, WM_SETFONT, (WPARAM)hFont, TRUE);
        }
    }
}

// ---------------------------------------------------------------------------
void CUILabel::SetText(const CString& T)
{
    _text = T;
    HWND h = ViewHWND(this);
    if (!h) { OnAddedToSuperview(); h = ViewHWND(this); }
    if (h) SetWindowTextA(h, *T);
}

const CString& CUILabel::Text() const { return _text; }

// ---------------------------------------------------------------------------
void CUILabel::SetTextAlignment(ETextAlignment A)
{
    _alignment = A;
    HWND hWnd = ViewHWND(this);
    if (!hWnd) return;
    LONG_PTR s = GetWindowLongPtr(hWnd, GWL_STYLE);
    s &= ~(SS_LEFT | SS_CENTER | SS_RIGHT);
    if (A == ETextAlignment::Left) s |= SS_LEFT;
    else if (A == ETextAlignment::Center) s |= SS_CENTER;
    else s |= SS_RIGHT;
    SetWindowLongPtr(hWnd, GWL_STYLE, s);
    InvalidateRect(hWnd, NULL, TRUE);
}

ETextAlignment CUILabel::TextAlignment() const { return _alignment; }

// ---------------------------------------------------------------------------
// Text color / size
// ---------------------------------------------------------------------------
void CUILabel::SetTextColor(const SColorF& Color)
{
    _textColor = Color;
    HWND h = ViewHWND(this);
    if (h) InvalidateRect(h, NULL, TRUE);
}

SColorF CUILabel::TextColor() const { return _textColor; }

void CUILabel::SetTextSize(Float PixelSize)
{
    _textSize = PixelSize;
    HWND h = ViewHWND(this);
    if (!h) return;

    HFONT hFont = CreateFontA(
        -(int)PixelSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, nullptr);
    if (hFont)
    {
        _nativeInstance->SetFont(hFont);
        SendMessageA(h, WM_SETFONT, (WPARAM)hFont, TRUE);
        InvalidateRect(h, NULL, TRUE);
    }
}

Float CUILabel::TextSize() const { return _textSize; }
