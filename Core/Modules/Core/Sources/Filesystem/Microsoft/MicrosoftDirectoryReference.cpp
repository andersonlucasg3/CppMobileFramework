#include "MicrosoftDirectoryReference.h"

#if PLATFORM_WINDOWS

#include "Filesystem/Path.h"

#include <Windows.h>
#include <filesystem>

void CMicrosoftDirectoryReference::UpdateExistance()
{
	DWORD Attrs = GetFileAttributes(*_path);
	
	_bExists = Attrs != INVALID_FILE_ATTRIBUTES && (Attrs & FILE_ATTRIBUTE_DIRECTORY);
}

CMicrosoftDirectoryReference::CMicrosoftDirectoryReference(const CString& InPath) : Super(InPath)
,	_path(InPath)
{	
	UpdateExistance();
}

bool CMicrosoftDirectoryReference::Create(bool InCreateIntermediates)
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

bool CMicrosoftDirectoryReference::Delete()
{
    if (!RemoveDirectory(*_path))
    {
        return false;
    }

    UpdateExistance();
    return true;
}

SDirectoryRef CMicrosoftDirectoryReference::Combine(const CString& InComponent) const
{
	return GPath.Combine({ _path, InComponent });
}

SDirectoryRef CMicrosoftDirectoryReference::Combine(const TArray<CString>& InComponents) const
{
	TArray<CString> NewArray(InComponents);
	NewArray.Insert(0, _path);
	
	return GPath.Combine(NewArray);
}

SFileRef CMicrosoftDirectoryReference::CombineFile(const CString& InFilename) const
{
	return GPath.Combine({ _path, InFilename });
}

SFileRef CMicrosoftDirectoryReference::CombineFile(const TArray<CString>& InComponents) const
{
	TArray<CString> NewArray(InComponents);
	NewArray.Insert(0, _path);
	
	return GPath.Combine(NewArray);
}

#endif // PLATFORM_WINDOWS