#pragma once

#include "Filesystem/Path.h"

class CGooglePath : public CPath
{
    CString _filesPath;
    CString _cachesPath;

public:
    CORE_API CGooglePath();
    CORE_API ~CGooglePath() override = default;

    CORE_API char PathSeparator() const override;

    CORE_API CString GetFullPath(const CString& InPath) const override;
    CORE_API CString GetPathRoot(const CString& InPath) const override;
    
    CORE_API const CString& CachesPath() const override;
    CORE_API const CString& GetFilesPath() const;
};

typedef CGooglePath CPlatformPath;

extern const CGooglePath& GAndroidPath;