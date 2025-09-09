#pragma once

#include "Threading/Thread.h"

#include <thread>

DECLARE_CLASS_HEADER(MacThread);

class CMacThread : public CThread
{
    using Super = CThread;

    CString ThreadName;
    TSharedPtr<std::thread> Thread;

public:
    CORE_API CMacThread() = default;
    CORE_API ~CMacThread() override = default;

    CORE_API void SetName(const CString& Name) override;
    CORE_API const CString& Name() const override;

    CORE_API void Start(const TFunction<void(CThread*)>& ThreadFunc) override;
    CORE_API void Join() override;

    CORE_API void Sleep(UInt64 InTimeMilliseconds) const override;
};

typedef CMacThread CPlatformThread;