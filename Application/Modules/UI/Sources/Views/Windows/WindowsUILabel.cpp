#include "Views/UILabel.h"
#include "WindowsNative.h"
#include <Windows.h>

CUILabel::CUILabel()
{
    // CUIView() created a generic child HWND. Swap it for a STATIC control.
    HWND oldHwnd = ViewHWND(this);
    HWND parent = GetParent(oldHwnd);
    HWND h = CreateWindowExW(0, L"STATIC", L"", WS_CHILD|WS_VISIBLE|SS_CENTER,
        0, 0, 200, 40, parent, NULL, GetModuleHandle(NULL), NULL);
    DestroyWindow(oldHwnd);
    _nativeInstance->SetHWND(h);
}

CUILabel::CUILabel(const SRectF& LabelRect)
{
    HWND oldHwnd = ViewHWND(this);
    HWND parent = GetParent(oldHwnd);
    HWND h = CreateWindowExW(0, L"STATIC", L"", WS_CHILD|WS_VISIBLE|SS_CENTER,
        (int)LabelRect.Origin.X, (int)LabelRect.Origin.Y,
        (int)LabelRect.Size.Width, (int)LabelRect.Size.Height,
        parent, NULL, GetModuleHandle(NULL), NULL);
    DestroyWindow(oldHwnd);
    _nativeInstance->SetHWND(h);
}

void CUILabel::SetText(const CString& T) { _text = T; SetWindowTextA(ViewHWND(this), *T); }
const CString& CUILabel::Text() const { return _text; }
void CUILabel::SetTextAlignment(ETextAlignment A)
{
    _alignment = A;
    LONG_PTR s = GetWindowLongPtr(ViewHWND(this), GWL_STYLE);
    s &= ~(SS_LEFT | SS_CENTER | SS_RIGHT);
    if (A == ETextAlignment::Left) s |= SS_LEFT;
    else if (A == ETextAlignment::Center) s |= SS_CENTER;
    else s |= SS_RIGHT;
    SetWindowLongPtr(ViewHWND(this), GWL_STYLE, s);
    InvalidateRect(ViewHWND(this), NULL, TRUE);
}
ETextAlignment CUILabel::TextAlignment() const { return _alignment; }