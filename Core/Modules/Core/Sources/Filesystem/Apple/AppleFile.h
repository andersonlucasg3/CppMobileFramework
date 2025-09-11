#pragma once

#include "Templates/Array.h"

#include "Filesystem/File.h"

#include "Object/ClassMacros.h"

#include <cstdio>

FORWARD_DECLARE_OBJECT(AppleFile);

class CAppleFile : public CFile
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
    CORE_API CAppleFile(const CString& FilePath);
    CORE_API ~CAppleFile() override;

    CORE_API static CAppleFileObjectPtr Create(const CString& FilePath);
    CORE_API static CAppleFile* CreateUnsafe(const char* FilePath);
    CORE_API static bool Exists(const CString& FilePath);
};