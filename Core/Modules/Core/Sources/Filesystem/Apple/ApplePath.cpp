#include "ApplePath.h"

#include "Preprocessors.h"

#include "Templates/Array.h"

#include "String/Apple/AppleStringConvertion.h"

#include COMPILE_PLATFORM_HEADER_FEATURE(Process, Process.h)

#include "NSFileManager.h"
#include "NSError.h"
#include "URL/NSURL.h"

char CApplePath::PathSeparator() const
{
    return '/';
}

CString CApplePath::GetFullPath(const CString& InPath) const
{
    CString Path = InPath;

    if (Path.IsEmpty())
    {
        return Path;
    }

    if (Path[0] != '/')
    {
        const CString& WorkingDirectory = GProcess.GetContainerPath();
        Path = GPath.Combine({ WorkingDirectory, InPath });    
    }

    NS::String* FullPathString = CStringToNSString(Path)->stringByResolvingSymlinksInPath();
    
    return CString(FullPathString->fileSystemRepresentation());
}

CString CApplePath::GetPathRoot(const CString& InPath) const
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

const CString& CApplePath::CachesPath() const
{
    static CString CachesPath = []
    {
        if (GAppleProcess.IsRunningInsideAppBundle())
        {
            NS::Error* Error = nullptr;

            NS::URL* CachesDirectory = NS::FileManager::defaultManager()->urlForDirectory(NS::CachesDirectory, NS::UserDomainMask, nullptr, true, &Error);

            return CString(CachesDirectory->fileSystemRepresentation());
        }
        else
        {
            return GPath.Combine({
                GAppleProcess.GetContainerPath(),
                "Caches",
            });
        }
    }();
    return CachesPath;
}
