#include "MicrosoftFile.h"

#if PLATFORM_WINDOWS

#include "Defines/Asserts.h"

#include <filesystem>

bool CWindowsFile::Create()
{
	FileHandle = _fsopen(*FilePath, "wb+", _SH_DENYNO);
	return FileHandle != nullptr;
}

bool CWindowsFile::Open(EOpenMode Mode)
{
	const char* ModeStr = nullptr;
	switch (Mode)
	{
	case EOpenMode::OpenRead: ModeStr = "rb"; break;
	case EOpenMode::OpenWrite: ModeStr = "ab"; break;
	case EOpenMode::OpenReadWrite: ModeStr = "ab+"; break;
	default:
		return false;
	}

	FileHandle = _fsopen(*FilePath, ModeStr, _SH_DENYNO);
	return FileHandle != nullptr;
}

bool CWindowsFile::Delete()
{
	return std::filesystem::remove(*FilePath);
}

bool CWindowsFile::Write(const TArray<int8_t>& ByteArray)
{
	UInt64 BytesWritten = 0;
	do
	{
		const int8_t* Buffer = (ByteArray.GetData() + BytesWritten);
		const UInt64 RemainingSize = ByteArray.Num() - BytesWritten;

		UInt64 Written = fwrite(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Written < 0) return false;

		BytesWritten += Written;

	} 
	while (BytesWritten < ByteArray.Num());

	return BytesWritten == ByteArray.Num();
}

bool CWindowsFile::Write(const void* Data, SizeT Size)
{
	UInt64 BytesWritten = 0;
	do
	{
		const UByte* Buffer = ((const UByte*)Data + BytesWritten);
		const UInt64 RemainingSize = Size - BytesWritten;

		UInt64 Written = fwrite(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Written < 0) return false;

		BytesWritten += Written;

	} 
	while (BytesWritten < Size);

	return BytesWritten == Size;
}

bool CWindowsFile::Read(TArray<int8_t>& OutByteArray)
{
	fseek(FileHandle, 0, SEEK_END);
	UInt64 FileSize = ftell(FileHandle);
	fseek(FileHandle, 0, SEEK_SET);

	OutByteArray.Resize(FileSize, true);

	UInt64 BytesRead = 0;
	do
	{
		void* Buffer = ((int8_t*)OutByteArray.GetData() + BytesRead);
		const UInt64 RemainingSize = OutByteArray.Num() - BytesRead;

		UInt64 Read = fread(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Read < 0) return false;

		BytesRead += Read;

	} while (BytesRead < OutByteArray.Num());

	return BytesRead == OutByteArray.Num();
}

bool CWindowsFile::Read(void*& OutData, UInt64& Size)
{
	assert(OutData != nullptr);

	fseek(FileHandle, 0, SEEK_END);
	// TODO: check why this is not used (compare to apple implementation)
	UInt64 FileSize = ftell(FileHandle);
	fseek(FileHandle, 0, SEEK_SET);

	UInt64 BytesRead = 0;
	do
	{
		int8_t* Buffer = ((int8_t*)OutData + BytesRead);
		const UInt64 RemainingSize = Size - BytesRead;

		UInt64 Read = fread(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Read < 0) return false;

		BytesRead += Read;

	} while (BytesRead < Size);

	return BytesRead == Size;
}

bool CWindowsFile::Flush()
{
	return fflush(FileHandle) > 0;
}

UInt64 CWindowsFile::GetSize() const
{
	fseek(FileHandle, 0, SEEK_END);
	UInt64 FileSize = ftell(FileHandle);
	fseek(FileHandle, 0, SEEK_SET);

	return FileSize;
}

CWindowsFile::CWindowsFile(const CString& FilePath) : Super(FilePath), FileHandle(nullptr)
{
	//
}

CWindowsFile::~CWindowsFile()
{
	if (FileHandle != nullptr)
	{
		fclose(FileHandle);
		FileHandle = nullptr;
	}
}

CWindowsFilePtr CWindowsFile::Create(const CString& FilePath)
{
	return MakeShareable(new CWindowsFile(FilePath));
}

CWindowsFile* CWindowsFile::CreateUnsafe(const char* FilePath)
{
	return new CWindowsFile(FilePath);
}

bool CWindowsFile::Exists(const CString& FilePath)
{
	return std::filesystem::exists(*FilePath);
}

#endif // PLATFORM_WINDOWS