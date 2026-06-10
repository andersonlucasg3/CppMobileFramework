#pragma once

#include "Filesystem/FileReference.h"

class CAndroidFileReference : public CFileReference
{
    using Super = CFileReference;

protected:
    CORE_API void UpdateExistance() override;

public:
    CORE_API CAndroidFileReference(const CString& InPath);
    CORE_API ~CAndroidFileReference() override = default;
};

typedef CAndroidFileReference CPlatformFileReference;