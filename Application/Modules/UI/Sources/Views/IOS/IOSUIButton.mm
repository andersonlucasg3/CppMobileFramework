#include "Views/IOS/IOSUIView.h"
#include "Views/UIButton.h"
#include "Templates/IOS/CreateView.h"

CUIButton::CUIButton()
:   Super(CreateView<IOSView>(this))
{
    _textColor = SColorF::White();
}

CUIButton::CUIButton(const SRectF& Rect)
:   Super(CreateView<IOSView>(this, Rect))
{
    _textColor = SColorF::White();
}

void CUIButton::OnAddedToSuperview() {}
void CUIButton::SetText(const CString& T) { _text = T; }
const CString& CUIButton::Text() const { return _text; }
void CUIButton::SetTextColor(const SColorF& C) { _textColor = C; }
SColorF CUIButton::TextColor() const { return _textColor; }
void CUIButton::SetOnClickListener(TFunction<void()>&& CB) { _onClick = std::move(CB); }
bool CUIButton::OnTouchEvent(EUITouchPhase, SPointF) { return false; }
