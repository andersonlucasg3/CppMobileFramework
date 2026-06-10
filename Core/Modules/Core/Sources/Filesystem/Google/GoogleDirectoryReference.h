#pragma once

#include "Filesystem/DirectoryReference.h"

class CGoogleDirectoryReference : public CDirectoryReference
{
    using Super = CDirectoryReference;

protected:
    CORE_API void UpdateExistance() override;
    
public:
    CORE_API explicit CGoogleDirectoryReference(const CString& InPath);
    CORE_API ~CGoogleDirectoryReference() override = default;
};

typedef CGoogleDirectoryReference CPlatformDirectoryReference;