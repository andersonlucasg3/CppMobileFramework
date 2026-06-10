#include "Application.h"
#include "Views/UIWindow.h"
#include "SmartPointer/MakeAndCasts.h"
#include <Windows.h>

static CApplication* GSharedApp = nullptr;

CApplication* CApplication::SharedApp() { return GSharedApp; }

int CApplication::Run(int argc, char* argv[])
{
    GSharedApp = this;
    CUIWindow* Window = CreateMainWindow();
    DidLaunch();
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}