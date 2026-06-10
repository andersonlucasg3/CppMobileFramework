#include "WindowsDirectoryReference.h"

#if PLATFORM_WINDOWS

#include "Filesystem/Path.h"

#include <Windows.h>
#include <filesystem>

void CWindowsDirectoryReference::UpdateExistance()
{
	DWORD Attrs = GetFileAttributes(*_path);
	
	_bExists = Attrs != INVALID_FILE_ATTRIBUTES && (Attrs & FILE_ATTRIBUTE_DIRECTORY);
}

CWindowsDirectoryReference::CWindowsDirectoryReference(const CString& InPath) : Super(InPath)
,	_path(InPath)
{	
	UpdateExistance();
}

bool CWindowsDirectoryReference::Create(bool InCreateIntermediates)
{
    if (InCreateIntermediates)
    {
        // SHCreateDirectoryEx or recursive CreateDirectory
        std::filesystem::create_directories(*_path);
    }
    else if (!CreateDirectory(*_path, NULL))
    {
        return false;
    }

    UpdateExistance();
    return _bExists;
}

bool CWindowsDirectoryReference::Delete()
{
    if (!RemoveDirectory(*_path))
    {
        return false;
    }

    UpdateExistance();
    return true;
}

SDirectoryRef CWindowsDirectoryReference::Combine(const CString& InComponent) const
{
	return GPath.Combine({ _path, InComponent });
}

SDirectoryRef CWindowsDirectoryReference::Combine(const TArray<CString>& InComponents) const
{
	TArray<CString> NewArray(InComponents);
	NewArray.Insert(0, _path);
	
	return GPath.Combine(NewArray);
}

SFileRef CWindowsDirectoryReference::CombineFile(const CString& InFilename) const
{
	return GPath.Combine({ _path, InFilename });
}

SFileRef CWindowsDirectoryReference::CombineFile(const TArray<CString>& InComponents) const
{
	TArray<CString> NewArray(InComponents);
	NewArray.Insert(0, _path);
	
	return GPath.Combine(NewArray);
}

#endif // PLATFORM_WINDOWS