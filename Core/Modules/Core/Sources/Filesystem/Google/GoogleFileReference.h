#pragma once

#include "Filesystem/FileReference.h"

class CGoogleFileReference : public CFileReference
{
    using Super = CFileReference;

protected:
    CORE_API void UpdateExistance() override;

public:
    CORE_API CGoogleFileReference(const CString& InPath);
    CORE_API ~CGoogleFileReference() override = default;
};

typedef CGoogleFileReference CPlatformFileReference;