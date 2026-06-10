#include "MicrosoftThread.h"
#include "Threading/Thread.h"

#include <Windows.h>

#include <processthreadsapi.h>
#include <synchapi.h>

void CWindowsThread::SetName(const CString& Name)
{
	this->_name = Name;
}

const CString& CWindowsThread::Name() const
{
	return _name;
}

void CWindowsThread::Join()
{
	if (Thread && Thread->joinable())
	{
		Thread->join();
	}
}

void CWindowsThread::Sleep(UInt64 InTimeMilliseconds) const
{
	::Sleep(InTimeMilliseconds);
}

void CWindowsThread::StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc)
{
	if (!Thread)
	{
		Thread = MakeShared<std::thread>([this, ThreadFunc]
		{
			while (IsRunning())
			{
				ThreadFunc(this);
			}
		});

		if (!_name.IsEmpty())
		{
			SetThreadDescription(Thread->native_handle(), _name.WStr().Raw());
		}
	}
}