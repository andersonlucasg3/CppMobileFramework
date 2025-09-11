#include "File.h"

#include "Defines/Preprocessors.h"

#include <filesystem>

#include COMPILE_PLATFORM_HEADER(File.h)

CFile::CFile(const CString& InFilePath) :
	FilePath(InFilePath)
{

}

CFileObjectPtr CFile::Create(const CString& FilePath)
{
	CFile* File = new CPlatformFile(FilePath);
	if (!File->Create()) return nullptr;
	return File;
}

CFile* CFile::CreateUnsafe(const char* FilePath)
{
	CFile* FilePtr = CPlatformFile::CreateUnsafe(FilePath);
	if (!FilePtr->Create()) return nullptr;
	return FilePtr;
}

CFileObjectPtr CFile::Open(const CString& FilePath, EOpenMode Mode)
{
	CFile* File = new CPlatformFile(FilePath);
	if (!File->Open(Mode)) return nullptr;
	return File;
}

CFile* CFile::OpenUnsafe(const char* FilePath, EOpenMode Mode)
{
	CFile* FilePtr = CPlatformFile::CreateUnsafe(FilePath);
	if (!FilePtr->Open(Mode)) return nullptr;
	return FilePtr;
}

bool CFile::Delete(const CString& FilePath)
{
	CFile* File = CreateUnsafe(*FilePath);
	return File->Delete();
}

bool CFile::Exists(const CString& FilePath)
{
	return std::filesystem::exists(*FilePath);
}