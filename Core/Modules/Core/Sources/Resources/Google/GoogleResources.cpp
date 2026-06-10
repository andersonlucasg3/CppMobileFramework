#include "GoogleResources.h"

#include "Filesystem/Android/AndroidPath.h"

const CGoogleResources& GAndroidResources = static_cast<const CGoogleResources&>(GResources);

SFileRef CGoogleResources::GetResource(const CString& InResourceName) const
{
    return GPath.Combine({
        GAndroidPath.GetFilesPath(),
        "Resources",
        InResourceName
    });
}