#include "Views/UIWindow.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Devices/UIScreen.h"
#include "Controllers/UIViewController.h"

#include "Views/UIButton.h"
#include "Views/UILabel.h"
#include "Views/UITextField.h"
#include "Views/UIScrollView.h"

#include "Logger/Logger.h"

#include <Windows.h>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
static CUIWindow* GMainWindow = nullptr;

// ---------------------------------------------------------------------------
// Helper: SColorF → COLORREF
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
// HitTest — recursive, deepest-first (same logic as Android)
// ---------------------------------------------------------------------------
CUIView* CUIWindow::HitTestView(CUIView* view, float x, float y, float ox, float oy)
{
    if (!view) return nullptr;

    SRectF f = view->Frame();
    float vx = f.Origin.X + ox;
    float vy = f.Origin.Y + oy;
    float vw = f.Size.Width;
    float vh = f.Size.Height;

    if (x < vx || y < vy || x >= vx + vw || y >= vy + vh)
        return nullptr;

    // Scroll view: adjust offset for children
    float cox = ox, coy = oy;
    if (CUIScrollView* sv = dynamic_cast<CUIScrollView*>(view))
    {
        SPointF co = sv->ContentOffset();
        cox -= co.X;
        coy -= co.Y;
    }

    // Children tested in reverse order (topmost first)
    for (int i = (int)view->_subviews.Num() - 1; i >= 0; --i)
    {
        CUIView* child = view->_subviews[i];
        CUIView* hit = HitTestView(child, x, y, cox, coy);
        if (hit) return hit;
    }

    // Return views that handle touch
    if (dynamic_cast<CUIButton*>(view) ||
        dynamic_cast<CUIScrollView*>(view) ||
        dynamic_cast<CUITextField*>(view))
        return view;

    return nullptr;
}

CUIView* CUIWindow::HitTest(SPointF Point)
{
    for (int i = (int)_subviews.Num() - 1; i >= 0; --i)
    {
        CUIView* child = _subviews[i];
        CUIView* hit = HitTestView(child, Point.X, Point.Y, 0.0f, 0.0f);
        if (hit) return hit;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// Touch capture for drag operations
// ---------------------------------------------------------------------------
static CUIView* GTouchCapture = nullptr;
static SPointF  GLastTouchPoint;
static bool     GbMouseCaptured = false;

static void DispatchTouch(HWND /*hWnd*/, EUITouchPhase Phase, float X, float Y)
{
    SPointF pt(X, Y);

    if (Phase == EUITouchPhase::Began)
    {
        if (!GMainWindow) return;
        CUIView* hit = GMainWindow->HitTest(pt);
        if (hit)
        {
            GTouchCapture = hit;
            GLastTouchPoint = pt;
            hit->OnTouchEvent(Phase, pt);
        }
    }
    else if (GTouchCapture)
    {
        GTouchCapture->OnTouchEvent(Phase, pt);
        GLastTouchPoint = pt;

        if (Phase == EUITouchPhase::Ended || Phase == EUITouchPhase::Cancelled)
            GTouchCapture = nullptr;
    }
}

// ---------------------------------------------------------------------------
// Main window procedure
// ---------------------------------------------------------------------------
static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // ----- Mouse input → touch model -----
    switch (msg)
    {
    case WM_SIZE:
    {
        int cw = (int)(short)LOWORD(lParam);
        int ch = (int)(short)HIWORD(lParam);
        if (GMainWindow)
            GMainWindow->HandleResize(cw, ch);
        break; // let DefWindowProc handle the rest
    }
    case WM_LBUTTONDOWN:
    {
        SetCapture(hWnd);
        GbMouseCaptured = true;
        int mx = (int)(short)LOWORD(lParam);
        int my = (int)(short)HIWORD(lParam);
        DispatchTouch(hWnd, EUITouchPhase::Began, (float)mx, (float)my);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (GbMouseCaptured && GTouchCapture)
        {
            int mx = (int)(short)LOWORD(lParam);
            int my = (int)(short)HIWORD(lParam);
            DispatchTouch(hWnd, EUITouchPhase::Moved, (float)mx, (float)my);
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        if (GbMouseCaptured)
        {
            ReleaseCapture();
            GbMouseCaptured = false;
            int mx = (int)(short)LOWORD(lParam);
            int my = (int)(short)HIWORD(lParam);
            DispatchTouch(hWnd, EUITouchPhase::Ended, (float)mx, (float)my);
        }
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        if (GMainWindow)
            GMainWindow->HandleWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        return 0;
    }
    case WM_CANCELMODE:
        if (GbMouseCaptured) { ReleaseCapture(); GbMouseCaptured = false; }
        if (GTouchCapture)
        {
            DispatchTouch(hWnd, EUITouchPhase::Cancelled, GLastTouchPoint.X, GLastTouchPoint.Y);
        }
        return 0;
    }

    // ----- WM_DRAWITEM for owner-draw buttons -----
    if (msg == WM_DRAWITEM)
    {
        LPDRAWITEMSTRUCT ds = (LPDRAWITEMSTRUCT)lParam;
        if (ds->CtlType == ODT_BUTTON)
        {
            CUIView* v = ViewGetUserData(ds->hwndItem);
            if (CUIButton* btn = dynamic_cast<CUIButton*>(v))
            {
                HDC hdc = ds->hDC;
                RECT& r = ds->rcItem;
                bool bPressed = (ds->itemState & ODS_SELECTED) != 0;

                // Background
                HBRUSH hBr = GetHWNDBrush(ds->hwndItem);
                if (hBr)
                    FillRect(hdc, &r, hBr);
                else
                    FillRect(hdc, &r, (HBRUSH)(COLOR_BTNFACE + 1));

                if (bPressed)
                {
                    // Darken slightly for pressed state
                    RECT r2 = r;
                    InflateRect(&r2, -2, -2);
                    HBRUSH hDark = CreateSolidBrush(RGB(0, 0, 0));
                    if (hDark)
                    {
                        // 20% alpha-like dark overlay via a pattern brush won't work simply.
                        // Instead, just draw a thin darker border.
                        DeleteObject(hDark);
                    }
                    // Simple: draw a 2px inset border
                    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
                    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
                    HBRUSH oldBr = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    Rectangle(hdc, r.left, r.top, r.right, r.bottom);
                    SelectObject(hdc, oldBr);
                    SelectObject(hdc, oldPen);
                    DeleteObject(hPen);
                }
                else
                {
                    // Normal: draw a 2px lighter top/left border
                    HPEN hPenLight = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
                    HPEN hPenDark  = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                    HPEN oldPen = (HPEN)SelectObject(hdc, hPenLight);
                    // Top edge
                    MoveToEx(hdc, r.left, r.bottom - 1, NULL);
                    LineTo(hdc, r.left, r.top);
                    LineTo(hdc, r.right - 1, r.top);
                    SelectObject(hdc, hPenDark);
                    LineTo(hdc, r.right - 1, r.bottom - 1);
                    LineTo(hdc, r.left, r.bottom - 1);
                    SelectObject(hdc, oldPen);
                    DeleteObject(hPenLight);
                    DeleteObject(hPenDark);
                }

                // Draw text centered
                const CString& t = btn->Text();
                if (t.Len() > 0)
                {
                    SetBkMode(hdc, TRANSPARENT);
                    COLORREF cr = SColorFToCOLORREF(btn->TextColor());
                    SetTextColor(hdc, cr);

                    // Create font matching button height
                    int fontSize = (r.bottom - r.top) / 3;
                    if (fontSize < 12) fontSize = 12;
                    if (fontSize > 48) fontSize = 48;
                    HFONT hFont = CreateFontA(-fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, nullptr);
                    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

                    RECT tr = r;
                    if (bPressed) { tr.top += 1; tr.left += 1; }
                    DrawTextA(hdc, *t, -1, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                    SelectObject(hdc, oldFont);
                    DeleteObject(hFont);
                }

                return TRUE; // handled
            }
        }
    }

    // ----- WM_COMMAND from child controls -----
    if (msg == WM_COMMAND)
    {
        HWND hCtrl = (HWND)lParam;
        WORD code  = HIWORD(wParam);
        CUIView* v = ViewGetUserData(hCtrl);

        if (v)
        {
            // CUIButton → BN_CLICKED
            if (CUIButton* btn = dynamic_cast<CUIButton*>(v))
            {
                if (code == BN_CLICKED)
                {
                    // Access private _onClick via friend
                    extern void WindowsButton_FireClick(CUIButton*);
                    WindowsButton_FireClick(btn);
                }
            }

            // CUITextField → EN_CHANGE
            if (CUITextField* tf = dynamic_cast<CUITextField*>(v))
            {
                if (code == EN_CHANGE)
                {
                    extern void WindowsTextField_FireChanged(CUITextField*);
                    WindowsTextField_FireChanged(tf);
                }
            }
        }
        return 0; // Let DefWindowProc handle it too
    }

    // ----- WM_CTLCOLOR* for styling -----
    if (msg == WM_CTLCOLORSTATIC || msg == WM_CTLCOLORBTN || msg == WM_CTLCOLOREDIT)
    {
        HDC hdc = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;
        CUIView* v = ViewGetUserData(hCtrl);
        if (v)
        {
            HBRUSH hBr = GetHWNDBrush(hCtrl);
            if (hBr)
            {
                SetBkMode(hdc, TRANSPARENT);

                // Text color
                if (CUILabel* lbl = dynamic_cast<CUILabel*>(v))
                {
                    COLORREF cr = SColorFToCOLORREF(lbl->TextColor());
                    SetTextColor(hdc, cr);
                }
                else if (CUITextField* tf = dynamic_cast<CUITextField*>(v))
                {
                    COLORREF cr = SColorFToCOLORREF(tf->TextColor());
                    SetTextColor(hdc, cr);
                }

                if (msg == WM_CTLCOLORSTATIC || msg == WM_CTLCOLOREDIT)
                    SetBkColor(hdc, SColorFToCOLORREF(v->BackgroundColor()));

                return (LRESULT)hBr;
            }
        }
    }

    // ----- WM_ERASEBKGND for plain CUIView containers -----
    if (msg == WM_ERASEBKGND)
    {
        CUIView* v = ViewGetUserData(hWnd);
        if (v)
        {
            HBRUSH hBr = GetHWNDBrush(hWnd);
            if (hBr)
            {
                RECT rc; GetClientRect(hWnd, &rc);
                FillRect((HDC)wParam, &rc, hBr);
                return 1; // handled
            }
        }
    }

    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
// CUIWindow constructors
// ---------------------------------------------------------------------------
CUIWindow::CUIWindow()
{
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = MainWndProc; wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = L"CppUIWindow";
    RegisterClassExW(&wc);

    SRectF B = CUIScreen::MainScreen()->Bounds();
    HWND h = CreateWindowExW(0, L"CppUIWindow", L"CppMobileFramework",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT,0,(int)B.Size.Width,(int)B.Size.Height,
        NULL,NULL,GetModuleHandle(NULL),NULL);

    if (!h)
    {
        MessageBoxA(NULL, "CreateWindowExW failed", "CppMobileFramework Error", MB_OK | MB_ICONERROR);
        return;
    }

    _nativeInstance = MakeShared<CNativeInstance>(h);
    ViewSetUserData(h, this);
    GMainWindow = this;
    ShowWindow(h, SW_SHOW);
    UpdateWindow(h);
}

CUIWindow::CUIWindow(const SRectF& R)
{
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = MainWndProc; wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = L"CppUIWindow";
    RegisterClassExW(&wc);

    HWND h = CreateWindowExW(0, L"CppUIWindow", L"CppMobileFramework",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE, (int)R.Origin.X,(int)R.Origin.Y,(int)R.Size.Width,(int)R.Size.Height,
        NULL,NULL,GetModuleHandle(NULL),NULL);
    _nativeInstance = MakeShared<CNativeInstance>(h);
    ViewSetUserData(h, this);
    GMainWindow = this;
}

// ---------------------------------------------------------------------------
// SetRootViewController
// ---------------------------------------------------------------------------
void CUIWindow::SetRootViewController(CUIViewController* VC)
{
    _rootViewController = VC;
    if (!VC) return;

    HWND hWnd = ViewHWND(this);
    if (!hWnd) return;

    CUIView* V = VC->View();
    if (!V) return;

    AddSubview(V);
    RECT rc; GetClientRect(hWnd, &rc);
    V->SetFrame(SRectF(0,0,(Float)(rc.right-rc.left),(Float)(rc.bottom-rc.top)));

    VC->ViewDidLoad();
}

// ---------------------------------------------------------------------------
// Render (no-op on Windows)
// ---------------------------------------------------------------------------
void CUIWindow::Render() {}

// ---------------------------------------------------------------------------
// HandleWheel — called on WM_MOUSEWHEEL to scroll the content
// ---------------------------------------------------------------------------
void CUIWindow::HandleWheel(short zDelta)
{
    if (!_rootViewController) return;
    CUIView* rootView = _rootViewController->View();
    if (!rootView || rootView->_subviews.Num() == 0) return;

    CUIScrollView* sv = dynamic_cast<CUIScrollView*>(rootView->_subviews[0]);
    if (!sv) return;

    SPointF off = sv->ContentOffset();
    off.Y -= (float)zDelta / 3.0f;
    sv->SetContentOffset(off);
}
void CUIWindow::HandleResize(int clientWidth, int clientHeight)
{
    // Resize the root VC view to fill the new client area
    CUIView* rootView = _rootViewController ? _rootViewController->View() : nullptr;
    if (rootView)
    {
        rootView->SetFrame(SRectF(0, 0, (Float)clientWidth, (Float)clientHeight));

        // If the root view has a CUIScrollView as first child, update its frame too
        if (rootView->_subviews.Num() > 0)
        {
            CUIView* firstChild = rootView->_subviews[0];
            if (CUIScrollView* sv = dynamic_cast<CUIScrollView*>(firstChild))
            {
                sv->SetFrame(SRectF(0, 0, (Float)clientWidth, (Float)clientHeight));
                // Update content width to match new width
                SSizeF cs = sv->ContentSize();
                cs.Width = (Float)clientWidth;
                sv->SetContentSize(cs);
                // Clamp offset after resize (SetContentOffset already clamps)
                sv->SetContentOffset(sv->ContentOffset());
            }
        }
    }
}
