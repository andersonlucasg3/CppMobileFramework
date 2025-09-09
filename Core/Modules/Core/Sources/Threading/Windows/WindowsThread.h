#pragma once

#include "Threading/Thread.h"

#include <thread>

DECLARE_CLASS_HEADER(WindowsThread);

class CWindowsThread : public CThread
{
    using Super = CThread;

    CString _name;
    TSharedPtr<std::thread> Thread;
    bool bIsRunning = false;

public:
    CORE_API CWindowsThread() = default;
    CORE_API ~CWindowsThread() override = default;

    CORE_API void SetName(const CString& Name) override;
    CORE_API const CString& Name() const override;

    CORE_API void Join() override;

    CORE_API void Sleep(UInt64 InTimeInMilliseconds) const override;

private:
    CORE_API void StartInternal(const TFunction<void(const CThreadWeakPtr&)>& ThreadFunc) override;
};

typedef CWindowsThread CPlatformThread;