#include "Application.h"

CApplication* CApplication::_shared = nullptr;

int CApplication::Run(int, char*[])
{
    _shared = this;

    // will be handled by platform overrides
    return 0;
}

CApplication* CApplication::Shared()
{
    return _shared;
}