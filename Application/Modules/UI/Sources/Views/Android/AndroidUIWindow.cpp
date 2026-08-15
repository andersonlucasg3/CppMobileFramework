#include "Views/UIWindow.h"

#include "AndroidNative.h"
#include "AndroidFramebuffer.h"
#include "AndroidFont.h"

#include "SmartPointer/MakeAndCasts.h"
#include "Controllers/UIViewController.h"
#include "Devices/UIScreen.h"
#include "Views/UILabel.h"
#include "Views/UIButton.h"
#include "Views/UITextField.h"
#include "Views/UIScrollView.h"

#include <functional>

// ---------------------------------------------------------------------------
extern "C" ANativeWindow* AndroidApplication_GetNativeWindow();

// ---------------------------------------------------------------------------
static CUIWindow* GMainWindow = nullptr;
static bool       GSuppressRender = false;

void AndroidWindow_RequestRender()
{
    if (GSuppressRender) return;
    if (GMainWindow)
        GMainWindow->Render();
}

// ---------------------------------------------------------------------------
// Helper: convert SColorF to ARGB uint32_t.
// ---------------------------------------------------------------------------
static uint32_t ColorToARGB(const SColorF& c)
{
    uint8_t r = (uint8_t)(c.R() * 255.0f);
    uint8_t g = (uint8_t)(c.G() * 255.0f);
    uint8_t b = (uint8_t)(c.B() * 255.0f);
    uint8_t a = (uint8_t)(c.A() * 255.0f);
    return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------
void CUIWindow::Render()
{
    ANativeWindow* win = AndroidApplication_GetNativeWindow();
    if (!win) return;

    _nativeInstance->SetWindow(win);

    SAndroidFont::Shared().Initialize();

    SAndroidFramebuffer fb;
    if (!fb.Lock(win))
    {
        GLogger.Error("Render: ANativeWindow_lock failed");
        return;
    }

    fb.Clear(_nativeInstance->GetBackgroundColor());

    // -------------------------------------------------------------------
    // Forward declarations for mutual recursion.
    // -------------------------------------------------------------------
    std::function<void(CUIView*, float, float)> DrawView;

    // -------------------------------------------------------------------
    // DrawView(view, offsetX, offsetY)
    //
    // offsetX/offsetY are used by CUIScrollView to shift children.
    // For the root pass they are zero.
    // -------------------------------------------------------------------
    DrawView = [&](CUIView* view, float ox, float oy)
    {
        if (!view) return;

        SRectF  f  = view->Frame();
        uint32_t bg = view->_nativeInstance->GetBackgroundColor();

        int vx = (int)(f.Origin.X + ox);
        int vy = (int)(f.Origin.Y + oy);
        int vw = (int)f.Size.Width;
        int vh = (int)f.Size.Height;

        if (vw <= 0 || vh <= 0) return;

        // ---------------------------------------------------------------
        // CUIScrollView: clip children, then draw them shifted
        // ---------------------------------------------------------------
        if (CUIScrollView* sv = dynamic_cast<CUIScrollView*>(view))
        {
            fb.FillRect(vx, vy, vw, vh, bg);

            SPointF co = sv->ContentOffset();
            float sx = -(float)co.X;
            float sy = -(float)co.Y;

            fb.SetClipRect(vx, vy, vw, vh);

            for (int i = 0; i < (int)sv->_subviews.Num(); ++i)
            {
                CUIView* child = sv->_subviews[i];
                if (child) DrawView(child, ox + sx, oy + sy);
            }

            fb.ClearClipRect();
            return;
        }

        // ---------------------------------------------------------------
        // CUIButton: rounded rect look
        // ---------------------------------------------------------------
        if (CUIButton* btn = dynamic_cast<CUIButton*>(view))
        {
            // Main fill
            fb.FillRect(vx, vy, vw, vh, bg);

            // Top highlight (pressed: none; normal: slightly lighter)
            if (!btn->_bPressed)
            {
                uint8_t r = (uint8_t)(((bg >> 16) & 0xFF) * 1.2f > 255 ? 255 : (uint8_t)(((bg >> 16) & 0xFF) * 1.2f));
                uint8_t g = (uint8_t)(((bg >>  8) & 0xFF) * 1.2f > 255 ? 255 : (uint8_t)(((bg >>  8) & 0xFF) * 1.2f));
                uint8_t b = (uint8_t)(((bg >>  0) & 0xFF) * 1.2f > 255 ? 255 : (uint8_t)(((bg >>  0) & 0xFF) * 1.2f));
                uint32_t lighter = ((uint32_t)((bg >> 24) & 0xFF) << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
                // Top 40% of button is lighter
                int topH = vh * 2 / 5;
                if (topH > 0) fb.FillRect(vx, vy, vw, topH, lighter);
            }
            else
            {
                // Pressed: slightly darker overlay
                uint8_t r2 = (uint8_t)(((bg >> 16) & 0xFF) * 0.7f);
                uint8_t g2 = (uint8_t)(((bg >>  8) & 0xFF) * 0.7f);
                uint8_t b2 = (uint8_t)(((bg >>  0) & 0xFF) * 0.7f);
                uint32_t darker = ((uint32_t)((bg >> 24) & 0xFF) << 24) | ((uint32_t)r2 << 16) | ((uint32_t)g2 << 8) | (uint32_t)b2;
                fb.FillRect(vx + 2, vy + 2, vw - 4, vh - 4, darker);
            }

            // Draw text centered
            const CString& t = btn->Text();
            if (t.Len() > 0)
            {
                uint32_t tc = ColorToARGB(btn->_textColor);
                float pxSize = (vh > 0) ? (float)vh * 0.45f : 36.0f;
                if (pxSize < 12.0f) pxSize = 12.0f;
                if (pxSize > 72.0f) pxSize = 72.0f;
                SAndroidFont::Shared().SetPixelSize(pxSize);
                SAndroidFont::Shared().RenderString(fb, *t,
                    vx, vy, vw, tc, ETextAlignment::Center);
            }

            // Children (unlikely but supported)
            for (int i = 0; i < (int)btn->_subviews.Num(); ++i)
            {
                CUIView* child = btn->_subviews[i];
                if (child) DrawView(child, ox, oy);
            }
            return;
        }

        // ---------------------------------------------------------------
        // CUITextField
        // ---------------------------------------------------------------
        if (CUITextField* tf = dynamic_cast<CUITextField*>(view))
        {
            // Light background
            fb.FillRect(vx, vy, vw, vh, 0xFFF0F0F0);

            // Border (2px)
            uint32_t borderColor = tf->IsFocused() ? 0xFF4488FF : 0xFFAAAAAA;
            fb.FillRect(vx,         vy,          vw, 2,  borderColor); // top
            fb.FillRect(vx,         vy + vh - 2, vw, 2,  borderColor); // bottom
            fb.FillRect(vx,         vy,          2,  vh, borderColor); // left
            fb.FillRect(vx + vw - 2, vy,          2,  vh, borderColor); // right

            // Padding = 8px inside
            int pad = 8;
            int textX = vx + pad;
            int textY = vy + pad;
            int textW = vw - pad * 2;
            int textH = vh - pad * 2;
            if (textW < 0) textW = 0;
            if (textH < 0) textH = 0;

            const CString& tText = tf->Text();
            if (tText.Len() > 0)
            {
                uint32_t tc = ColorToARGB(tf->TextColor());
                float pxSize = (float)textH * 0.7f;
                if (pxSize < 12.0f) pxSize = 12.0f;
                if (pxSize > 64.0f) pxSize = 64.0f;
                SAndroidFont::Shared().SetPixelSize(pxSize);
                SAndroidFont::Shared().RenderString(fb, *tText,
                    textX, textY, textW, tc, ETextAlignment::Left);
            }
            else
            {
                const CString& ph = tf->Placeholder();
                if (ph.Len() > 0)
                {
                    float pxSize = (float)textH * 0.7f;
                    if (pxSize < 12.0f) pxSize = 12.0f;
                    if (pxSize > 64.0f) pxSize = 64.0f;
                    SAndroidFont::Shared().SetPixelSize(pxSize);
                    SAndroidFont::Shared().RenderString(fb, *ph,
                        textX, textY, textW, 0xFFAAAAAA, ETextAlignment::Left);
                }
            }

            // Cursor (filled vertical line when focused)
            if (tf->IsFocused())
            {
                int cursorX = textX;
                if (tText.Len() > 0)
                {
                    int w, h;
                    float pxSize = (float)textH * 0.7f;
                    if (pxSize < 12.0f) pxSize = 12.0f;
                    if (pxSize > 64.0f) pxSize = 64.0f;
                    SAndroidFont::Shared().SetPixelSize(pxSize);
                    SAndroidFont::Shared().MeasureString(*tText, &w, &h);
                    cursorX = textX + w;
                    if (cursorX > textX + textW - 2) cursorX = textX + textW - 2;
                }
                int cursorH = textH * 3 / 5;
                int cursorY = textY + (textH - cursorH) / 2;
                fb.FillRect(cursorX, cursorY, 2, cursorH, 0xFF000000);
            }

            for (int i = 0; i < (int)tf->_subviews.Num(); ++i)
            {
                CUIView* child = tf->_subviews[i];
                if (child) DrawView(child, ox, oy);
            }
            return;
        }

        // ---------------------------------------------------------------
        // Default: solid fill + optional label text
        // ---------------------------------------------------------------
        fb.FillRect(vx, vy, vw, vh, bg);

        if (CUILabel* label = dynamic_cast<CUILabel*>(view))
        {
            const CString& text = label->Text();
            if (text.Len() > 0)
            {
                ETextAlignment align = label->TextAlignment();
                Float ts = label->TextSize();
                float pxSize;
                if (ts > 0.0f)
                    pxSize = ts;
                else
                    pxSize = (vh > 0) ? (float)vh * 0.6f : 56.0f;
                if (pxSize < 12.0f) pxSize = 12.0f;
                if (pxSize > 128.0f) pxSize = 128.0f;

                uint32_t tc = ColorToARGB(label->TextColor());

                SAndroidFont::Shared().SetPixelSize(pxSize);
                SAndroidFont::Shared().RenderString(
                    fb, *text,
                    vx, vy, vw,
                    tc, align);
            }
        }

        // Draw children
        for (int i = 0; i < (int)view->_subviews.Num(); ++i)
        {
            CUIView* child = view->_subviews[i];
            if (child) DrawView(child, ox, oy);
        }
    };

    // Root pass: no offset
    for (int i = 0; i < (int)_subviews.Num(); ++i)
    {
        CUIView* child = _subviews[i];
        if (child) DrawView(child, 0.0f, 0.0f);
    }

    fb.UnlockAndPost();
}

// ---------------------------------------------------------------------------
// HitTest — recursive, deepest-first (reverse children = topmost first)
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

    // Only return leaf views that handle touch
    if (dynamic_cast<CUIButton*>(view) ||
        dynamic_cast<CUIScrollView*>(view) ||
        dynamic_cast<CUITextField*>(view))
        return view;

    return nullptr;
}

CUIView* CUIWindow::HitTest(SPointF Point)
{
    // Search from the root view controller's view downward
    for (int i = (int)_subviews.Num() - 1; i >= 0; --i)
    {
        CUIView* child = _subviews[i];
        CUIView* hit = CUIWindow::HitTestView(child, Point.X, Point.Y, 0.0f, 0.0f);
        if (hit) return hit;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// Touch dispatch globals (used by OnInputEvent in AndroidApplication.cpp)
// ---------------------------------------------------------------------------
static CUIView* GTouchCapture = nullptr;

void AndroidWindow_DispatchTouch(EUITouchPhase Phase, float X, float Y)
{
    if (!GMainWindow) return;

    if (Phase == EUITouchPhase::Began)
    {
        CUIView* hit = GMainWindow->HitTest(SPointF(X, Y));
        if (hit)
        {
            GTouchCapture = hit;
            GTouchCapture->OnTouchEvent(Phase, SPointF(X, Y));
        }
    }
    else if (GTouchCapture)
    {
        GTouchCapture->OnTouchEvent(Phase, SPointF(X, Y));
        if (Phase == EUITouchPhase::Ended || Phase == EUITouchPhase::Cancelled)
            GTouchCapture = nullptr;
    }
}

// Expose for the focused text field key dispatch.
CUITextField* AndroidWindow_GetFocusedTextField();

void AndroidWindow_DispatchKeyEvent(int keyCode, int action)
{
    if (action != 0 /* AKEY_EVENT_ACTION_DOWN */) return;

    CUITextField* ftf = AndroidWindow_GetFocusedTextField();
    if (ftf)
    {
        ftf->HandleKeyEvent(keyCode, action);
    }
}

// ---------------------------------------------------------------------------
// CUIWindow constructors
// ---------------------------------------------------------------------------

CUIWindow::CUIWindow()
{
    _nativeInstance = MakeShared<CNativeInstance>();
    GMainWindow = this;

    ANativeWindow* win = AndroidApplication_GetNativeWindow();
    if (win)
        _nativeInstance->SetWindow(win);
}

CUIWindow::CUIWindow(const SRectF& WindowRect)
{
    CUIWindow();
    SetFrame(WindowRect);
}

// ---------------------------------------------------------------------------
// SetRootViewController
// ---------------------------------------------------------------------------

void CUIWindow::SetRootViewController(CUIViewController* VC)
{
    _rootViewController = VC;
    if (!VC) return;

    GSuppressRender = true;

    VC->ViewDidLoad();

    CUIView* V = VC->View();
    if (V)
    {
        AddSubview(V);
        SRectF B = CUIScreen::MainScreen()->Bounds();
        V->SetFrame(SRectF(0, 0, B.Size.Width, B.Size.Height));
    }

    GSuppressRender = false;
}
