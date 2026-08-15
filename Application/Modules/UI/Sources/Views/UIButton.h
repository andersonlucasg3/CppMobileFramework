#pragma once

#include "Object/ClassMacros.h"

#include "Views/UIView.h"
#include "Graphics/Rects.h"
#include "Graphics/Colors.h"

#include "Templates/Functions.h"

FORWARD_DECLARE_OBJECT(UIButton);

using namespace UI::Graphics;

class CUIButton : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIButton();
    UI_API CUIButton(const SRectF& Rect);
    UI_API ~CUIButton() override = default;

    UI_API void SetText(const CString& InText);
    UI_API const CString& Text() const;

    UI_API void SetTextColor(const SColorF& Color);
    UI_API SColorF TextColor() const;

    UI_API void SetOnClickListener(TFunction<void()>&& Callback);

    UI_API virtual bool OnTouchEvent(EUITouchPhase Phase, SPointF Point) override;

protected:
    UI_API void OnAddedToSuperview() override;

private:
    CString _text;
    SColorF _textColor;
    TFunction<void()> _onClick;
    bool _bPressed = false;

    friend class CUIWindow;
    friend void WindowsButton_FireClick(CUIButton*);
};
