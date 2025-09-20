#pragma once

#include "Object/ClassMacros.h"
#include "UIView.h"

FORWARD_DECLARE_OBJECT(UIScrollView);

class CUIScrollView : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIScrollView();
    UI_API CUIScrollView(const SRectF& ScrollViewRect);
    UI_API ~CUIScrollView() override = default;
};