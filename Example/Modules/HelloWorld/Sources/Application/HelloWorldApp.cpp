#include "HelloWorldApp.h"
#include "Views/UIWindow.h"
#include "Controllers/UIViewController.h"
#include "Views/UILabel.h"

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
            V->AddSubview(Label);

            SRectF ViewFrame = V->Frame();
            Float LabelW = 200.0f;
            Float LabelH = 40.0f;
            Float X = (ViewFrame.Size.Width - LabelW) / 2.0f;
            Float Y = (ViewFrame.Size.Height - LabelH) / 2.0f;
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
#endif