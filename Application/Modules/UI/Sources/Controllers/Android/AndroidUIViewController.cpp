#include "Controllers/UIViewController.h"

#include "Views/UIView.h"
#include "SmartPointer/MakeAndCasts.h"

// ---------------------------------------------------------------------------
// CUIViewController — Android implementation (minimal, no JNI).
// ---------------------------------------------------------------------------

CUIViewController::CUIViewController()
{
    _view = new CUIView();
}

CUIView* CUIViewController::View() const
{
    return _view;
}

void CUIViewController::SetView(CUIView* V)
{
    _view = V;
}
