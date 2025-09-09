#pragma once

#include "Threading/ScopeLock.h"
#include "Threading/CriticalSection.h"

template<typename TType>
struct TAtomic
{
public:
    TAtomic(TType Value)
    :   _value(Value)
    {
        //
    }

    TType CompareExchange(TType NewValue)
    {
        SScopeLock Lock(_criticalSection);

        TType Current = _value;
        
        if (_value != NewValue)
        {
            _value = NewValue;
        }

        return Current;
    }

    TAtomic& operator=(TType NewValue)
    {
        SScopeLock Lock(_criticalSection);

        _value = NewValue;

        return *this;
    }

    operator TType()
    {
        SScopeLock Lock(_criticalSection);

        return _value;
    }

    operator const TType() const
    {
        SScopeLock Lock(_criticalSection);

        return _value;
    }

private:
    TType _value;

    SCriticalSection _criticalSection;
};