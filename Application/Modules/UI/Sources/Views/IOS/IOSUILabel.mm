#include "Views/UILabel.h"
#include "Views/IOS/IOSUIView.h"
#include "Templates/IOS/CreateView.h"

CUILabel::CUILabel()
:   Super(CreateView<IOSView>(this))
{
    _textColor = SColorF::White();
}

CUILabel::CUILabel(const SRectF& LabelRect)
:   Super(CreateView<IOSView>(this, LabelRect))
{
    _textColor = SColorF::White();
}

void CUILabel::OnAddedToSuperview() {}
void CUILabel::SetText(const CString& T) { _text = T; }
const CString& CUILabel::Text() const { return _text; }
void CUILabel::SetTextAlignment(ETextAlignment A) { _alignment = A; }
ETextAlignment CUILabel::TextAlignment() const { return _alignment; }
void CUILabel::SetTextColor(const SColorF& C) { _textColor = C; }
SColorF CUILabel::TextColor() const { return _textColor; }
void CUILabel::SetTextSize(Float S) { _textSize = S; }
Float CUILabel::TextSize() const { return _textSize; }
