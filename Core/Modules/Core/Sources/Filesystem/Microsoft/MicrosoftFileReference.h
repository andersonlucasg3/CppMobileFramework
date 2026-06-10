#pragma once

#if PLATFORM_WINDOWS

#include "Filesystem/FileReference.h"

class CWindowsFileReference : public CFileReference
{
    using Super = CFileReference;

    CString _path;

    CORE_API void UpdateExistance() override;

public:
    explicit CORE_API CWindowsFileReference(const CString& InPath);
    CORE_API ~CWindowsFileReference() override = default;
};

typedef CWindowsFileReference CPlatformFileReference;

#endif // PLATFORM_WINDOWS