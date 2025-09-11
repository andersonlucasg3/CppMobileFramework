#pragma once

// TODO: remove this if unnecessary

#include <type_traits>

class CObject;

class CClass
{
    // maybe in the future
};

template<class TObject = CObject>
class TClass : public CClass
{
    static_assert(std::is_base_of_v<CObject, TObject>);
};