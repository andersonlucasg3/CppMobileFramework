#pragma once

#include "Object/ClassMacros.h"

#include "Views/UIView.h"
#include "Graphics/Rects.h"

FORWARD_DECLARE_OBJECT(UILabel);

using namespace UI::Graphics;

enum class ETextAlignment : uint8_t
{
    Left,
    Center,
    Right,
};

class CUILabel : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUILabel();
    UI_API CUILabel(const SRectF& LabelRect);
    UI_API ~CUILabel() override = default;

    UI_API void SetText(const CString& InText);
    UI_API const CString& Text() const;

    UI_API void SetTextAlignment(ETextAlignment InAlignment);
    UI_API ETextAlignment TextAlignment() const;

protected:
    UI_API void OnAddedToSuperview() override;

private:
    CString _text;
    ETextAlignment _alignment = ETextAlignment::Left;
};