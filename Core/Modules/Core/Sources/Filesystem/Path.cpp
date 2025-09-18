#include "Path.h"

#include "Preprocessors.h"

#include "Templates/Array.h"

#include COMPILE_PLATFORM_GROUP_HEADER(Path.h)

static const CPlatformPath GPlatformPath;
const CPath& GPath = GPlatformPath;

CString CPath::Combine(const TArray<CString>& InPathComponents) const
{
    CString FullPath = InPathComponents[0];
    for (UInt64 Index = 1; Index < InPathComponents.Num(); ++Index)
    {
        if (FullPath.IsEmpty() || FullPath.LastChar() != PathSeparator())
        {
            FullPath += PathSeparator();
        }

        FullPath += InPathComponents[Index];
    }
    return FullPath;
}

CString CPath::LastPathComponent(const CString& InPath) const
{
    if (InPath.IsEmpty())
    {
        return InPath;
    }

    UInt64 LastSlashIndex = InPath.LastIndexOf(PathSeparator());
    
    if (LastSlashIndex == -1ul)
    {
        return InPath; // No slashes, return the whole path
    }

    return InPath.SubString(LastSlashIndex + 1);
}

CString CPath::RemoveLastPathComponent(const CString& Path) const
{
    UInt64 LastSlashIndex = Path.LastIndexOf(PathSeparator());
    if (LastSlashIndex == -1ul)
    {
        return Path;
    }
    return Path.SubString(0, LastSlashIndex);
}