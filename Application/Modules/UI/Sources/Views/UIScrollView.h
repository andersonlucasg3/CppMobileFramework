#pragma once

#include "Object/ClassMacros.h"
#include "UIView.h"

#include "Graphics/Points.h"
#include "Graphics/Sizes.h"

FORWARD_DECLARE_OBJECT(UIScrollView);

class CUIScrollView : public CUIView
{
    using Super = CUIView;

public:
    UI_API CUIScrollView();
    UI_API CUIScrollView(const SRectF& ScrollViewRect);
    UI_API ~CUIScrollView() override = default;

    UI_API SPointF ContentOffset() const;
    UI_API void SetContentOffset(SPointF Offset);
    UI_API SSizeF ContentSize() const;
    UI_API void SetContentSize(SSizeF Size);

    UI_API virtual bool OnTouchEvent(EUITouchPhase Phase, SPointF Point) override;

protected:
    UI_API void OnAddedToSuperview() override;

private:
    SPointF _contentOffset;
    SSizeF _contentSize;
    SPointF _dragStartOffset;
    SPointF _dragStartPoint;
    bool    _bDragging = false;
    UI_API void RepositionChildren();
};