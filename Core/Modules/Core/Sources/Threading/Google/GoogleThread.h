#pragma once

#include "Object/ClassMacros.h"

#include "Threading/Thread.h"

#include "String/String.h"

DECLARE_CLASS_HEADER(AndroidThread);

class CAndroidThread : public CThread
{
    using Super = CThread;
    
    CString _threadName;

public:
    CORE_API ~CAndroidThread() override = default;

    CORE_API void SetName(const CString& Name) override;
    CORE_API const CString& Name() const override;

    CORE_API void Join() override;

    CORE_API static CAndroidThreadPtr Create();
};

typedef CAndroidThread CPlatformThread;