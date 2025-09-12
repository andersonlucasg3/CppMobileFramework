#pragma once

#include "NSObject.hpp"
namespace NS
{
    class OperationQueue : public Copying<OperationQueue>
    {
    public:
        FOUNDATION_API static OperationQueue* alloc();
        
        FOUNDATION_API OperationQueue* init();
    };
}