#include "AndroidJni.h"

#include "Preprocessors.h"
#include "Logger/Logger.h"

#include "native_app_glue/android_native_app_glue.h"

#include <android/log.h>
#include <cstdarg>

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

extern "C" JavaVM* GJavaVM = nullptr;
extern "C" jobject GAndroidActivityClazz = nullptr;

static struct android_app* GAndroidApp = nullptr;

extern "C" struct android_app* AndroidApplication_GetAndroidApp()
{
    return GAndroidApp;
}

extern "C" void AndroidApplication_SetAndroidApp(struct android_app* app)
{
    // Idempotent: called both from the main-thread hook and android_main.
    if (GAndroidApp == app && GAndroidActivityClazz)
        return;

    GAndroidApp = app;

    // Store a global ref to the activity for the UI module.
    if (app && app->activity && app->activity->clazz)
    {
        SAndroidJniScope Scope;
        if (Scope.Env)
        {
            if (GAndroidActivityClazz)
                Scope.Env->DeleteGlobalRef(GAndroidActivityClazz);
            GAndroidActivityClazz = Scope.Env->NewGlobalRef(app->activity->clazz);
        }
    }
}

extern "C" ANativeWindow* AndroidApplication_GetNativeWindow()
{
    if (GAndroidApp)
        return GAndroidApp->window;
    return nullptr;
}

// ---------------------------------------------------------------------------
// Show / hide the soft keyboard via InputMethodManager.
// ---------------------------------------------------------------------------
extern "C" void AndroidApplication_ShowSoftKeyboard(bool bShow)
{
    if (!GAndroidActivityClazz) return;

    SAndroidJniScope Scope;
    JNIEnv* env = Scope.Env;
    if (!env) return;

    // Get the activity's window → decorView
    jclass activityClass = env->GetObjectClass(GAndroidActivityClazz);
    jmethodID getWindow = env->GetMethodID(activityClass, "getWindow", "()Landroid/view/Window;");
    if (!getWindow) { env->DeleteLocalRef(activityClass); return; }

    jobject window = env->CallObjectMethod(GAndroidActivityClazz, getWindow);
    if (!window) { env->DeleteLocalRef(activityClass); return; }

    jclass windowClass = env->GetObjectClass(window);
    jmethodID getDecorView = env->GetMethodID(windowClass, "getDecorView", "()Landroid/view/View;");
    if (!getDecorView) { env->DeleteLocalRef(window); env->DeleteLocalRef(windowClass); env->DeleteLocalRef(activityClass); return; }

    jobject decorView = env->CallObjectMethod(window, getDecorView);
    if (!decorView) { env->DeleteLocalRef(window); env->DeleteLocalRef(windowClass); env->DeleteLocalRef(activityClass); return; }

    // Get InputMethodManager
    jclass contextClass = env->GetObjectClass(GAndroidActivityClazz);
    jmethodID getSystemService = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    env->DeleteLocalRef(contextClass);

    jstring serviceName = env->NewStringUTF("input_method");
    jobject imm = env->CallObjectMethod(GAndroidActivityClazz, getSystemService, serviceName);
    env->DeleteLocalRef(serviceName);

    if (!imm)
    {
        env->DeleteLocalRef(decorView);
        env->DeleteLocalRef(window);
        env->DeleteLocalRef(windowClass);
        env->DeleteLocalRef(activityClass);
        return;
    }

    jclass immClass = env->GetObjectClass(imm);

    if (bShow)
    {
        jmethodID showSoftInput = env->GetMethodID(immClass, "showSoftInput", "(Landroid/view/View;I)Z");
        if (showSoftInput)
            env->CallBooleanMethod(imm, showSoftInput, decorView, 0);
    }
    else
    {
        jmethodID hideSoftInputFromWindow = env->GetMethodID(immClass, "hideSoftInputFromWindow",
            "(Landroid/os/IBinder;I)Z");
        if (hideSoftInputFromWindow)
        {
            jclass decorClass = env->GetObjectClass(decorView);
            jmethodID getWindowToken = env->GetMethodID(decorClass, "getWindowToken", "()Landroid/os/IBinder;");
            env->DeleteLocalRef(decorClass);
            if (getWindowToken)
            {
                jobject token = env->CallObjectMethod(decorView, getWindowToken);
                if (token)
                {
                    env->CallBooleanMethod(imm, hideSoftInputFromWindow, token, 0);
                    env->DeleteLocalRef(token);
                }
            }
        }
    }

    env->DeleteLocalRef(imm);
    env->DeleteLocalRef(immClass);
    env->DeleteLocalRef(decorView);
    env->DeleteLocalRef(window);
    env->DeleteLocalRef(windowClass);
    env->DeleteLocalRef(activityClass);
}

// ---------------------------------------------------------------------------
// SAndroidJniScope
// ---------------------------------------------------------------------------

SAndroidJniScope::SAndroidJniScope()
    : Env(nullptr)
    , bAttached(false)
{
    if (!GJavaVM)
    {
        GLogger.Error("SAndroidJniScope: GJavaVM is null!");
        return;
    }

    jint result = GJavaVM->GetEnv((void**)&Env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED)
    {
        JavaVMAttachArgs args = {};
        args.version = JNI_VERSION_1_6;
        args.name    = "CppNativeThread";
        args.group   = nullptr;

        result = GJavaVM->AttachCurrentThread(&Env, &args);
        if (result == JNI_OK)
        {
            bAttached = true;
        }
        else
        {
            GLogger.Error("SAndroidJniScope: failed to attach thread");
            Env = nullptr;
        }
    }
    else if (result != JNI_OK)
    {
        GLogger.Error("SAndroidJniScope: GetEnv failed");
        Env = nullptr;
    }
}

SAndroidJniScope::~SAndroidJniScope()
{
    if (bAttached && GJavaVM)
    {
        GJavaVM->DetachCurrentThread();
    }
}

// ---------------------------------------------------------------------------
// AndroidJni helpers
// ---------------------------------------------------------------------------

namespace AndroidJni
{

jclass FindClass(JNIEnv* env, const char* className)
{
    if (!env) return nullptr;
    jclass clazz = env->FindClass(className);
    if (!clazz)
    {
        GLogger.Error("AndroidJni::FindClass: failed to find {}", className);
        if (env->ExceptionCheck())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
    }
    return clazz;
}

jmethodID GetStaticMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig)
{
    if (!env || !clazz) return nullptr;
    jmethodID m = env->GetStaticMethodID(clazz, name, sig);
    if (!m)
    {
        GLogger.Error("AndroidJni::GetStaticMethodID: {} {} not found", name, sig);
        if (env->ExceptionCheck()) { env->ExceptionDescribe(); env->ExceptionClear(); }
    }
    return m;
}

jmethodID GetMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig)
{
    if (!env || !clazz) return nullptr;
    jmethodID m = env->GetMethodID(clazz, name, sig);
    if (!m)
    {
        GLogger.Error("AndroidJni::GetMethodID: {} {} not found", name, sig);
        if (env->ExceptionCheck()) { env->ExceptionDescribe(); env->ExceptionClear(); }
    }
    return m;
}

jobject CallStaticObjectMethod(JNIEnv* env, jclass clazz, jmethodID method, ...)
{
    if (!env || !clazz || !method) return nullptr;
    va_list args;
    va_start(args, method);
    jobject result = env->CallStaticObjectMethodV(clazz, method, args);
    va_end(args);
    return result;
}

jobject CallObjectMethod(JNIEnv* env, jobject obj, jmethodID method, ...)
{
    if (!env || !obj || !method) return nullptr;
    va_list args;
    va_start(args, method);
    jobject result = env->CallObjectMethodV(obj, method, args);
    va_end(args);
    return result;
}

void CallVoidMethod(JNIEnv* env, jobject obj, jmethodID method, ...)
{
    if (!env || !obj || !method) return;
    va_list args;
    va_start(args, method);
    env->CallVoidMethodV(obj, method, args);
    va_end(args);
}

void CallVoidMethodIntInt(JNIEnv* env, jobject obj, jmethodID method, int a, int b)
{
    if (!env || !obj || !method) return;
    env->CallVoidMethod(obj, method, a, b);
}

jint CallIntMethod(JNIEnv* env, jobject obj, jmethodID method)
{
    if (!env || !obj || !method) return 0;
    return env->CallIntMethod(obj, method);
}

jint CallIntMethodInt(JNIEnv* env, jobject obj, jmethodID method, int arg)
{
    if (!env || !obj || !method) return 0;
    return env->CallIntMethod(obj, method, arg);
}

jstring NewStringUTF(JNIEnv* env, const char* utf)
{
    if (!env) return nullptr;
    return env->NewStringUTF(utf ? utf : "");
}

} // namespace AndroidJni
