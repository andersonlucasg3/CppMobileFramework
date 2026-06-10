#include "MicrosoftFileReference.h"

#if PLATFORM_WINDOWS

#include <Windows.h>

void CWindowsFileReference::UpdateExistance()
{
    DWORD Attrs = GetFileAttributes(*_path);

    _bExists = Attrs != INVALID_FILE_ATTRIBUTES && !(Attrs & FILE_ATTRIBUTE_DIRECTORY);
}

CWindowsFileReference::CWindowsFileReference(const CString& InPath) 
:   Super(InPath)
,   _path(InPath)
{
    UpdateExistance();
}

#endif // PLATFORM_WINDOWS