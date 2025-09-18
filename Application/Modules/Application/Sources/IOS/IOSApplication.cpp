#include "IOSApplication.h"

#include "ObjC/ObjCMain.h"

int CIOSApplication::Run(int argc, char* argv[])
{
    // true, non-zero result here is failure
    if (int Result = Super::Run(argc, argv))
    {
        return Result;
    }
    
    return CallMain(argc, argv);
}

