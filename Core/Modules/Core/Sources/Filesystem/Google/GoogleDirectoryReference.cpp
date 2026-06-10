#include "GoogleDirectoryReference.h"

#include <filesystem>

void CAndroidDirectoryReference::UpdateExistance()
{
    _bExists = std::filesystem::exists(*_path);
}

CAndroidDirectoryReference::CAndroidDirectoryReference(const CString& InPath) : Super(InPath)
{
    //
}