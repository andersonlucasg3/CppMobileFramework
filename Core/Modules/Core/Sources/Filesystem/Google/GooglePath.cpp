#include "GooglePath.h"

#include "Templates/Array.h"

#ifndef ANDROID_APPLICATION_PACKAGE
#define ANDROID_APPLICATION_PACKAGE "com.app.missing"
#warning "ANDROID_APPLICATION_PACKAGE not defined yet"
#endif // ANDROID_APPLICATION_PACKAGE

const CAndroidPath& GAndroidPath = static_cast<const CAndroidPath&>(GPath);

CAndroidPath::CAndroidPath()
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

char CAndroidPath::PathSeparator() const
{
    return '/';
}

CString CAndroidPath::GetFullPath(const CString& InPath) const
{
    return InPath;
}

CString CAndroidPath::GetPathRoot(const CString& InPath) const
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

const CString& CAndroidPath::CachesPath() const
{
    return _cachesPath;
}

const CString& CAndroidPath::GetFilesPath() const
{
    return _filesPath;
}