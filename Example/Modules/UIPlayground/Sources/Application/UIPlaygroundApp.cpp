#include "UIPlaygroundApp.h"
#include "Views/UIWindow.h"
#include "Controllers/UIViewController.h"
#include "Views/UILabel.h"
#include "Views/UIButton.h"
#include "Views/UIStackView.h"
#include "Views/UIScrollView.h"
#include "Views/UITextField.h"
#include "Devices/UIScreen.h"

// ---------------------------------------------------------------------------
// ViewController
// ---------------------------------------------------------------------------

class CUIPlaygroundViewController : public CUIViewController
{
    using Super = CUIViewController;

public:
    CUILabel* StatusLabel = nullptr;
    CUILabel* MirrorLabel = nullptr;
    CUITextField* TextField = nullptr;
    int ClickCount = 0;

    void ViewDidLoad() override
    {
        Super::ViewDidLoad();

        CUIView* V = this->View();
        if (!V) return;

        V->SetBackgroundColor(SColorF(0.12f, 0.12f, 0.15f, 1.0f));

        SRectF B = CUIScreen::MainScreen()->Bounds();
        Float SW = B.Size.Width;
        Float SH = B.Size.Height;

        // Density scale factor — clamp to sane range.
        const Float D = CUIScreen::MainScreen()->Density();
        const Float S = (D < 1.0f) ? 1.0f : ((D > 4.0f) ? 4.0f : D);
        auto Px = [S](Float v) -> Float { return v * S; };

        // -------------------------------------------------------------------
        // ScrollView — fills the screen
        // -------------------------------------------------------------------
        CUIScrollView* Scroll = new CUIScrollView();
        Scroll->SetFrame(SRectF(0, 0, SW, SH));
        Scroll->SetBackgroundColor(SColorF(0.12f, 0.12f, 0.15f, 1.0f));
        V->AddSubview(Scroll);

        Float contentH = Px(2000.0f); // enough for all content
        Scroll->SetContentSize(SSizeF(SW, contentH));

        Float y = Px(20.0f);
        Float margin = Px(16.0f);
        Float contentW = SW - margin * 2;

        // -------------------------------------------------------------------
        // Title label — large, centered
        // -------------------------------------------------------------------
        {
            CUILabel* Title = new CUILabel();
            Title->SetText("UI Playground");
            Title->SetTextAlignment(ETextAlignment::Center);
            Title->SetTextColor(SColorF::White());
            Title->SetTextSize(Px(48.0f));
            Title->SetBackgroundColor(SColorF(0.18f, 0.18f, 0.22f, 1.0f));
            Title->SetFrame(SRectF(margin, y, contentW, Px(80.0f)));
            Scroll->AddSubview(Title);
            y += Px(80.0f) + Px(16.0f);
        }

        // -------------------------------------------------------------------
        // Section: Alignment demo
        // -------------------------------------------------------------------
        {
            CUILabel* SecLabel = new CUILabel();
            SecLabel->SetText("Text Alignment");
            SecLabel->SetTextColor(SColorF(0.7f, 0.7f, 0.8f, 1.0f));
            SecLabel->SetTextSize(Px(24.0f));
            SecLabel->SetFrame(SRectF(margin, y, contentW, Px(36.0f)));
            Scroll->AddSubview(SecLabel);
            y += Px(40.0f);

            CUILabel* Left = new CUILabel();
            Left->SetText("Left-aligned text");
            Left->SetTextAlignment(ETextAlignment::Left);
            Left->SetTextColor(SColorF::White());
            Left->SetTextSize(Px(22.0f));
            Left->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            Left->SetFrame(SRectF(margin, y, contentW, Px(44.0f)));
            Scroll->AddSubview(Left);
            y += Px(48.0f);

            CUILabel* Center = new CUILabel();
            Center->SetText("Center-aligned text");
            Center->SetTextAlignment(ETextAlignment::Center);
            Center->SetTextColor(SColorF::White());
            Center->SetTextSize(Px(22.0f));
            Center->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            Center->SetFrame(SRectF(margin, y, contentW, Px(44.0f)));
            Scroll->AddSubview(Center);
            y += Px(48.0f);

            CUILabel* Right = new CUILabel();
            Right->SetText("Right-aligned text");
            Right->SetTextAlignment(ETextAlignment::Right);
            Right->SetTextColor(SColorF::White());
            Right->SetTextSize(Px(22.0f));
            Right->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            Right->SetFrame(SRectF(margin, y, contentW, Px(44.0f)));
            Scroll->AddSubview(Right);
            y += Px(52.0f);
        }

        // -------------------------------------------------------------------
        // Section: Colors & sizes
        // -------------------------------------------------------------------
        {
            CUILabel* SecLabel = new CUILabel();
            SecLabel->SetText("Colored Text & Sizes");
            SecLabel->SetTextColor(SColorF(0.7f, 0.7f, 0.8f, 1.0f));
            SecLabel->SetTextSize(Px(24.0f));
            SecLabel->SetFrame(SRectF(margin, y, contentW, Px(36.0f)));
            Scroll->AddSubview(SecLabel);
            y += Px(40.0f);

            CUILabel* Red = new CUILabel();
            Red->SetText("Red text, 24px");
            Red->SetTextColor(SColorF::Red());
            Red->SetTextSize(Px(24.0f));
            Red->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            Red->SetFrame(SRectF(margin, y, contentW, Px(44.0f)));
            Scroll->AddSubview(Red);
            y += Px(48.0f);

            CUILabel* Green = new CUILabel();
            Green->SetText("Green text, 20px");
            Green->SetTextColor(SColorF::Green());
            Green->SetTextSize(Px(20.0f));
            Green->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            Green->SetFrame(SRectF(margin, y, contentW, Px(44.0f)));
            Scroll->AddSubview(Green);
            y += Px(48.0f);

            CUILabel* Blue = new CUILabel();
            Blue->SetText("Blue text, 28px");
            Blue->SetTextColor(SColorF::Blue());
            Blue->SetTextSize(Px(28.0f));
            Blue->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            Blue->SetFrame(SRectF(margin, y, contentW, Px(44.0f)));
            Scroll->AddSubview(Blue);
            y += Px(52.0f);
        }

        // -------------------------------------------------------------------
        // Section: Colored views
        // -------------------------------------------------------------------
        {
            CUILabel* SecLabel = new CUILabel();
            SecLabel->SetText("Colored Views");
            SecLabel->SetTextColor(SColorF(0.7f, 0.7f, 0.8f, 1.0f));
            SecLabel->SetTextSize(Px(24.0f));
            SecLabel->SetFrame(SRectF(margin, y, contentW, Px(36.0f)));
            Scroll->AddSubview(SecLabel);
            y += Px(40.0f);

            Float gap = Px(4.0f);
            Float blockW = contentW / 3.0f - gap;
            CUIView* Block1 = new CUIView();
            Block1->SetBackgroundColor(SColorF::Red());
            Block1->SetFrame(SRectF(margin, y, blockW, Px(50.0f)));
            Scroll->AddSubview(Block1);

            CUIView* Block2 = new CUIView();
            Block2->SetBackgroundColor(SColorF::Green());
            Block2->SetFrame(SRectF(margin + blockW + gap, y, blockW, Px(50.0f)));
            Scroll->AddSubview(Block2);

            CUIView* Block3 = new CUIView();
            Block3->SetBackgroundColor(SColorF::Blue());
            Block3->SetFrame(SRectF(margin + (blockW + gap) * 2, y, blockW, Px(50.0f)));
            Scroll->AddSubview(Block3);
            y += Px(58.0f);
        }

        // -------------------------------------------------------------------
        // Section: StackView
        // -------------------------------------------------------------------
        {
            CUILabel* SecLabel = new CUILabel();
            SecLabel->SetText("StackView");
            SecLabel->SetTextColor(SColorF(0.7f, 0.7f, 0.8f, 1.0f));
            SecLabel->SetTextSize(Px(24.0f));
            SecLabel->SetFrame(SRectF(margin, y, contentW, Px(36.0f)));
            Scroll->AddSubview(SecLabel);
            y += Px(40.0f);

            CUIStackView* Stack = new CUIStackView();
            Stack->SetBackgroundColor(SColorF(0.2f, 0.2f, 0.28f, 1.0f));
            Stack->SetSpacing(Px(8.0f));
            Stack->SetFrame(SRectF(margin, y, contentW, Px(160.0f)));
            Scroll->AddSubview(Stack);

            CUILabel* S1 = new CUILabel();
            S1->SetText("Stack Item 1");
            S1->SetTextColor(SColorF::White());
            S1->SetTextSize(Px(20.0f));
            S1->SetBackgroundColor(SColorF(0.3f, 0.3f, 0.5f, 1.0f));
            S1->SetFrame(SRectF(0, 0, contentW, Px(48.0f)));
            Stack->AddArrangedSubview(S1);

            CUILabel* S2 = new CUILabel();
            S2->SetText("Stack Item 2");
            S2->SetTextColor(SColorF::White());
            S2->SetTextSize(Px(20.0f));
            S2->SetBackgroundColor(SColorF(0.3f, 0.5f, 0.3f, 1.0f));
            S2->SetFrame(SRectF(0, 0, contentW, Px(48.0f)));
            Stack->AddArrangedSubview(S2);

            CUILabel* S3 = new CUILabel();
            S3->SetText("Stack Item 3");
            S3->SetTextColor(SColorF::White());
            S3->SetTextSize(Px(20.0f));
            S3->SetBackgroundColor(SColorF(0.5f, 0.3f, 0.3f, 1.0f));
            S3->SetFrame(SRectF(0, 0, contentW, Px(48.0f)));
            Stack->AddArrangedSubview(S3);

            y += Px(168.0f);
        }

        // -------------------------------------------------------------------
        // Section: Button + status label
        // -------------------------------------------------------------------
        {
            CUILabel* SecLabel = new CUILabel();
            SecLabel->SetText("Button (tap to test touch)");
            SecLabel->SetTextColor(SColorF(0.7f, 0.7f, 0.8f, 1.0f));
            SecLabel->SetTextSize(Px(24.0f));
            SecLabel->SetFrame(SRectF(margin, y, contentW, Px(36.0f)));
            Scroll->AddSubview(SecLabel);
            y += Px(44.0f);

            CUIButton* Button = new CUIButton();
            Button->SetText("Click Me!");
            Button->SetTextColor(SColorF::White());
            Button->SetBackgroundColor(SColorF(0.2f, 0.45f, 0.8f, 1.0f));
            Button->SetFrame(SRectF(margin, y, contentW, Px(64.0f)));
            Scroll->AddSubview(Button);

            Button->SetOnClickListener([this]() {
                ClickCount++;
                if (StatusLabel)
                {
                    CString txt("Clicks: ");
                    // Simple int-to-string via format
                    char buf[32];
                    snprintf(buf, sizeof(buf), "Clicks: %d", ClickCount);
                    StatusLabel->SetText(CString(buf));
                }
            });

            y += Px(72.0f);

            StatusLabel = new CUILabel();
            StatusLabel->SetText("Clicks: 0");
            StatusLabel->SetTextAlignment(ETextAlignment::Center);
            StatusLabel->SetTextColor(SColorF::Yellow());
            StatusLabel->SetTextSize(Px(28.0f));
            StatusLabel->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            StatusLabel->SetFrame(SRectF(margin, y, contentW, Px(48.0f)));
            Scroll->AddSubview(StatusLabel);
            y += Px(56.0f);
        }

        // -------------------------------------------------------------------
        // Section: TextField + mirror label
        // -------------------------------------------------------------------
        {
            CUILabel* SecLabel = new CUILabel();
            SecLabel->SetText("TextField (tap to type)");
            SecLabel->SetTextColor(SColorF(0.7f, 0.7f, 0.8f, 1.0f));
            SecLabel->SetTextSize(Px(24.0f));
            SecLabel->SetFrame(SRectF(margin, y, contentW, Px(36.0f)));
            Scroll->AddSubview(SecLabel);
            y += Px(44.0f);

            TextField = new CUITextField();
            TextField->SetPlaceholder("Type something...");
            TextField->SetTextColor(SColorF::Black());
            TextField->SetFrame(SRectF(margin, y, contentW, Px(56.0f)));
            Scroll->AddSubview(TextField);

            // Mirror label updates when text field changes
            TextField->SetOnTextChanged([this](const CString& NewText) {
                if (MirrorLabel)
                {
                    if (NewText.Len() > 0)
                        MirrorLabel->SetText(NewText);
                    else
                        MirrorLabel->SetText("(your text will appear here)");
                }
            });

            y += Px(64.0f);

            MirrorLabel = new CUILabel();
            MirrorLabel->SetText("(your text will appear here)");
            MirrorLabel->SetTextColor(SColorF(0.8f, 0.8f, 0.9f, 1.0f));
            MirrorLabel->SetTextSize(Px(18.0f));
            MirrorLabel->SetBackgroundColor(SColorF(0.25f, 0.25f, 0.35f, 1.0f));
            MirrorLabel->SetFrame(SRectF(margin, y, contentW, Px(48.0f)));
            Scroll->AddSubview(MirrorLabel);
            y += Px(56.0f);
        }
    }
};

// ---------------------------------------------------------------------------
// CUIPlaygroundApp
// ---------------------------------------------------------------------------

CUIWindow* CUIPlaygroundApp::CreateMainWindow()
{
    CUIWindow* Window = new CUIWindow();
    CUIPlaygroundViewController* VC = new CUIPlaygroundViewController();
    Window->SetRootViewController(VC);
    return Window;
}

// ---------------------------------------------------------------------------
// Platform entry points
// ---------------------------------------------------------------------------

#if PLATFORM_WINDOWS
#include <Windows.h>
int main(int argc, char* argv[])
{
    CUIPlaygroundApp App;
    return App.Run(argc, argv);
}
#endif // PLATFORM_WINDOWS

#if PLATFORM_ANDROID
#include "Android/native_app_glue/android_native_app_glue.h"

extern "C" void  AndroidApplication_SetAndroidApp(struct android_app* app);
extern "C" JavaVM* GJavaVM;

void android_main(struct android_app* app)
{
    app_dummy();

    GJavaVM = app->activity->vm;
    AndroidApplication_SetAndroidApp(app);

    static CUIPlaygroundApp GApp;
    GApp.Run(0, nullptr);
}
#endif // PLATFORM_ANDROID
