#pragma once

#include <mutex>

struct SCriticalSection
{
    CORE_API SCriticalSection() = default;
    CORE_API virtual ~SCriticalSection() = default;

    CORE_API virtual void Lock() const;
    CORE_API virtual bool TryLock() const;
    CORE_API virtual void Unlock() const;

private:
    mutable std::mutex _mutex;
};