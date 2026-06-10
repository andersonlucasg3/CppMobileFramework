#include "MicrosoftFileReference.h"

#if PLATFORM_WINDOWS

#include <Windows.h>

void CMicrosoftFileReference::UpdateExistance()
{
    DWORD Attrs = GetFileAttributes(*_path);

    _bExists = Attrs != INVALID_FILE_ATTRIBUTES && !(Attrs & FILE_ATTRIBUTE_DIRECTORY);
}

CMicrosoftFileReference::CMicrosoftFileReference(const CString& InPath) 
:   Super(InPath)
,   _path(InPath)
{
    UpdateExistance();
}

#endif // PLATFORM_WINDOWS