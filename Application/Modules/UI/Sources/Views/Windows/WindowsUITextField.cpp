#include "Views/UITextField.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"
#include <Windows.h>
#include <Commctrl.h>

// ---------------------------------------------------------------------------
// Helper for MainWndProc to fire text-changed callback
// ---------------------------------------------------------------------------
void WindowsTextField_FireChanged(CUITextField* tf)
{
    if (!tf || !tf->_onTextChanged) return;

    HWND h = ViewHWND(tf);
    if (!h) return;

    int len = GetWindowTextLengthA(h);
    if (len == 0)
    {
        tf->_onTextChanged(CString(""));
        return;
    }

    char* buf = new char[len + 1];
    GetWindowTextA(h, buf, len + 1);
    tf->_onTextChanged(CString(buf));
    delete[] buf;
}

// ---------------------------------------------------------------------------
CUITextField::CUITextField()
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
    _textColor = SColorF::Black();
}

CUITextField::CUITextField(const SRectF& TextFieldRect)
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
    _textColor = SColorF::Black();
}

void CUITextField::OnAddedToSuperview()
{
    if (ViewHWND(this)) return;
    CUIView* P = Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 200; F.Size.Height = 30; }

    HWND h = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        (int)F.Origin.X, (int)F.Origin.Y, (int)F.Size.Width, (int)F.Size.Height,
        hParent, NULL, GetModuleHandle(NULL), NULL);

    _nativeInstance->SetHWND(h);
    ViewSetUserData(h, this);

    // Propagate brush
    COLORREF cr = _nativeInstance->GetBgColorRef();
    if (cr != 0xFFFFFFFF) SetHWNDBrushColor(h, cr);

    // Set placeholder if available (requires comctl32 v6 manifest)
    if (Placeholder().Len() > 0)
    {
        SendMessageA(h, 0x1501 /*EM_SETCUEBANNER*/, 0, (LPARAM)*Placeholder());
    }
}

// ---------------------------------------------------------------------------
// Text
// ---------------------------------------------------------------------------
void CUITextField::SetText(const CString& InText)
{
    _text = InText;
    HWND h = ViewHWND(this);
    if (h) SetWindowTextA(h, *InText);
}

const CString& CUITextField::Text() const
{
    HWND h = ViewHWND(this);
    if (h)
    {
        int len = GetWindowTextLengthA(h);
        if (len > 0)
        {
            char* buf = new char[len + 1];
            GetWindowTextA(h, buf, len + 1);
            // const_cast is ugly but _text is mutable in practice
            const_cast<CUITextField*>(this)->_text = CString(buf);
            delete[] buf;
        }
        else
        {
            const_cast<CUITextField*>(this)->_text = CString("");
        }
    }
    return _text;
}

// ---------------------------------------------------------------------------
// Placeholder
// ---------------------------------------------------------------------------
void CUITextField::SetPlaceholder(const CString& InPlaceholder)
{
    _placeholder = InPlaceholder;
    HWND h = ViewHWND(this);
    if (h) SendMessageA(h, 0x1501, 0, (LPARAM)*InPlaceholder);
}

const CString& CUITextField::Placeholder() const { return _placeholder; }

// ---------------------------------------------------------------------------
// Text color
// ---------------------------------------------------------------------------
void CUITextField::SetTextColor(const SColorF& Color)
{
    _textColor = Color;
    HWND h = ViewHWND(this);
    if (h) InvalidateRect(h, NULL, TRUE);
}

SColorF CUITextField::TextColor() const { return _textColor; }

// ---------------------------------------------------------------------------
// Text-changed callback
// ---------------------------------------------------------------------------
void CUITextField::SetOnTextChanged(TFunction<void(const CString&)>&& Callback)
{
    _onTextChanged = std::move(Callback);
}

// ---------------------------------------------------------------------------
// Focus
// ---------------------------------------------------------------------------
bool CUITextField::IsFocused() const
{
    HWND h = ViewHWND(this);
    return h && GetFocus() == h;
}

void CUITextField::SetFocused(bool bFocused)
{
    HWND h = ViewHWND(this);
    if (!h) return;
    _bFocused = bFocused;
    if (bFocused)
        SetFocus(h);
}

// ---------------------------------------------------------------------------
// Touch
// ---------------------------------------------------------------------------
bool CUITextField::OnTouchEvent(EUITouchPhase Phase, SPointF /*Point*/)
{
    if (Phase == EUITouchPhase::Began)
    {
        SetFocused(true);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// HandleKeyEvent — no-op on Windows (EDIT handles natively)
// ---------------------------------------------------------------------------
bool CUITextField::HandleKeyEvent(int, int)
{
    return false;
}
