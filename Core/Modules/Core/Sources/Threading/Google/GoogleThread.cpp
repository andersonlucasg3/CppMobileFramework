#include "GoogleThread.h"

void CGoogleThread::SetName(const CString& Name)
{
    _threadName = Name;
}

const CString& CGoogleThread::Name() const
{
    return _threadName;
}

void CGoogleThread::Join()
{
    // TODO: implement join for android.
    // why is this need per platform?
}

CGoogleThreadPtr CGoogleThread::Create()
{
    return MakeShared<CGoogleThread>();
}