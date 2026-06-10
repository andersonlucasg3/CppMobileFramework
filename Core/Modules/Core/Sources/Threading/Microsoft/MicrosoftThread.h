#pragma once

#include "Threading/Thread.h"

#include "Object/ClassMacros.h"

#include <thread>

FORWARD_DECLARE_OBJECT(WindowsThread);

class CMicrosoftThread : public CThread
{
    using Super = CThread;

    CString _name;
    TSharedPtr<std::thread> Thread;
    bool bIsRunning = false;

public:
    CORE_API CMicrosoftThread() = default;
    CORE_API ~CMicrosoftThread() override = default;

    CORE_API void SetName(const CString& Name) override;
    CORE_API const CString& Name() const override;

    CORE_API void Join() override;

    CORE_API void Sleep(UInt64 InTimeInMilliseconds) const override;

private:
    CORE_API void StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc) override;
};

typedef CMicrosoftThread CPlatformThread;
FORWARD_DECLARE_OBJECT_PTR(PlatformThread);