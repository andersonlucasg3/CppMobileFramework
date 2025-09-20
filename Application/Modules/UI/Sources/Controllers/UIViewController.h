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

    UI_API virtual void ViewDidLoad();

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