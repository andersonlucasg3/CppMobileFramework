#pragma once

#include "Object/ClassMacros.h"
#include "Object/Object.h"

#include "Graphics/Rects.h"
#include "Graphics/Points.h"
#include "Graphics/Colors.h"

FORWARD_DECLARE_OBJECT(UIView);

class CNativeInstance;
class CNativeViewController;

using namespace UI::Graphics;

enum class EUITouchPhase : uint8_t
{
    Began,
    Moved,
    Ended,
    Cancelled,
};

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
    UI_API void SetBackgroundColor(const SColorF& Color);
    UI_API SColorF BackgroundColor() const;

    UI_API virtual bool OnTouchEvent(EUITouchPhase Phase, SPointF Point);

    UI_API SRectF Frame() const;
    UI_API void SetFrame(const SRectF& Frame);
    
    UI_API CUIView* Superview() const;

    UI_API virtual void OnAddedToSuperview();

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

public:
    UI_API void* GetNativePointer() const;

protected:
    ARRAYPROPERTY(CUIView, _subviews);
    CUIViewWeakObjectPtr _superview;
    TSharedPtr<CNativeInstance> _nativeInstance;

    friend class CUIViewController;
    friend class CUIWindow;
    friend class CUIStackView;
    friend class CUIScrollView;
};