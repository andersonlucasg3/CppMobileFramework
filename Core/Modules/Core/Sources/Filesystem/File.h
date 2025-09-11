#pragma once

#include "Templates/Array.h"

#include "String/String.h"

#include "Object/Object.h"
#include "Object/ClassMacros.h"
#include "Object/ObjectPtr.h"

FORWARD_DECLARE_OBJECT(File);

class CFile : public CObject
{
public:
	enum EOpenMode : UByte
	{
		OpenRead,
		OpenWrite,
		OpenReadWrite
	};

protected:
	CString FilePath;

	CORE_API virtual bool Create() = 0;
	CORE_API virtual bool Open(EOpenMode Mode) = 0;
	CORE_API virtual bool Delete() = 0;

	CORE_API CFile(const CString& FilePath);

public:
	CORE_API virtual ~CFile() = default;

	CORE_API static CFileObjectPtr Create(const CString& FilePath);
	CORE_API static CFile* CreateUnsafe(const char* FilePath); // will be collected in the end of garbage scope
	CORE_API static CFileObjectPtr Open(const CString& FilePath, EOpenMode Mode);
	CORE_API static CFile* OpenUnsafe(const char* FilePath, EOpenMode Mode); // will be collected in the end of garbage scope
	CORE_API static bool Delete(const CString& FilePath);
	CORE_API static bool Exists(const CString& FilePath);

	CORE_API virtual bool Write(const TArray<int8_t>& ByteArray) = 0;
	CORE_API virtual bool Write(const void* Data, SizeT Size) = 0;
	CORE_API virtual bool Read(TArray<int8_t>& OutByteArray) = 0;
	CORE_API virtual bool Read(void*& OutData, UInt64& Size) = 0;
	CORE_API virtual bool Flush() = 0;
	CORE_API virtual UInt64 GetSize() const = 0;
};
