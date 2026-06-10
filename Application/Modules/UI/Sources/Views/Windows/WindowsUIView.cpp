#include "Views/UIView.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Devices/UIScreen.h"

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProc(h,m,w,l); }
static bool GClassesRegistered = false;
static void EnsureClasses()
{
    if (GClassesRegistered) return; GClassesRegistered = true;
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = WndProc; wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"CppUIView"; wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClassExW(&wc);
}

CUIView::CUIView()
{
    EnsureClasses();
    SRectF B = CUIScreen::MainScreen()->Bounds();
    HWND h = CreateWindowExW(0, L"CppUIView", L"", WS_CHILD|WS_VISIBLE, 0,0,(int)B.Size.Width,(int)B.Size.Height, NULL,NULL,GetModuleHandle(NULL),NULL);
    GLogger.Log("CUIView::CUIView() hwnd={}", (void*)h);
    _nativeInstance = MakeShared<CNativeInstance>(h);
}

CUIView::CUIView(const SRectF& R)
{
    EnsureClasses();
    HWND h = CreateWindowExW(0, L"CppUIView", L"", WS_CHILD|WS_VISIBLE, (int)R.Origin.X,(int)R.Origin.Y,(int)R.Size.Width,(int)R.Size.Height, NULL,NULL,GetModuleHandle(NULL),NULL);
    GLogger.Log("CUIView::CUIView(R) hwnd={}", (void*)h);
    _nativeInstance = MakeShared<CNativeInstance>(h);
}

CUIView::CUIView(CNativeViewController* Owner) { CUIView(); }

void CUIView::AddSubview(CUIView* S) { _subviews.Add(S); S->_superview=this; SetParent(ViewHWND(S),ViewHWND(this)); }
void CUIView::RemoveFromSuperview() { if(_superview){_superview->_subviews.Remove(this);_superview=nullptr;} ShowWindow(ViewHWND(this),SW_HIDE); }
void CUIView::SetBackgroundColor() { InvalidateRect(ViewHWND(this),NULL,TRUE); }
SRectF CUIView::Frame() const { RECT r; GetWindowRect(ViewHWND(this),&r); return SRectF((Float)r.left,(Float)r.top,(Float)(r.right-r.left),(Float)(r.bottom-r.top)); }
void CUIView::SetFrame(const SRectF& F) { SetWindowPos(ViewHWND(this),NULL,(int)F.Origin.X,(int)F.Origin.Y,(int)F.Size.Width,(int)F.Size.Height,SWP_NOZORDER); }
CUIView* CUIView::Superview() const { return _superview.Get(); }
void* CUIView::GetNativePointer() const { return _nativeInstance->Native(); }