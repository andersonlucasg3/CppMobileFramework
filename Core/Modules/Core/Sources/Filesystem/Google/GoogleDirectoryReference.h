#pragma once

#include "Filesystem/DirectoryReference.h"

class CAndroidDirectoryReference : public CDirectoryReference
{
    using Super = CDirectoryReference;

protected:
    CORE_API void UpdateExistance() override;
    
public:
    CORE_API explicit CAndroidDirectoryReference(const CString& InPath);
    CORE_API ~CAndroidDirectoryReference() override = default;
};

typedef CAndroidDirectoryReference CPlatformDirectoryReference;