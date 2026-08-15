#include "GoogleResources.h"

#include "Filesystem/Google/GooglePath.h"

const CGoogleResources& GAndroidResources = static_cast<const CGoogleResources&>(GResources);

SFileRef CGoogleResources::GetResource(const CString& InResourceName) const
{
    return GPath.Combine({
        GAndroidPath.GetFilesPath(),
        "Resources",
        InResourceName
    });
}