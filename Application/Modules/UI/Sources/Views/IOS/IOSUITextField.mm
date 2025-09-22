#include "IOSUITextField.h"

#include "Templates/IOS/CreateView.h"

#include "Views/UITextField.h"

CUITextField::CUITextField()
:   Super(CreateView<IOSTextField>(this))
{

}

CUITextField::CUITextField(const SRectF& TextFieldRect)
:   Super(CreateView<IOSTextField>(this, TextFieldRect))
{

}

@implementation IOSTextField
{
    @public CUITextFieldWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUITextField *)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

@end