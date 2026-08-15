#include "Views/UIView.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Devices/UIScreen.h"

#include <unordered_map>

// ---------------------------------------------------------------------------
// Per-HWND brush color storage (store COLORREF, create brush on demand)
// ---------------------------------------------------------------------------
static std::unordered_map<HWND, COLORREF> GBrushColorMap;

void SetHWNDBrushColor(HWND h, COLORREF cr) { GBrushColorMap[h] = cr; }
HBRUSH GetHWNDBrush(HWND h) {
    auto it = GBrushColorMap.find(h);
    if (it != GBrushColorMap.end())
        return CreateSolidBrush(it->second);
    return nullptr;
}
void RemoveHWNDBrush(HWND h) {
    GBrushColorMap.erase(h);
}

// ---------------------------------------------------------------------------
// Forward: CppUIView window proc (defined here, used by EnsureClasses)
// ---------------------------------------------------------------------------
static LRESULT CALLBACK CppUIViewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ---------------------------------------------------------------------------
// CppUIView window class — custom WndProc for WM_ERASEBKGND and forwarding
// WM_CTLCOLOR* / WM_DRAWITEM to the root CppUIWindow.
// ---------------------------------------------------------------------------
static bool GClassesRegistered = false;
void EnsureClasses()
{
    if (GClassesRegistered) return; GClassesRegistered = true;
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = CppUIViewWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"CppUIView";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClassExW(&wc);
}

// ---------------------------------------------------------------------------
// Helper: convert SColorF → COLORREF
// ---------------------------------------------------------------------------
static COLORREF SColorFToCOLORREF(const SColorF& c)
{
    return RGB(
        (int)(c.R() * 255.0f),
        (int)(c.G() * 255.0f),
        (int)(c.B() * 255.0f)
    );
}

// ---------------------------------------------------------------------------
// CppUIView window procedure
// ---------------------------------------------------------------------------
static LRESULT CALLBACK CppUIViewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_ERASEBKGND:
    {
        HBRUSH hBr = GetHWNDBrush(hWnd);
        if (hBr)
        {
            RECT rc; GetClientRect(hWnd, &rc);
            FillRect((HDC)wParam, &rc, hBr);
            return 1;
        }
        break;
    }

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_DRAWITEM:
    case WM_COMMAND:
    {
        HWND hRoot = GetAncestor(hWnd, GA_ROOT);
        if (hRoot && hRoot != hWnd)
            return SendMessageA(hRoot, msg, wParam, lParam);
        break;
    }

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MOUSEMOVE:
    {
        HWND hRoot = GetAncestor(hWnd, GA_ROOT);
        if (hRoot && hRoot != hWnd)
        {
            POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
            MapWindowPoints(hWnd, hRoot, &pt, 1);
            return SendMessageA(hRoot, msg, wParam, MAKELPARAM(pt.x, pt.y));
        }
        break;
    }

    case WM_MOUSEWHEEL:
    {
        HWND hRoot = GetAncestor(hWnd, GA_ROOT);
        if (hRoot && hRoot != hWnd)
            return SendMessageA(hRoot, msg, wParam, lParam);
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// OnAddedToSuperview — create a plain CppUIView container HWND for base views.
// Subclasses (CUILabel, CUIButton, etc.) override this to create their specific HWNDs.
// ---------------------------------------------------------------------------
void CUIView::OnAddedToSuperview()
{
    if (ViewHWND(this)) return;
    CUIView* P = Superview();
    if (!P) return;
    HWND hParent = ViewHWND(P);
    if (!hParent) return;

    SRectF F = Frame();
    if (F.Size.Width <= 0) { F.Size.Width = 100; F.Size.Height = 50; }

    EnsureClasses();
    HWND h = CreateWindowExW(0, L"CppUIView", L"",
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        (int)F.Origin.X, (int)F.Origin.Y, (int)F.Size.Width, (int)F.Size.Height,
        hParent, NULL, GetModuleHandle(NULL), NULL);

    _nativeInstance->SetHWND(h);
    ViewSetUserData(h, this);

    // Propagate brush color if already set
    COLORREF cr = _nativeInstance->GetBgColorRef();
    if (cr != 0xFFFFFFFF) {  // Check if a color was actually set
        SetHWNDBrushColor(h, cr);
        InvalidateRect(h, NULL, TRUE);
        UpdateWindow(h);
    }
}

// ---------------------------------------------------------------------------
// Add / Remove subview
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Background color — default (grey)
// ---------------------------------------------------------------------------
void CUIView::SetBackgroundColor()
{
    SetBackgroundColor(SColorF::Gray());
}

// ---------------------------------------------------------------------------
// Background color — SColorF overload (creates real brush)
// ---------------------------------------------------------------------------
void CUIView::SetBackgroundColor(const SColorF& Color)
{
    COLORREF cr = SColorFToCOLORREF(Color);
    HBRUSH hBr = CreateSolidBrush(cr);
    _nativeInstance->SetBgBrush(hBr);
    _nativeInstance->SetBgColorRef(cr);

    HWND hWnd = ViewHWND(this);
    if (hWnd)
    {
        SetHWNDBrushColor(hWnd, cr);
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

SColorF CUIView::BackgroundColor() const
{
    // Return a default; tracking exact RGB back from brush is unnecessary
    return SColorF::Gray();
}

// ---------------------------------------------------------------------------
// Touch event (base — returns false)
// ---------------------------------------------------------------------------
bool CUIView::OnTouchEvent(EUITouchPhase /*Phase*/, SPointF /*Point*/)
{
    return false;
}

// ---------------------------------------------------------------------------
// Frame get/set — uses stored _frame in CNativeInstance so positions survive
// before and after HWND creation.
// ---------------------------------------------------------------------------
SRectF CUIView::Frame() const
{
    HWND hWnd = ViewHWND(this);
    if (hWnd)
    {
        RECT r;
        GetWindowRect(hWnd, &r);
        HWND hParent = GetParent(hWnd);
        if (hParent)
        {
            POINT pt = { r.left, r.top };
            ScreenToClient(hParent, &pt);
            return SRectF((Float)pt.x, (Float)pt.y,
                          (Float)(r.right - r.left), (Float)(r.bottom - r.top));
        }
        return SRectF((Float)r.left, (Float)r.top,
                      (Float)(r.right - r.left), (Float)(r.bottom - r.top));
    }
    // No HWND yet — return stored frame
    return _nativeInstance->GetFrame();
}

void CUIView::SetFrame(const SRectF& F)
{
    // Always store in CNativeInstance
    _nativeInstance->SetFrame(F);

    HWND hWnd = ViewHWND(this);
    if (hWnd)
    {
        SetWindowPos(hWnd, NULL, (int)F.Origin.X, (int)F.Origin.Y,
            (int)F.Size.Width, (int)F.Size.Height, SWP_NOZORDER);
    }
}

CUIView* CUIView::Superview() const { return _superview.Get(); }
void* CUIView::GetNativePointer() const { return _nativeInstance->Native(); }
