#pragma once

#include <jni.h>
#include <android/native_window.h>

struct android_app;

/// Global JavaVM pointer set once during android_main by AndroidApplication.
/// All JNI calls throughout the framework go through this.
extern "C" JavaVM* GJavaVM;

/// Global ref to the Activity jobject, set by AndroidApplication_SetAndroidApp.
extern "C" jobject GAndroidActivityClazz;

/// Returns the android_app* for the current process (set during android_main).
/// These have C linkage so they can be forward-declared from any module.
extern "C" struct android_app* AndroidApplication_GetAndroidApp();
extern "C" void AndroidApplication_SetAndroidApp(struct android_app* app);

/// Convenience: returns the current ANativeWindow* directly (or nullptr).
/// The UI module cannot include android_native_app_glue.h, so this provides
/// a simple C-linkage bridge to access app->window.
extern "C" ANativeWindow* AndroidApplication_GetNativeWindow();

/// Show or hide the soft keyboard for the activity's current focus view.
extern "C" void AndroidApplication_ShowSoftKeyboard(bool bShow);

/// RAII scoped JNI env attachment.
/// On construction, gets a JNIEnv* for the calling thread (attaching if needed).
/// On destruction, detaches if this call attached.
struct SAndroidJniScope
{
    JNIEnv* Env;
    bool    bAttached;

    SAndroidJniScope();
    ~SAndroidJniScope();
};

namespace AndroidJni
{
    /// Convenience: find a class by name (framework classes only — does NOT use
    /// the app ClassLoader; works for android.widget.* etc.).
    jclass FindClass(JNIEnv* env, const char* className);

    /// Get a static method id.
    jmethodID GetStaticMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig);

    /// Get an instance method id.
    jmethodID GetMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig);

    /// Call a static object-returning method.
    jobject CallStaticObjectMethod(JNIEnv* env, jclass clazz, jmethodID method, ...);

    /// Call an object-returning method on an instance.
    jobject CallObjectMethod(JNIEnv* env, jobject obj, jmethodID method, ...);

    /// Call a void method on an instance.
    void CallVoidMethod(JNIEnv* env, jobject obj, jmethodID method, ...);

    /// Call a void method on an instance with int parameters.
    void CallVoidMethodIntInt(JNIEnv* env, jobject obj, jmethodID method, int a, int b);

    /// Call an int-returning method.
    jint CallIntMethod(JNIEnv* env, jobject obj, jmethodID method);

    /// Call a method that returns int with one int arg.
    jint CallIntMethodInt(JNIEnv* env, jobject obj, jmethodID method, int arg);

    /// NewStringUTF wrapper.
    jstring NewStringUTF(JNIEnv* env, const char* utf);
}
