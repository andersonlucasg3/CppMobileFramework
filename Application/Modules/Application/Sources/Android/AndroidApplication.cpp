#include "Application.h"

#include "AndroidJni.h"

#include "Preprocessors.h"
#include "Logger/Logger.h"
#include "Object/Collector/CollectorScope.h"
#include "SmartPointer/MakeAndCasts.h"
#include "Views/UIWindow.h"
#include "Views/UIView.h"

#include "native_app_glue/android_native_app_glue.h"
#include <android/input.h>

// ---------------------------------------------------------------------------
// Forward-declared render trigger (defined in the UI module's Android backend).
// ---------------------------------------------------------------------------
extern void AndroidWindow_RequestRender();

// Touch dispatch forward-declared (defined in AndroidUIWindow.cpp).
extern void AndroidWindow_DispatchTouch(EUITouchPhase Phase, float X, float Y);
extern void AndroidWindow_DispatchKeyEvent(int keyCode, int action);

// ---------------------------------------------------------------------------
// Static shared-app pointer (same pattern as Windows/IOS).
// ---------------------------------------------------------------------------
static CApplication* GSharedApp = nullptr;

CApplication* CApplication::SharedApp()
{
    return GSharedApp;
}

// ---------------------------------------------------------------------------
// State for the app-command callback (must be statics — no lambda captures).
// ---------------------------------------------------------------------------
static bool GbUiCreated = false;

static void OnAppCmd(struct android_app* a, int32_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
    {
        GLogger.Log("Android: APP_CMD_INIT_WINDOW");

        // Create the UI once the window surface is available.
        if (!GbUiCreated && GSharedApp && a->window)
        {
            CUIWindow* Window = GSharedApp->CreateMainWindow();
            if (Window)
            {
                GSharedApp->DidLaunch();
                GbUiCreated = true;

                // Initial render.
                AndroidWindow_RequestRender();
            }
        }
        else
        {
            // Window re-created (e.g. after rotation).
            AndroidWindow_RequestRender();
        }
        break;
    }
    case APP_CMD_WINDOW_REDRAW_NEEDED:
    case APP_CMD_CONTENT_RECT_CHANGED:
    {
        AndroidWindow_RequestRender();
        break;
    }
    case APP_CMD_TERM_WINDOW:
    {
        GLogger.Log("Android: APP_CMD_TERM_WINDOW");
        break;
    }
    case APP_CMD_DESTROY:
        GLogger.Log("Android: APP_CMD_DESTROY");
        break;
    case APP_CMD_RESUME:
        GLogger.Log("Android: APP_CMD_RESUME");
        break;
    case APP_CMD_PAUSE:
        GLogger.Log("Android: APP_CMD_PAUSE");
        break;
    default:
        break;
    }
}

// ---------------------------------------------------------------------------
// Input event callback — translates AInputEvent into framework touch events.
// ---------------------------------------------------------------------------

static int32_t OnInputEvent(struct android_app* /*app*/, AInputEvent* event)
{
    int type = AInputEvent_getType(event);

    if (type == AINPUT_EVENT_TYPE_MOTION)
    {
        int action = AMotionEvent_getAction(event);
        int actionCode = action & AMOTION_EVENT_ACTION_MASK;

        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);

        EUITouchPhase phase;
        switch (actionCode)
        {
        case AMOTION_EVENT_ACTION_DOWN:
            phase = EUITouchPhase::Began;
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            phase = EUITouchPhase::Moved;
            // Don't log every MOVE to avoid spam
            break;
        case AMOTION_EVENT_ACTION_UP:
            phase = EUITouchPhase::Ended;
            break;
        case AMOTION_EVENT_ACTION_CANCEL:
            phase = EUITouchPhase::Cancelled;
            break;
        default:
            return 0;
        }

        AndroidWindow_DispatchTouch(phase, x, y);
        return 1;
    }

    if (type == AINPUT_EVENT_TYPE_KEY)
    {
        int action = AKeyEvent_getAction(event);
        if (action == AKEY_EVENT_ACTION_DOWN)
        {
            int keyCode = AKeyEvent_getKeyCode(event);
            AndroidWindow_DispatchKeyEvent(keyCode, action);
            return 1;
        }
    }

    return 0;
}

// ---------------------------------------------------------------------------
// CApplication::Run — Android implementation using ALooper event loop.
// ---------------------------------------------------------------------------
int CApplication::Run(int /*argc*/, char* /*argv*/[])
{
    GSharedApp = this;

    SCollectorScope Scope;

    struct android_app* app = AndroidApplication_GetAndroidApp();
    if (!app)
    {
        GLogger.Error("CApplication::Run: AndroidApplication not set!");
        return 1;
    }

    GbUiCreated = false;

    app->onAppCmd = OnAppCmd;
    app->onInputEvent = OnInputEvent;

    GLogger.Log("CApplication::Run - entering Android event loop");

    while (!app->destroyRequested)
    {
        int events;
        struct android_poll_source* source;

        int result = ALooper_pollOnce(-1, nullptr, &events, (void**)&source);
        if (result >= 0 && source)
        {
            source->process(app, source);
        }
    }

    GLogger.Log("CApplication::Run - exiting");
    return 0;
}
