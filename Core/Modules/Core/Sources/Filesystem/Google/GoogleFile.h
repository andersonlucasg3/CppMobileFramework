#pragma once

#include "Filesystem/File.h"

class CAndroidFile : public CFile
{
    using Super = CFile;

    FILE* FileHandle;

protected:
    CORE_API bool Create() override;
	CORE_API bool Open(EOpenMode Mode) override;
	CORE_API bool Delete() override;

public:
    CORE_API CAndroidFile(const CString& FilePath);
	CORE_API ~CAndroidFile() override;

    CORE_API bool Write(const TArray<int8_t>& ByteArray) override;
	CORE_API bool Write(const void* Data, SizeT Size) override;
	CORE_API bool Read(TArray<int8_t>& OutByteArray) override;
	CORE_API bool Read(void*& OutData, UInt64& Size) override;
	CORE_API bool Flush() override;
	CORE_API UInt64 GetSize() const override;
};

typedef CAndroidFile CPlatformFile;