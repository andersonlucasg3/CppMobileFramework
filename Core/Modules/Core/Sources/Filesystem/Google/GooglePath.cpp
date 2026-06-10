#include "GooglePath.h"

#include "Templates/Array.h"

#ifndef ANDROID_APPLICATION_PACKAGE
#define ANDROID_APPLICATION_PACKAGE "com.app.missing"
#warning "ANDROID_APPLICATION_PACKAGE not defined yet"
#endif // ANDROID_APPLICATION_PACKAGE

const CGooglePath& GAndroidPath = static_cast<const CGooglePath&>(GPath);

CGooglePath::CGooglePath()
{
    _filesPath = Combine({
        "/",
        "sdcard",
        "Android",
        "data",
        ANDROID_APPLICATION_PACKAGE,
        "files"
    });

    _cachesPath = Combine({ _filesPath, "Caches" });
}

char CGooglePath::PathSeparator() const
{
    return '/';
}

CString CGooglePath::GetFullPath(const CString& InPath) const
{
    return InPath;
}

CString CGooglePath::GetPathRoot(const CString& InPath) const
{
    if (InPath.IsEmpty())
    {
        return InPath;
    }

    if (InPath[0] == '/')
    {
        return "/";
    }

    return "";
}

const CString& CGooglePath::CachesPath() const
{
    return _cachesPath;
}

const CString& CGooglePath::GetFilesPath() const
{
    return _filesPath;
}