#include "GoogleFileReference.h"

#include <filesystem>

void CAndroidFileReference::UpdateExistance()
{
    _bExists = std::filesystem::exists(*_path);
}

CAndroidFileReference::CAndroidFileReference(const CString& InPath) : Super(InPath)
{
    //
}