#include "AppleFile.h"

#include "SmartPointer/MakeAndCasts.h"

#include <filesystem>

bool CAppleFile::Create()
{
    FileHandle = fopen(*FilePath, "wb+");
	return FileHandle != nullptr;
}

bool CAppleFile::Open(EOpenMode Mode)
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

bool CAppleFile::Delete()
{
	return std::filesystem::remove(*FilePath);
}

bool CAppleFile::Write(const TArray<int8_t>& ByteArray)
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

bool CAppleFile::Write(const void* Data, SizeT Size)
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

bool CAppleFile::Read(TArray<int8_t>& OutByteArray)
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

bool CAppleFile::Read(void*& OutData, UInt64& Size)
{
	if (OutData == nullptr) throw "OutData should not be nullptr";

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

bool CAppleFile::Flush()
{
	return fflush(FileHandle) > 0;
}

UInt64 CAppleFile::GetSize() const
{
	fseek(FileHandle, 0, SEEK_END);
	UInt64 FileSize = ftell(FileHandle);
	fseek(FileHandle, 0, SEEK_SET);

	return FileSize;
}

CAppleFile::CAppleFile(const CString& FilePath) :
	Super(FilePath), FileHandle(nullptr)
{
	//
}

CAppleFile::~CAppleFile()
{
	if (FileHandle != nullptr)
	{
		fclose(FileHandle);
		FileHandle = nullptr;
	}
}

CAppleFileObjectPtr CAppleFile::Create(const CString &FilePath)
{
	return new CAppleFile(FilePath);
}

CAppleFile* CAppleFile::CreateUnsafe(const char *FilePath)
{
	return new CAppleFile(FilePath);
}