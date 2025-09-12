#include "OperationQueue.h"
#include "NSObject.hpp"

namespace NS
{
    OperationQueue* OperationQueue::alloc()
    {
        return Object::alloc<OperationQueue>(_NS_PRIVATE_CLS(NSOperationQueue));
    }

    OperationQueue* OperationQueue::init()
    {
        return Object::init<OperationQueue>();
    }
}