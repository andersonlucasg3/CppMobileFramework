#pragma once

#include "Object/ClassMacros.h"

#include "Views/UIView.h"

#include "Graphics/Rects.h"
#include "Graphics/Colors.h"

FORWARD_DECLARE_OBJECT(UITextField);

using namespace UI::Graphics;

class CUITextField : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUITextField();
    UI_API CUITextField(const SRectF& TextFieldRect);
    UI_API ~CUITextField() override = default;

    UI_API void SetText(const CString& InText);
    UI_API const CString& Text() const;

    UI_API void SetPlaceholder(const CString& InPlaceholder);
    UI_API const CString& Placeholder() const;

    UI_API void SetTextColor(const SColorF& Color);
    UI_API SColorF TextColor() const;

    UI_API bool IsFocused() const;
    UI_API void SetFocused(bool bFocused);

    /// Callback invoked when the text changes (e.g. key typed).
    UI_API void SetOnTextChanged(TFunction<void(const CString&)>&& Callback);

    UI_API virtual bool OnTouchEvent(EUITouchPhase Phase, SPointF Point) override;

    /// Called by the input system when a key event is received while focused.
    /// Returns true if the key was handled.
    UI_API bool HandleKeyEvent(int keyCode, int action);

protected:
    UI_API void OnAddedToSuperview() override;

private:
    CString _text;
    CString _placeholder;
    SColorF _textColor;
    bool    _bFocused = false;
    TFunction<void(const CString&)> _onTextChanged;

    friend void WindowsTextField_FireChanged(CUITextField*);
};