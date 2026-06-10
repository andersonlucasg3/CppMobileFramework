#pragma once

#if PLATFORM_WINDOWS

#include "Filesystem/File.h"

class CWindowsFile;
using CWindowsFilePtr = TSharedPtr<CWindowsFile>;
using CWindowsFileWeakPtr = TWeakPtr<CWindowsFile>;

class CWindowsFile : public CFile
{
	using Super = CFile;

	FILE* FileHandle;

protected:
	CORE_API bool Create() override;
	CORE_API bool Open(EOpenMode Mode) override;
	CORE_API bool Delete() override;
	CORE_API bool Write(const TArray<int8_t>& ByteArray) override;
	CORE_API bool Write(const void* Data, SizeT Size) override;
	CORE_API bool Read(TArray<int8_t>& OutByteArray) override;
	CORE_API bool Read(void*& OutData, UInt64& Size) override;
	CORE_API bool Flush() override;
	CORE_API UInt64 GetSize() const override;

public:
	CORE_API CWindowsFile(const CString& FilePath);
	CORE_API ~CWindowsFile() override;

	CORE_API static CWindowsFilePtr Create(const CString& FilePath);
	CORE_API static CWindowsFile* CreateUnsafe(const char* FilePath);
	CORE_API static bool Exists(const CString& FilePath);
};

typedef CWindowsFile CPlatformFile;

#endif // PLATFORM_WINDOWS