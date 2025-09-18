#pragma once

// TODO: remove this if unnecessary

#include "Object.h"
#include "ClassMacros.h"

#include <type_traits>

class CObject;

FORWARD_DECLARE_OBJECT(Class);

class CClass : public CObject
{
    using Super = CObject;

public:
    CClass() = default;
    virtual ~CClass() = default;

    
};

template<class TObject = CObject>
class TClass : public CClass
{
    static_assert(std::is_base_of_v<CObject, TObject>);

public:
    TClass() = default;
    ~TClass() override = default;
};

template<class TSuperClass = CObject>
class TSubclassOf
{
public:
    TSubclassOf() = default;
    ~TSubclassOf() = default;

    template<class TSubClass = CObject>
    TSubclassOf(TClass<TSubClass>* OtherClass)
    {
        static_assert(std::is_base_of_v<TSuperClass, TSubClass>);

        _subclass = OtherClass;
    }

    TClass<TSuperClass>* GetClass() const
    {
        return _subclass;
    }

private:
    TObjectPtr<TClass<TSuperClass>> _subclass;
};