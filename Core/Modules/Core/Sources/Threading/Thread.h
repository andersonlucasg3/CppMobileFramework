#pragma once

#include "Defines/Types.h"

#include "Templates/Functions.h"

#include "Object/Object.h"

#include "String/String.h"

#include "CriticalSection.h"

DECLARE_CLASS_HEADER(Thread);

class CThread : public CObject
{
    using Super = CObject;
    
public:
    CORE_API CThread() = default;
    CORE_API ~CThread() override = default;

    CORE_API virtual void SetName(const CString& Name) = 0;
    CORE_API virtual const CString& Name() const = 0;

    CORE_API virtual bool IsRunning();
    CORE_API virtual void Start(const TFunction<void(CThread*)>& ThreadFunc);
    CORE_API virtual void Join() = 0;
    CORE_API virtual void Exit();

    CORE_API virtual void Sleep(UInt64 InTimeMilliseconds) const = 0;

    CORE_API static CThread* Create();

private:
    bool _bIsRunning = false;
    SCriticalSection _isRunningSection;
};