#pragma once

#include "Filesystem/FileReference.h"

#include "SmartPointer/SharedFromThis.h"

#include "Object/ClassMacros.h"

#include "NSSharedPtr.hpp"
#include "NSString.h"
#include "URL/NSURL.h"

FORWARD_DECLARE_SHARED(AppleFileReference);

class CAppleFileReference : public CFileReference
{
    using Super = CFileReference;

    NS::SharedPtr<NS::URL> _fileURL;
    
    CORE_API void UpdateExistance() override;
    
public:
    CORE_API CAppleFileReference(const CString& InPath);
    CORE_API ~CAppleFileReference() override = default;

    CORE_API NS::String* PathString() const;
    CORE_API NS::URL* PathURL() const;
};

typedef CAppleFileReference CPlatformFileReference;