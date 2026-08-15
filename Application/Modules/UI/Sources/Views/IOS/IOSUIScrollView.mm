#include "IOSUIScrollView.h"

#include "Templates/IOS/CreateView.h"
#include "Views/IOS/IOSUIView.h"
#include "Views/UIScrollView.h"

#include "Graphics/IOS/IOSGraphics.h"

#include <UIKit/UIKit.h>

CUIScrollView::CUIScrollView()
:   Super(CreateView<IOSScrollView>(this))
{
    
}

CUIScrollView::CUIScrollView(const SRectF& ScrollViewRect)
:   Super(CreateView<IOSScrollView>(this, ScrollViewRect))
{
    
}

void CUIScrollView::OnAddedToSuperview() {}

SPointF CUIScrollView::ContentOffset() const { return _contentOffset; }
void CUIScrollView::SetContentOffset(SPointF O) { _contentOffset = O; }
SSizeF CUIScrollView::ContentSize() const { return _contentSize; }
void CUIScrollView::SetContentSize(SSizeF S) { _contentSize = S; }
bool CUIScrollView::OnTouchEvent(EUITouchPhase, SPointF) { return false; }
void CUIScrollView::RepositionChildren() {}

@implementation IOSScrollView
{
    @public CUIScrollViewWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUIScrollView *)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

@end