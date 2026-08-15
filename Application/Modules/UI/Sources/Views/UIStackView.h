#pragma once

#include "Object/ClassMacros.h"

#include "Views/UIView.h"

#include "Graphics/Rects.h"

FORWARD_DECLARE_OBJECT(UIStackView);

class CUIStackView : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIStackView();
    UI_API CUIStackView(const SRectF& StackViewRect);
    UI_API ~CUIStackView() override = default;

    UI_API void AddArrangedSubview(CUIView* Subview);
    UI_API void RemoveArrangedSubview(CUIView* Subview);

    UI_API void SetSpacing(Float Spacing);
    UI_API Float Spacing() const;

protected:
    UI_API void OnAddedToSuperview() override;

private:
    Float _spacing = 8.0f;
    UI_API void LayoutArrangedSubviews();
};