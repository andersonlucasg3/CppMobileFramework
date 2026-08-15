#include "GoogleFile.h"

#include <cassert>
#include <filesystem>

bool CGoogleFile::Create()
{
    FileHandle = fopen(*FilePath, "wb+");
	return FileHandle != nullptr;
}

bool CGoogleFile::Open(EOpenMode Mode)
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

    FileHandle = fopen(*FilePath, ModeStr);
	return FileHandle != nullptr;
}

bool CGoogleFile::Delete()
{
	return std::filesystem::remove(*FilePath);
}

CGoogleFile::CGoogleFile(const CString& FilePath) :
	Super(FilePath), FileHandle(nullptr)
{
	//
}

CGoogleFile::~CGoogleFile()
{
	if (FileHandle != nullptr)
	{
		fclose(FileHandle);
		FileHandle = nullptr;
	}
}

bool CGoogleFile::Write(const TArray<int8_t>& ByteArray)
{
	UInt64 BytesWritten = 0;
	do
	{
		const void* Buffer = (ByteArray.GetData() + BytesWritten);
		const UInt64 RemainingSize = ByteArray.Num() - BytesWritten;

		UInt64 Written = fwrite(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Written < 0) return false;

		BytesWritten += Written;

	} 
	while (BytesWritten < ByteArray.Num());

	return BytesWritten == ByteArray.Num();
}

bool CGoogleFile::Write(const void* Data, SizeT Size)
{
	UInt64 BytesWritten = 0;
	do
	{
		const void* Buffer = ((const int8_t*)Data + BytesWritten);
		const UInt64 RemainingSize = Size - BytesWritten;

		UInt64 Written = fwrite(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Written < 0) return false;

		BytesWritten += Written;

	} 
	while (BytesWritten < Size);

	return BytesWritten == Size;
}

bool CGoogleFile::Read(TArray<int8_t>& OutByteArray)
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

	} 
	while (BytesRead < OutByteArray.Num());

	return BytesRead == OutByteArray.Num();
}

bool CGoogleFile::Read(void*& OutData, UInt64& Size)
{
	assert(OutData != nullptr);

	UInt64 BytesRead = 0;
	do
	{
		int8_t* Buffer = ((int8_t*)OutData + BytesRead);
		const UInt64 RemainingSize = Size - BytesRead;

		UInt64 Read = fread(Buffer, sizeof(int8_t), RemainingSize, FileHandle);

		if (Read < 0) return false;

		BytesRead += Read;

	} 
	while (BytesRead < Size);

	return BytesRead == Size;
}

bool CGoogleFile::Flush()
{
	return fflush(FileHandle) > 0;
}

UInt64 CGoogleFile::GetSize() const
{
	fseek(FileHandle, 0, SEEK_END);
	UInt64 FileSize = ftell(FileHandle);
	fseek(FileHandle, 0, SEEK_SET);

	return FileSize;
}