#include "WindowsPath.h"

#include "Templates/Array.h"

#include "Process/Desktop/DesktopProcess.h"

#include "Filesystem/Path.h"

#include <Windows.h>
#include <shlobj.h>

CString CWindowsPath::FixPath(const CString& Path)
{
    return Path.Replace('/', '\\');
}

char CWindowsPath::PathSeparator() const
{
    return '\\';
}

CString CWindowsPath::GetFullPath(const CString& InPath) const
{
    CString Path = InPath;

    if (Path.IsEmpty())
    {
        return Path;
    }

    if (Path.Len() < 2 || Path[1] != ':')
    {
        const CString& WorkingDirectory = GProcess.GetContainerPath();
        Path = GPath.Combine({ WorkingDirectory, InPath });
    }

    char FullPath[MAX_PATH];
    DWORD Length = GetFullPathName(*Path, MAX_PATH, FullPath, nullptr);

    if (Length == 0 || Length > MAX_PATH)
    {
        return Path;
    }

    return CString(FullPath, static_cast<UInt64>(Length));
}

CString CWindowsPath::GetPathRoot(const CString& InPath) const
{
    CString Path = InPath;

    if (Path.IsEmpty())
    {
        return Path;
    }

    if (Path == ".")
    {
        Path = GDesktopProcess.GetContainerPath();
    }
    else if (Path[1] != ':')
    {
        Path = GPath.Combine({ GDesktopProcess.GetContainerPath(), Path });
    }

    Path = GetFullPath(Path);

    return Path.SubString(0, 3);
}

CString CWindowsPath::Combine(const TArray<CString>& PathComponents) const
{
    CString FullPath = FixPath(PathComponents[0]);
    for (UInt32 Index = 1; Index < PathComponents.Num(); ++Index)
    {
        FullPath += "\\";
        FullPath += FixPath(PathComponents[Index]);
    }
    return FullPath;
}

CString CWindowsPath::LastPathComponent(const CString& InPath) const
{
    Int64 LastSlashIndex = InPath.LastIndexOf('\\');
    if (LastSlashIndex == -1)
    {
        return InPath;
    }
    return InPath.SubString(LastSlashIndex, InPath.Len());
}

CString CWindowsPath::RemoveLastPathComponent(const CString& InPath) const
{
    Int64 LastSlashIndex = InPath.LastIndexOf('\\');
    if (LastSlashIndex == -1)
    {
        return InPath;
    }
    return InPath.SubString(0, LastSlashIndex);
}

const CString& CWindowsPath::CachesPath() const
{
    static CString CachesPath = []
    {
        PWSTR Path = NULL;
        HRESULT HResult = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &Path);

        if (SUCCEEDED(HResult))
        {
            WCHAR FullPath[MAX_PATH];
            wcscpy_s(FullPath, Path);

            CString FullPathString(FullPath, wcslen(FullPath));

            CoTaskMemFree(Path);

            return GPath.Combine({
                FullPathString,
                GPath.LastPathComponent(GDesktopProcess.GetExecutablePath()), // executable name
                "Cache"
            });
        }

        throw "Failed to setup caches path";
    }();

    return CachesPath;
}