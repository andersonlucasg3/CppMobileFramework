#include "GoogleResources.h"

#include "Filesystem/Android/AndroidPath.h"

const CAndroidResources& GAndroidResources = static_cast<const CAndroidResources&>(GResources);

SFileRef CAndroidResources::GetResource(const CString& InResourceName) const
{
    return GPath.Combine({
        GAndroidPath.GetFilesPath(),
        "Resources",
        InResourceName
    });
}