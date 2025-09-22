#pragma once

#include "Object/ClassMacros.h"
#include "Object/Object.h"

#include "Views/UIView.h"

FORWARD_DECLARE_OBJECT(UIViewController);

class CNativeViewController;

class CUIViewController : public CObject
{
    using Super = CObject;

public:
    UI_API CUIViewController();
    UI_API virtual ~CUIViewController() = default;

    //// Begin Events
    UI_API virtual void ViewDidLoad() {}
    UI_API virtual void ViewWillUnload() {}
    UI_API virtual void ViewDidUnload() {}
    
    UI_API virtual void ViewWillAppear(bool bAnimated) {}
    UI_API virtual void ViewDidAppear(bool bAnimated) {}
    UI_API virtual void ViewWillDisappear(bool bAnimated) {}
    UI_API virtual void ViewDidDisappear(bool bAnimated) {}
    //// End Events

    UI_API CUIView* View() const;
    UI_API void SetView(CUIView* View);

    template<typename TNativeViewController>
    inline TNativeViewController* NativeInstance() const
    {
        return *_nativeViewController;
    }

protected:
    template<typename TNativeViewController>
    inline CUIViewController(TNativeViewController* View)
    :   Super()
    {
        _nativeViewController = MakeShared<CNativeViewController>(View);
    }

private:
    TSharedPtr<CNativeViewController> _nativeViewController;

    PROPERTY(CUIView, _view);
};