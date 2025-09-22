#pragma once

#include "Object/ClassMacros.h"

#include "Views/UIView.h"

#include "Graphics/Rects.h"

FORWARD_DECLARE_OBJECT(UITextField);

using namespace UI::Graphics;

// TODO: maybe add UIControl here
class CUITextField : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUITextField();
    UI_API CUITextField(const SRectF& TextFieldRect);
    UI_API ~CUITextField() override = default;
};