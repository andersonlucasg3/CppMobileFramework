#include "Devices/UIScreen.h"

#include "SmartPointer/MakeAndCasts.h"

#include <android/native_window.h>
#include <jni.h>

// ---------------------------------------------------------------------------
// Access the ANativeWindow from the Application module (same .so).
// ---------------------------------------------------------------------------
extern "C" ANativeWindow* AndroidApplication_GetNativeWindow();
extern "C" JavaVM* GJavaVM;
extern "C" jobject GAndroidActivityClazz;

// ---------------------------------------------------------------------------
// CNativeScreen — empty placeholder (same pattern as Windows).
// ---------------------------------------------------------------------------
class CNativeScreen {};

// ---------------------------------------------------------------------------
// Static singleton.
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Bounds — read from the ANativeWindow when available, otherwise fall back
// to a reasonable default (1080×1920).
// ---------------------------------------------------------------------------

SRectF CUIScreen::Bounds() const
{
    ANativeWindow* win = AndroidApplication_GetNativeWindow();
    if (win)
    {
        int32_t w = ANativeWindow_getWidth(win);
        int32_t h = ANativeWindow_getHeight(win);
        if (w > 0 && h > 0)
            return SRectF(0, 0, (Float)w, (Float)h);
    }

    // Fallback: common emulator portrait resolution.
    return SRectF(0, 0, 1080, 1920);
}

// ---------------------------------------------------------------------------
// Density — read DisplayMetrics.density via JNI.
// ---------------------------------------------------------------------------

Float CUIScreen::Density() const
{
    if (!GJavaVM || !GAndroidActivityClazz)
        return 1.0f;

    JNIEnv* env = nullptr;
    bool bAttached = false;

    jint result = GJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED)
    {
        JavaVMAttachArgs args = {};
        args.version = JNI_VERSION_1_6;
        args.name    = "CppDensityThread";
        if (GJavaVM->AttachCurrentThread(&env, &args) == JNI_OK)
            bAttached = true;
    }

    if (!env)
        return 1.0f;

    Float density = 1.0f;

    // Activity.getResources().getDisplayMetrics().density
    jclass activityClass = env->GetObjectClass(GAndroidActivityClazz);
    if (activityClass)
    {
        jmethodID getResources = env->GetMethodID(activityClass, "getResources",
            "()Landroid/content/res/Resources;");
        if (getResources)
        {
            jobject resources = env->CallObjectMethod(GAndroidActivityClazz, getResources);
            if (resources)
            {
                jclass resourcesClass = env->GetObjectClass(resources);
                jmethodID getDisplayMetrics = env->GetMethodID(resourcesClass,
                    "getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
                if (getDisplayMetrics)
                {
                    jobject dm = env->CallObjectMethod(resources, getDisplayMetrics);
                    if (dm)
                    {
                        jclass dmClass = env->GetObjectClass(dm);
                        jfieldID densityField = env->GetFieldID(dmClass, "density", "F");
                        if (densityField)
                            density = (Float)env->GetFloatField(dm, densityField);
                        env->DeleteLocalRef(dmClass);
                        env->DeleteLocalRef(dm);
                    }
                }
                env->DeleteLocalRef(resourcesClass);
                env->DeleteLocalRef(resources);
            }
        }
        env->DeleteLocalRef(activityClass);
    }

    if (bAttached)
        GJavaVM->DetachCurrentThread();

    return density;
}

// ---------------------------------------------------------------------------
// NativeScreen accessor
// ---------------------------------------------------------------------------

CNativeScreen& CUIScreen::NativeScreen() const
{
    return *_nativeScreen;
}
