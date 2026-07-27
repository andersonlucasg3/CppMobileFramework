#include "Application.h"
#include "Views/UIWindow.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Logger/Logger.h"
#include <Windows.h>

static CApplication* GSharedApp = nullptr;
CApplication* CApplication::SharedApp() { return GSharedApp; }

int CApplication::Run(int argc, char* argv[])
{
    GSharedApp = this;

    GLogger.Log("CApplication::Run - creating main window");
    CUIWindow* Window = CreateMainWindow();
    if (!Window) { GLogger.Error("CreateMainWindow returned null"); return 1; }
    GLogger.Log("CApplication::Run - calling DidLaunch");
    DidLaunch();

    GLogger.Log("CApplication::Run - entering message loop");
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GLogger.Log("CApplication::Run - exiting");
    return 0;
}