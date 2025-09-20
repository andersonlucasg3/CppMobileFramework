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

    template<class TOtherNativeClass = TNativeClass>
    TNativeInstance(TOtherNativeClass* Other)
    :   _native((TNativeClass*)[Other retain])
    {
        
    }

    template<class TOtherNativeClass = TNativeClass>
    TNativeInstance(TNativeInstance<TOtherNativeClass>* Other)
    :   _native((TNativeClass*)[Other->_native retain])
    {
        
    }

    ~TNativeInstance()
    {
        [_native release];
        _native = nil;
    }

    TNativeClass* Native() const
    {
        return _native;
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

    template<class TAnyNativeClass>
    friend class TNativeInstance;
};