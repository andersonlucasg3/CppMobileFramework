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