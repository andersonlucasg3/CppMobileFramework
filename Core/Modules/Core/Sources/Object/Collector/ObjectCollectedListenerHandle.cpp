#include "ObjectCollectedListenerHandle.h"

bool SObjectCollectedListenerHandle::IsValid() const
{
	return _handle >= 0;
}

SizeT SObjectCollectedListenerHandle::Hash() const
{
	return _handle;
}

bool SObjectCollectedListenerHandle::operator==(const SObjectCollectedListenerHandle& Other) const
{
	return _handle == Other._handle;
}

SObjectCollectedListenerHandle::SObjectCollectedListenerHandle(Int64 Handle)
:	_handle(Handle)
{

}

void SObjectCollectedListenerHandle::Invalidate()
{
	_handle = -1;
}

SObjectCollectedListenerHandle::operator Int64() const
{
	return _handle;
}