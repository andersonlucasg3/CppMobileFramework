#pragma once

#include "Filesystem/Path.h"

class CApplePath : public CPath
{
    CString _cachesPath;

public:
    CORE_API CApplePath() = default;
    CORE_API ~CApplePath() override = default;

    CORE_API char PathSeparator() const override;

    CORE_API CString GetFullPath(const CString& InPath) const override;
    CORE_API CString GetPathRoot(const CString& InPath) const override;
    
    CORE_API const CString& CachesPath() const override;
};

typedef CApplePath CPlatformPath;