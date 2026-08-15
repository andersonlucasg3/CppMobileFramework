#include "Views/UIStackView.h"

#include "AndroidNative.h"

#include "SmartPointer/MakeAndCasts.h"

extern void AndroidWindow_RequestRender();

// ---------------------------------------------------------------------------
CUIStackView::CUIStackView()
{
    _nativeInstance = MakeShared<CNativeInstance>();
}

CUIStackView::CUIStackView(const SRectF& StackViewRect)
{
    _nativeInstance = MakeShared<CNativeInstance>();
    SetFrame(StackViewRect);
}

// ---------------------------------------------------------------------------
void CUIStackView::OnAddedToSuperview()
{
    LayoutArrangedSubviews();
    AndroidWindow_RequestRender();
}

// ---------------------------------------------------------------------------
void CUIStackView::AddArrangedSubview(CUIView* Subview)
{
    AddSubview(Subview);
    LayoutArrangedSubviews();
}

void CUIStackView::RemoveArrangedSubview(CUIView* Subview)
{
    if (Subview)
        Subview->RemoveFromSuperview();
    LayoutArrangedSubviews();
}

// ---------------------------------------------------------------------------
void CUIStackView::SetSpacing(Float Spacing)
{
    _spacing = Spacing;
    LayoutArrangedSubviews();
}

Float CUIStackView::Spacing() const
{
    return _spacing;
}

// ---------------------------------------------------------------------------
void CUIStackView::LayoutArrangedSubviews()
{
    SRectF f = Frame();
    float x = f.Origin.X;
    float y = f.Origin.Y;
    float w = f.Size.Width;

    int n = (int)_subviews.Num();
    if (n == 0) return;

    // Compute total height: use each child's current height, or equal division
    float totalChildH = 0.0f;
    bool hasHeights = true;
    for (int i = 0; i < n; ++i)
    {
        CUIView* child = _subviews[i];
        if (!child) continue;
        float ch = child->Frame().Size.Height;
        if (ch <= 0.0f) { hasHeights = false; break; }
        totalChildH += ch;
    }

    if (!hasHeights)
    {
        // Equal distribution: fill the stack height minus spacing
        float avail = f.Size.Height - _spacing * (float)(n - 1);
        float each = avail / (float)n;
        if (each < 0.0f) each = 0.0f;
        float cy = y;
        for (int i = 0; i < n; ++i)
        {
            CUIView* child = _subviews[i];
            if (!child) continue;
            child->SetFrame(SRectF(x, cy, w, each));
            cy += each + _spacing;
        }
    }
    else
    {
        float cy = y;
        for (int i = 0; i < n; ++i)
        {
            CUIView* child = _subviews[i];
            if (!child) continue;
            float ch = child->Frame().Size.Height;
            child->SetFrame(SRectF(x, cy, w, ch));
            cy += ch + _spacing;
        }
    }
}
