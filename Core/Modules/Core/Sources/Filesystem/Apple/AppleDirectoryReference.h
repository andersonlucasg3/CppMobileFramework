#pragma once

#include "Filesystem/DirectoryReference.h"

#include "NSString.h"
#include "NSSharedPtr.hpp"
#include "URL/NSURL.h"

class CAppleDirectoryReference : public CDirectoryReference
{
    using Super = CDirectoryReference;

    NS::SharedPtr<NS::URL> _directoryURL;
    
    CORE_API void UpdateExistance() override;
    
public:
    CORE_API CAppleDirectoryReference(const CString& InPath);
    CORE_API ~CAppleDirectoryReference() override = default;

    CORE_API bool Create(bool InCreateIntermediates = false) override;
    CORE_API bool Delete() override;

    CORE_API NS::String* PathString() const;
    CORE_API NS::URL* PathURL() const;
};

typedef CAppleDirectoryReference CPlatformDirectoryReference;