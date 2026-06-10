#include "MicrosoftThread.h"
#include "Threading/Thread.h"

#include <Windows.h>

#include <processthreadsapi.h>
#include <synchapi.h>

void CMicrosoftThread::SetName(const CString& Name)
{
	this->_name = Name;
}

const CString& CMicrosoftThread::Name() const
{
	return _name;
}

void CMicrosoftThread::Join()
{
	if (Thread && Thread->joinable())
	{
		Thread->join();
	}
}

void CMicrosoftThread::Sleep(UInt64 InTimeMilliseconds) const
{
	::Sleep(InTimeMilliseconds);
}

void CMicrosoftThread::StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc)
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