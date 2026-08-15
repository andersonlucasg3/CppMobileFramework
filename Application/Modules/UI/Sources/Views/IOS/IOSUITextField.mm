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

void CUITextField::OnAddedToSuperview() {}

void CUITextField::SetText(const CString& T) { _text = T; }
const CString& CUITextField::Text() const { return _text; }
void CUITextField::SetPlaceholder(const CString& P) { _placeholder = P; }
const CString& CUITextField::Placeholder() const { return _placeholder; }
void CUITextField::SetTextColor(const SColorF& C) { _textColor = C; }
SColorF CUITextField::TextColor() const { return _textColor; }
bool CUITextField::IsFocused() const { return _bFocused; }
void CUITextField::SetFocused(bool bF) { _bFocused = bF; }
void CUITextField::SetOnTextChanged(TFunction<void(const CString&)>&& CB) { _onTextChanged = std::move(CB); }
bool CUITextField::OnTouchEvent(EUITouchPhase, SPointF) { return false; }
bool CUITextField::HandleKeyEvent(int, int) { return false; }

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