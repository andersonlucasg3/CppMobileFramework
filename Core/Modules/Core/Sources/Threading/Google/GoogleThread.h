#pragma once

#include "Object/ClassMacros.h"

#include "Threading/Thread.h"

#include "String/String.h"

#include <thread>

class CGoogleThread;
typedef TSharedPtr<CGoogleThread> CGoogleThreadPtr;

class CGoogleThread : public CThread
{
    using Super = CThread;
    
    CString _threadName;
    TSharedPtr<std::thread> Thread;

public:
    CORE_API ~CGoogleThread() override = default;

    CORE_API void SetName(const CString& Name) override;
    CORE_API const CString& Name() const override;

    CORE_API void Join() override;
    CORE_API void Sleep(UInt64 InTimeMilliseconds) const override;

    CORE_API static CGoogleThreadPtr Create();

private:
    CORE_API void StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc) override;
};

typedef CGoogleThread CPlatformThread;
FORWARD_DECLARE_OBJECT_PTR(PlatformThread);