#include "Views/UIScrollView.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"

// Forward: defined in WindowsUIView.cpp
extern void EnsureClasses();

// ---------------------------------------------------------------------------
CUIScrollView::CUIScrollView()
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

CUIScrollView::CUIScrollView(const SRectF& ScrollViewRect)
{
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

void CUIScrollView::OnAddedToSuperview()
{
    if (ViewHWND(this)) return;
    CUIView* P = Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 400; F.Size.Height = 400; }

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
}

// ---------------------------------------------------------------------------
SPointF CUIScrollView::ContentOffset() const
{
    return _contentOffset;
}

void CUIScrollView::SetContentOffset(SPointF Offset)
{
    // Clamp to valid range
    SRectF f = Frame();
    float maxX = _contentSize.Width - f.Size.Width;
    float maxY = _contentSize.Height - f.Size.Height;
    if (maxX < 0.0f) maxX = 0.0f;
    if (maxY < 0.0f) maxY = 0.0f;
    if (Offset.X < 0.0f) Offset.X = 0.0f;
    if (Offset.Y < 0.0f) Offset.Y = 0.0f;
    if (Offset.X > maxX) Offset.X = maxX;
    if (Offset.Y > maxY) Offset.Y = maxY;

    _contentOffset = Offset;
    RepositionChildren();
}

SSizeF CUIScrollView::ContentSize() const
{
    return _contentSize;
}

void CUIScrollView::SetContentSize(SSizeF Size)
{
    _contentSize = Size;
}

// ---------------------------------------------------------------------------
void CUIScrollView::RepositionChildren()
{
    float ox = -_contentOffset.X;
    float oy = -_contentOffset.Y;

    for (int i = 0; i < (int)_subviews.Num(); ++i)
    {
        CUIView* child = _subviews[i];
        if (!child) continue;
        HWND hChild = ViewHWND(child);
        if (!hChild) continue;

        // Use the STORED logical frame (set via SetFrame), NOT the live HWND
        // rect — reading the live rect here would compound the offset on
        // every call and push children off-screen.
        SRectF f = child->_nativeInstance->GetFrame();
        SetWindowPos(hChild, NULL,
            (int)(f.Origin.X + ox),
            (int)(f.Origin.Y + oy),
            (int)f.Size.Width, (int)f.Size.Height,
            SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
    }

    // Force redraw of scroll view and all children
    HWND hSelf = ViewHWND(this);
    if (hSelf)
    {
        InvalidateRect(hSelf, NULL, TRUE);
        UpdateWindow(hSelf);
    }
}

// ---------------------------------------------------------------------------
bool CUIScrollView::OnTouchEvent(EUITouchPhase Phase, SPointF Point)
{
    switch (Phase)
    {
    case EUITouchPhase::Began:
        _bDragging = true;
        _dragStartPoint = Point;
        _dragStartOffset = _contentOffset;
        return true;

    case EUITouchPhase::Moved:
        if (_bDragging)
        {
            float dx = Point.X - _dragStartPoint.X;
            float dy = Point.Y - _dragStartPoint.Y;

            float newOx = _dragStartOffset.X - dx;
            float newOy = _dragStartOffset.Y - dy;

            SetContentOffset(SPointF(newOx, newOy));
            return true;
        }
        break;

    case EUITouchPhase::Ended:
    case EUITouchPhase::Cancelled:
        _bDragging = false;
        break;
    }

    return false;
}
