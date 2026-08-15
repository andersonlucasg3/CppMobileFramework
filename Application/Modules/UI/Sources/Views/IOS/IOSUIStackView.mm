#include "IOSUIStackView.h"

#include "Templates/IOS/CreateView.h"

#include "Views/IOS/IOSUIView.h"
#include "Views/UIStackView.h"

#include "Graphics/Rects.h"

CUIStackView::CUIStackView()
:   Super(CreateView<IOSStackView>(this))
{

}

CUIStackView::CUIStackView(const SRectF& StackViewRect)
:   Super(CreateView<IOSStackView>(this, StackViewRect))
{

}

void CUIStackView::AddArrangedSubview(CUIView* Subview)
{
    _subviews.Add(Subview);
    Subview->_superview = this;

    [NativeInstance<IOSStackView>() addArrangedSubview:Subview->NativeInstance<IOSView>()];
}

void CUIStackView::RemoveArrangedSubview(CUIView* Subview)
{
    _subviews.Remove(Subview);
    Subview->_superview = nullptr;

    [NativeInstance<IOSStackView>() removeArrangedSubview:Subview->NativeInstance<IOSView>()];
}

void CUIStackView::SetSpacing(Float S) { _spacing = S; }
Float CUIStackView::Spacing() const { return _spacing; }
void CUIStackView::OnAddedToSuperview() {}
void CUIStackView::LayoutArrangedSubviews() {}

@implementation IOSStackView
{
    @public CUIStackViewWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUIStackView *)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

@end