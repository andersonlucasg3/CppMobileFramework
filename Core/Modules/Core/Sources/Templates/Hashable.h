#pragma once

#include "Defines/Types.h"

using namespace Core::Types;

class CHashable
{
public:
    virtual ~CHashable() = default;
    
    virtual SizeT Hash() const = 0;
};