#include "IOSUIView.h"

#include "Graphics/Rects.h"
#include "SmartPointer/MakeAndCasts.h"
#include "SmartPointer/SharedPointer.h"

#include "Controllers/IOS/IOSUIViewController.h"

#include "Templates/IOS/CreateView.h"
#include "Views/UIView.h"

#include "Devices/UIScreen.h"

#include "Graphics/IOS/IOSGraphics.h"

CUIView::CUIView()
:   CUIView(CreateView<IOSView>(this))
{

}

CUIView::CUIView(const SRectF& ViewRect)
:   CUIView(CreateView<IOSView>(this, ViewRect))
{

}

void CUIView::AddSubview(CUIView* Subview)
{
    _subviews.Add(Subview);
    Subview->_superview = this;

    [NativeInstance<IOSView>() addSubview:Subview->NativeInstance<IOSView>()];
}

void CUIView::RemoveFromSuperview()
{
    _superview->_subviews.Remove(this);
    _superview = nullptr;
    
    [NativeInstance<IOSView>() removeFromSuperview];
}

void CUIView::OnAddedToSuperview()
{
    // No-op on iOS — UIKit handles view hierarchy natively
}

void CUIView::SetBackgroundColor()
{
    // TODO: pass the CUIColor
    [NativeInstance<IOSView>() setBackgroundColor:[UIColor grayColor]];
}

void CUIView::SetBackgroundColor(const SColorF& Color)
{
    UIColor* c = [UIColor colorWithRed:(CGFloat)Color.R() green:(CGFloat)Color.G()
                                  blue:(CGFloat)Color.B() alpha:(CGFloat)Color.A()];
    [NativeInstance<IOSView>() setBackgroundColor:c];
}

SColorF CUIView::BackgroundColor() const
{
    return SColorF::Gray(); // Simplified
}

bool CUIView::OnTouchEvent(EUITouchPhase /*Phase*/, SPointF /*Point*/)
{
    return false;
}

SRectF CUIView::Frame() const
{
    return ToRectF([NativeInstance<IOSView>() frame]);
}

void CUIView::SetFrame(const SRectF& Frame)
{
    [NativeInstance<IOSView>() setFrame:ToCGRect(Frame)];
}

CUIView* CUIView::Superview() const
{
    return _superview.Get();
}

CUIView::CUIView(CNativeViewController* Owner)
:   Super()
{
    _nativeInstance = MakeShared<CNativeInstance>([*Owner view]);
}

void* CUIView::GetNativePointer() const
{
    return _nativeInstance->Native();
}

@implementation IOSView
{
    @public CUIViewWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUIView*)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

@end