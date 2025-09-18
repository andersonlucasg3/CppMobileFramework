#include "FileReference.h"

#include "Templates/Functions.h"

#include "Preprocessors.h"

#include <filesystem>

#include COMPILE_PLATFORM_GROUP_HEADER(FileReference.h)

static TFunction<TSharedPtr<CPlatformFileReference>(const CString&)> FactoryFunction = [](const CString& InPath)
{
    return MakeShared<CPlatformFileReference>(InPath);
};

SFileRef::SFileRef(const char* InCStr) : SFileRef(CString(InCStr)) 
{
    //
}

SFileRef::SFileRef(const CString& InPath) : Super()
{
    this->operator=(CFileReference::Get(InPath, FactoryFunction));
}

SFileRef::SFileRef(const TArray<CString>& InComponents) : Super()
{
    this->operator=(CFileReference::Get(GPath.Combine(InComponents), FactoryFunction));
}

SFileRef& SFileRef::operator=(const char* InCStr)
{
    return this->operator=(CString(InCStr));
}

SFileRef& SFileRef::operator=(const CString& InPath)
{
    this->operator=(CFileReference::Get(InPath, FactoryFunction));

    return *this;
}

SFileRef& SFileRef::operator=(const TArray<CString>& InComponents)
{
    this->operator=(CFileReference::Get(GPath.Combine(InComponents), FactoryFunction));

    return *this;
}

void SFileRef::operator=(const TSharedPtr<CFileReference>& Other)
{
    Super::operator=(Other);
}

CFileReference::CFileReference(const CString& InPath) : Super(InPath) 
{
    //
}

bool CFileReference::Delete()
{
    bool bResult = std::filesystem::remove(*_path);

    if (bResult) UpdateExistance();

    return bResult;
}