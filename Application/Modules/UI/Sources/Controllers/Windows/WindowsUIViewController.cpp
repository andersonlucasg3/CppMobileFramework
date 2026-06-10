#include "Controllers/UIViewController.h"
#include "SmartPointer/MakeAndCasts.h"

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