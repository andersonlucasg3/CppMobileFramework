#include "Views/UILabel.h"
#include "WindowsNative.h"
#include <Windows.h>

static void EnsureLabelHWND(CUILabel* L)
{
    if (ViewHWND(L)) return;
    CUIView* P = L->Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = L->Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 200; F.Size.Height = 40; }

    DWORD Style = WS_CHILD | WS_VISIBLE;
    switch (L->TextAlignment())
    {
    case ETextAlignment::Left:   Style |= SS_LEFT; break;
    case ETextAlignment::Center: Style |= SS_CENTER; break;
    case ETextAlignment::Right:  Style |= SS_RIGHT; break;
    }

    HWND h = CreateWindowExW(0, L"STATIC", L"", Style,
        (int)F.Origin.X, (int)F.Origin.Y, (int)F.Size.Width, (int)F.Size.Height,
        hParent, NULL, GetModuleHandle(NULL), NULL);
    ((CNativeInstance*)L->GetNativePointer())->SetHWND(h);

    if (L->Text().Len() > 0)
        SetWindowTextA(h, *L->Text());
}

CUILabel::CUILabel()
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

CUILabel::CUILabel(const SRectF& LabelRect)
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

void CUILabel::OnAddedToSuperview()
{
    EnsureLabelHWND(this);
}

void CUILabel::SetText(const CString& T)
{
    _text = T;
    EnsureLabelHWND(this);
    HWND h = ViewHWND(this);
    if (h) SetWindowTextA(h, *T);
}

const CString& CUILabel::Text() const { return _text; }

void CUILabel::SetTextAlignment(ETextAlignment A)
{
    _alignment = A;
    EnsureLabelHWND(this);
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