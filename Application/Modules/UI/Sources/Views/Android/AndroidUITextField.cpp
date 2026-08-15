#include "Views/UITextField.h"

#include "AndroidNative.h"

#include "SmartPointer/MakeAndCasts.h"

extern void AndroidWindow_RequestRender();

// Forward-declared keyboard helpers from the Application module.
extern "C" void AndroidApplication_ShowSoftKeyboard(bool bShow);

// ---------------------------------------------------------------------------
// Global focused text field (only one at a time)
// ---------------------------------------------------------------------------
static CUITextField* GFocusedTextField = nullptr;

CUITextField* AndroidWindow_GetFocusedTextField()
{
    return GFocusedTextField;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------

void CUITextField::OnAddedToSuperview()
{
    // No special native widget needed on Android
}

CUITextField::CUITextField()
{
    _nativeInstance = MakeShared<CNativeInstance>();
    _textColor = SColorF::Black();
    _nativeInstance->SetBackgroundColor(0xFFF0F0F0);
}

CUITextField::CUITextField(const SRectF& TextFieldRect)
{
    _nativeInstance = MakeShared<CNativeInstance>();
    _textColor = SColorF::Black();
    _nativeInstance->SetBackgroundColor(0xFFF0F0F0);
    SetFrame(TextFieldRect);
}

// ---------------------------------------------------------------------------
// Text
// ---------------------------------------------------------------------------

void CUITextField::SetText(const CString& InText)
{
    _text = InText;
    AndroidWindow_RequestRender();
}

const CString& CUITextField::Text() const
{
    return _text;
}

// ---------------------------------------------------------------------------
// Placeholder
// ---------------------------------------------------------------------------

void CUITextField::SetPlaceholder(const CString& InPlaceholder)
{
    _placeholder = InPlaceholder;
    AndroidWindow_RequestRender();
}

const CString& CUITextField::Placeholder() const
{
    return _placeholder;
}

// ---------------------------------------------------------------------------
// Text color
// ---------------------------------------------------------------------------

void CUITextField::SetTextColor(const SColorF& Color)
{
    _textColor = Color;
    AndroidWindow_RequestRender();
}

SColorF CUITextField::TextColor() const
{
    return _textColor;
}

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
    return _bFocused;
}

void CUITextField::SetFocused(bool bFocused)
{
    if (_bFocused == bFocused) return;

    // Unfocus previous
    if (bFocused && GFocusedTextField && GFocusedTextField != this)
    {
        GFocusedTextField->_bFocused = false;
    }

    _bFocused = bFocused;
    GFocusedTextField = bFocused ? this : nullptr;

    AndroidApplication_ShowSoftKeyboard(bFocused);
    AndroidWindow_RequestRender();
}

// ---------------------------------------------------------------------------
// Touch
// ---------------------------------------------------------------------------

bool CUITextField::OnTouchEvent(EUITouchPhase Phase, SPointF Point)
{
    if (Phase == EUITouchPhase::Began)
    {
        SetFocused(true);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Key event handling (called from AndroidApplication input callback)
// ---------------------------------------------------------------------------

bool CUITextField::HandleKeyEvent(int keyCode, int /*action*/)
{
    // Simple ASCII mapping for common keycodes
    char ch = 0;

    if (keyCode >= 7 && keyCode <= 16)
    {
        // AKEYCODE_0 through AKEYCODE_9
        ch = '0' + (keyCode - 7);
    }
    else if (keyCode >= 29 && keyCode <= 54)
    {
        // AKEYCODE_A through AKEYCODE_Z
        ch = 'a' + (keyCode - 29);
    }
    else
    {
        switch (keyCode)
        {
        case 62: ch = ' '; break;   // SPACE
        case 55: ch = ','; break;   // COMMA
        case 56: ch = '.'; break;   // PERIOD
        case 69: ch = '-'; break;   // MINUS
        case 70: ch = '='; break;   // EQUALS
        case 76: ch = '/'; break;   // SLASH
        case 68: ch = '\''; break;  // APOSTROPHE
        case 75: ch = '!'; break;   // (approximate via shift+1 on some keyboards)
        default: break;
        }
    }

    if (ch != 0)
    {
        _text += ch;
        if (_onTextChanged) _onTextChanged(_text);
        AndroidWindow_RequestRender();
        return true;
    }

    // Backspace (DEL = 67)
    if (keyCode == 67)
    {
        if (_text.Len() > 0)
        {
            _text = _text.SubString(0, _text.Len() - 1);
            if (_onTextChanged) _onTextChanged(_text);
            AndroidWindow_RequestRender();
        }
        return true;
    }

    // Enter (66) — dismiss keyboard
    if (keyCode == 66)
    {
        SetFocused(false);
        return true;
    }

    return false;
}
