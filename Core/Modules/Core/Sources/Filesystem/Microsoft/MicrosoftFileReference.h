#pragma once

#if PLATFORM_WINDOWS

#include "Filesystem/FileReference.h"

class CMicrosoftFileReference : public CFileReference
{
    using Super = CFileReference;

    CString _path;

    CORE_API void UpdateExistance() override;

public:
    explicit CORE_API CMicrosoftFileReference(const CString& InPath);
    CORE_API ~CMicrosoftFileReference() override = default;
};

typedef CMicrosoftFileReference CPlatformFileReference;

#endif // PLATFORM_WINDOWS