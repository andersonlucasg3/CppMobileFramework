#include "Views/UIWindow.h"
#include "WindowsNative.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Devices/UIScreen.h"
#include "Controllers/UIViewController.h"

static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

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
    _nativeInstance = MakeShared<CNativeInstance>(h);
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
}

void CUIWindow::SetRootViewController(CUIViewController* VC)
{
    _rootViewController = VC;
    VC->ViewDidLoad();
    if (CUIView* V = VC->View())
    {
        RECT rc; GetClientRect(ViewHWND(this), &rc);
        V->SetFrame(SRectF(0,0,(Float)(rc.right-rc.left),(Float)(rc.bottom-rc.top)));
    }
}