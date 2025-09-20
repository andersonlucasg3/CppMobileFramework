#pragma once

#include "Object/ClassMacros.h"
#include "Object/Object.h"

#include "Graphics/Rects.h"

FORWARD_DECLARE_OBJECT(UIView);

class CNativeInstance;
class CNativeViewController;

using namespace UI::Graphics;

class CUIView : public CObject
{
    using Super = CObject;

public:
    UI_API CUIView();
    UI_API CUIView(const SRectF& ViewRect);
    UI_API ~CUIView() = default;

    UI_API void AddSubview(CUIView* Subview);
    UI_API void RemoveFromSuperview();

    UI_API void SetBackgroundColor();

    UI_API CUIView* Superview() const;

    template<typename TNativeView>
    TNativeView* NativeInstance() const
    {
        return (TNativeView*)GetNativePointer();
    }

protected:
    template<typename TNativeView>
    inline CUIView(TNativeView* View)
    :   Super()
    {
        _nativeInstance = MakeShared<CNativeInstance>(View);
    }

    CUIView(CNativeViewController* Owner);

    void* GetNativePointer() const;

private:
    TSharedPtr<CNativeInstance> _nativeInstance;

    CUIViewWeakObjectPtr _superview;
    ARRAYPROPERTY(CUIView, _subviews);

    friend class CUIViewController;
};