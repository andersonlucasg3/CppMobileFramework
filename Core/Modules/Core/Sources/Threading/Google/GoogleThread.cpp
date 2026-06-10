#include "GoogleThread.h"

void CAndroidThread::SetName(const CString& Name)
{
    _threadName = Name;
}

const CString& CAndroidThread::Name() const
{
    return _threadName;
}

void CAndroidThread::Join()
{
    // TODO: implement join for android.
    // why is this need per platform?
}

CAndroidThreadPtr CAndroidThread::Create()
{
    return MakeShared<CAndroidThread>();
}