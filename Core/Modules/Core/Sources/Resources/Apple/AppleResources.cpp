#include "AppleResources.h"

#include "Filesystem/DirectoryReference.h"
#include "Filesystem/FileReference.h"

#include COMPILE_PLATFORM_HEADER_FEATURE(Process, Process.h)

SFileRef CAppleResources::GetResource(const CString& InResourceName) const
{
    SDirectoryRef ContainerDirectory = GAppleProcess.GetContainerPath();

    return ContainerDirectory->CombineFile({
        GAppleProcess.IsRunningInsideAppBundle() ? "../Resources" : "Resources",
        InResourceName
    });
}