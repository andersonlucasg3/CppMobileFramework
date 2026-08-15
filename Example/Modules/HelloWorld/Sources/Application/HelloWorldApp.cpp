#include "HelloWorldApp.h"
#include "Views/UIWindow.h"
#include "Controllers/UIViewController.h"
#include "Views/UILabel.h"
#include "Devices/UIScreen.h"

class CHelloWorldViewController : public CUIViewController
{
    using Super = CUIViewController;

public:
    void ViewDidLoad() override
    {
        Super::ViewDidLoad();

        CUILabel* Label = new CUILabel();
        Label->SetText("Hello World");
        Label->SetTextAlignment(ETextAlignment::Center);

        CUIView* V = this->View();
        if (V)
        {
            V->SetBackgroundColor();

            // Add to view hierarchy FIRST so the native widget gets a parent
            V->AddSubview(Label);

            // Center using the screen bounds
            SRectF ScreenBounds = CUIScreen::MainScreen()->Bounds();
            Float LabelW = 480.0f;
            Float LabelH = 160.0f;
            Float X = (ScreenBounds.Size.Width - LabelW) / 2.0f;
            Float Y = (ScreenBounds.Size.Height - LabelH) / 2.0f;
            Label->SetFrame(SRectF(X, Y, LabelW, LabelH));
        }
    }
};

CUIWindow* CHelloWorldApp::CreateMainWindow()
{
    CUIWindow* Window = new CUIWindow();
    CHelloWorldViewController* VC = new CHelloWorldViewController();
    Window->SetRootViewController(VC);
    return Window;
}

#if PLATFORM_WINDOWS
int main(int argc, char* argv[])
{
    CHelloWorldApp App;
    return App.Run(argc, argv);
}
#endif // PLATFORM_WINDOWS

#if PLATFORM_ANDROID
#include "Android/native_app_glue/android_native_app_glue.h"

// Declared in Application/Modules/Application/Sources/Android/AndroidJni.h
extern "C" void  AndroidApplication_SetAndroidApp(struct android_app* app);
extern "C" JavaVM* GJavaVM;

void android_main(struct android_app* app)
{
    // Prevent the linker from stripping the glue library.
    app_dummy();

    GJavaVM = app->activity->vm;
    AndroidApplication_SetAndroidApp(app);

    // Run the platform event loop. The UI is created when APP_CMD_INIT_WINDOW
    // fires inside CApplication::Run.
    static CHelloWorldApp GApp;
    GApp.Run(0, nullptr);
}
#endif // PLATFORM_ANDROID
