#include "Devices/UIScreen.h"
#include "SmartPointer/MakeAndCasts.h"
#include <Windows.h>

class CNativeScreen {};

static CUIScreen* GMainScreen = nullptr;

CUIScreen* CUIScreen::MainScreen()
{
    if (!GMainScreen)
    {
        GMainScreen = new CUIScreen();
        GMainScreen->_nativeScreen = MakeShared<CNativeScreen>();
    }
    return GMainScreen;
}

SRectF CUIScreen::Bounds() const
{
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    return SRectF(0, 0, (Float)w, (Float)h);
}

Float CUIScreen::Density() const
{
    Float density = (Float)GetDpiForSystem() / 96.0f;
    return density > 0.0f ? density : 1.0f;
}

CNativeScreen& CUIScreen::NativeScreen() const
{
    return *_nativeScreen;
}