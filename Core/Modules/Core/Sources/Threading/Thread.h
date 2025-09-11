#pragma once

#include "Defines/Types.h"

#include "Templates/Functions.h"

#include "String/String.h"

#include "Object/Object.h"
#include "Object/ObjectPtr.h"
#include "Object/ClassMacros.h"

#include "CriticalSection.h"

FORWARD_DECLARE_OBJECT(Thread);

class CThread : public CObject
{
public:
    CORE_API CThread() = default;
    CORE_API ~CThread() override = default;

    CORE_API virtual void SetName(const CString& Name) = 0;
    CORE_API virtual const CString& Name() const = 0;

    CORE_API void Start(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc);
    CORE_API virtual void Join() = 0;
    CORE_API void Exit();

    CORE_API virtual bool IsRunning() const;
    CORE_API virtual void Sleep(UInt64 InTimeMilliseconds) const = 0;

    CORE_API static CThreadObjectPtr Create();
    CORE_API static CThread* Current();
    CORE_API static bool IsMainThread();

protected:
    CORE_API virtual void StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc) = 0;

private:
    bool _bIsRunning = false;
    SCriticalSection _isRunningSection;
};