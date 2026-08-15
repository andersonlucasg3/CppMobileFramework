#include "Views/UIStackView.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"

// Forward: defined in WindowsUIView.cpp
extern void EnsureClasses();

// ---------------------------------------------------------------------------
CUIStackView::CUIStackView()
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

CUIStackView::CUIStackView(const SRectF& StackViewRect)
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

// ---------------------------------------------------------------------------
void CUIStackView::OnAddedToSuperview()
{
    if (ViewHWND(this)) return;
    CUIView* P = Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 200; F.Size.Height = 200; }

    EnsureClasses();
    HWND h = CreateWindowExW(0, L"CppUIView", L"",
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        (int)F.Origin.X, (int)F.Origin.Y, (int)F.Size.Width, (int)F.Size.Height,
        hParent, NULL, GetModuleHandle(NULL), NULL);

    _nativeInstance->SetHWND(h);
    ViewSetUserData(h, this);

    // Propagate brush
    COLORREF cr = _nativeInstance->GetBgColorRef();
    if (cr != 0xFFFFFFFF) SetHWNDBrushColor(h, cr);

    LayoutArrangedSubviews();
}

// ---------------------------------------------------------------------------
void CUIStackView::AddArrangedSubview(CUIView* Subview)
{
    AddSubview(Subview);
    LayoutArrangedSubviews();
}

void CUIStackView::RemoveArrangedSubview(CUIView* Subview)
{
    if (Subview) Subview->RemoveFromSuperview();
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
    float w = f.Size.Width;
    float y = 0.0f;

    int n = (int)_subviews.Num();
    if (n == 0) return;

    bool hasHeights = true;
    float totalChildH = 0.0f;
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
        float avail = f.Size.Height - _spacing * (float)(n - 1);
        float each = avail / (float)n;
        if (each < 0.0f) each = 0.0f;
        for (int i = 0; i < n; ++i)
        {
            CUIView* child = _subviews[i];
            if (!child) continue;
            child->SetFrame(SRectF(0, y, w, each));
            y += each + _spacing;
        }
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            CUIView* child = _subviews[i];
            if (!child) continue;
            float ch = child->Frame().Size.Height;
            child->SetFrame(SRectF(0, y, w, ch));
            y += ch + _spacing;
        }
    }
}
