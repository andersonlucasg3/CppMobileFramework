#include "DirectoryReference.h"

#include "Defines/Preprocessors.h"

#include "Filesystem/Path.h"

#include COMPILE_PLATFORM_GROUP_HEADER(DirectoryReference.h)

#include <filesystem>

static TFunction<TSharedPtr<CPlatformDirectoryReference>(const CString&)> FactoryFunction = [](const CString& InPath)
{
    return MakeShared<CPlatformDirectoryReference>(InPath);
};

SDirectoryRef::SDirectoryRef(const char* InCStr) : SDirectoryRef(CString(InCStr))
{
    //
}

SDirectoryRef::SDirectoryRef(const CString& InPath) : Super()
{
    this->operator=(InPath);
}

SDirectoryRef::SDirectoryRef(const TArray<CString>& InComponents) : Super()
{
    this->operator=(CPlatformDirectoryReference::Get(GPath.Combine(InComponents), FactoryFunction));
}

SDirectoryRef& SDirectoryRef::operator=(const char* InCStr)
{
    this->operator=(CString(InCStr));

    return *this;
}

SDirectoryRef& SDirectoryRef::operator=(const CString& InPath)
{
    this->operator=(CPlatformDirectoryReference::Get(InPath, FactoryFunction));

    return *this;
}

SDirectoryRef& SDirectoryRef::operator=(const TArray<CString>& InComponentes)
{
    this->operator=(CPlatformDirectoryReference::Get(GPath.Combine(InComponentes), FactoryFunction));

    return *this;
}

void SDirectoryRef::operator=(const TSharedPtr<CDirectoryReference>& Other)
{
    Super::operator=(Other);
}

bool CDirectoryReference::Create(bool InCreateIntermediates)
{
    if (InCreateIntermediates ? std::filesystem::create_directories(*_path) : std::filesystem::create_directory(*_path))
    {
        UpdateExistance();

        return true;
    }

    return false;
}

bool CDirectoryReference::Delete()
{
    bool bResult = std::filesystem::remove(*_path);

    if (bResult) UpdateExistance();

    return bResult;
}

SDirectoryRef CDirectoryReference::Combine(const CString& InComponent) const
{
    return GPath.Combine({ _path, InComponent });
}

SDirectoryRef CDirectoryReference::Combine(const TArray<CString>& InComponents) const
{
    TArray<CString> NewArray(InComponents);
    NewArray.Insert(0, _path);

    return GPath.Combine(NewArray);
}

SFileRef CDirectoryReference::CombineFile(const CString& InFilename) const
{
    return GPath.Combine({ _path, InFilename });
}

SFileRef CDirectoryReference::CombineFile(const TArray<CString>& InComponents) const
{
    TArray<CString> NewArray(InComponents);
    NewArray.Insert(0, _path);

    return GPath.Combine(NewArray);
    
}