#include "Views/UIScrollView.h"

#include "AndroidNative.h"

#include "SmartPointer/MakeAndCasts.h"

extern void AndroidWindow_RequestRender();

// ---------------------------------------------------------------------------
void CUIScrollView::OnAddedToSuperview()
{
    // No special native widget needed on Android
}

// ---------------------------------------------------------------------------
CUIScrollView::CUIScrollView()
{
    _nativeInstance = MakeShared<CNativeInstance>();
}

CUIScrollView::CUIScrollView(const SRectF& ScrollViewRect)
{
    _nativeInstance = MakeShared<CNativeInstance>();
    SetFrame(ScrollViewRect);
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
    AndroidWindow_RequestRender();
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

            // Natural scrolling: content follows the finger — dragging down
            // decreases the offset, dragging up increases it.
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

// ---------------------------------------------------------------------------
// RepositionChildren — no-op on Android (reposition happens during Render)
// ---------------------------------------------------------------------------
void CUIScrollView::RepositionChildren()
{
}
