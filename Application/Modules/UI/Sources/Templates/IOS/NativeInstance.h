#pragma once

#include <MacTypes.h>

template<class TNativeClass>
class TNativeInstance
{
public:
    TNativeInstance(TNativeClass* Native)
    :   _native([Native retain])
    {
        
    }

    ~TNativeInstance()
    {
        [_native release];
        _native = nil;
    }

    operator TNativeClass*() const
    {
        return _native;
    }

    TNativeClass* operator*() const
    {
        return _native;
    }

    TNativeClass* operator->() const
    {
        return _native;
    }

private:
    TNativeClass* _native;
};