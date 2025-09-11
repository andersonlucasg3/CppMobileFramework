#pragma once

#include "Threading/Thread.h"

#include "Object/ClassMacros.h"

#include <thread>

FORWARD_DECLARE_OBJECT(MacThread);

class CMacThread : public CThread
{
    using Super = CThread;

public:
    CORE_API CMacThread() = default;
    CORE_API ~CMacThread() override = default;

    CORE_API void SetName(const CString& Name) override;
    CORE_API const CString& Name() const override;

    CORE_API void Join() override;

    CORE_API void Sleep(UInt64 InTimeMilliseconds) const override;

private:
    CString _threadName;
    TSharedPtr<std::thread> _thread;

    CORE_API void StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc) override;
};

typedef CMacThread CPlatformThread;
FORWARD_DECLARE_OBJECT_PTR(PlatformThread);