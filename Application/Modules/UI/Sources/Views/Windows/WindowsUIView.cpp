#include "Views/UIView.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Devices/UIScreen.h"

static bool GClassesRegistered = false;
void EnsureClasses()
{
    if (GClassesRegistered) return; GClassesRegistered = true;
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"CppUIView";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClassExW(&wc);
}

CUIView::CUIView()
{
    EnsureClasses();
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

CUIView::CUIView(const SRectF& R)
{
    EnsureClasses();
    _nativeInstance = MakeShared<CNativeInstance>((HWND)NULL);
}

CUIView::CUIView(CNativeViewController* Owner) { CUIView(); }

void CUIView::AddSubview(CUIView* S)
{
    _subviews.Add(S);
    S->_superview = this;

    HWND hParent = ViewHWND(this);
    if (!hParent) return;

    HWND hChild = ViewHWND(S);
    if (hChild)
    {
        SetParent(hChild, hParent);
    }
    else
    {
        S->OnAddedToSuperview();
    }
}

void CUIView::RemoveFromSuperview()
{
    if (_superview)
    {
        _superview->_subviews.Remove(this);
        _superview = nullptr;
    }
    HWND hWnd = ViewHWND(this);
    if (hWnd) ShowWindow(hWnd, SW_HIDE);
}

void CUIView::SetBackgroundColor()
{
    HWND hWnd = ViewHWND(this);
    if (hWnd) InvalidateRect(hWnd, NULL, TRUE);
}

SRectF CUIView::Frame() const
{
    HWND hWnd = ViewHWND(this);
    if (!hWnd) return SRectF(0,0,0,0);
    RECT r;
    GetWindowRect(hWnd, &r);
    return SRectF((Float)r.left, (Float)r.top, (Float)(r.right-r.left), (Float)(r.bottom-r.top));
}

void CUIView::SetFrame(const SRectF& F)
{
    HWND hWnd = ViewHWND(this);
    if (!hWnd) return;
    SetWindowPos(hWnd, NULL, (int)F.Origin.X, (int)F.Origin.Y,
        (int)F.Size.Width, (int)F.Size.Height, SWP_NOZORDER);
}

CUIView* CUIView::Superview() const { return _superview.Get(); }
void* CUIView::GetNativePointer() const { return _nativeInstance->Native(); }