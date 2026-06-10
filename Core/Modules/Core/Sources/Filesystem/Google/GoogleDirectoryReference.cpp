#include "GoogleDirectoryReference.h"

#include <filesystem>

void CGoogleDirectoryReference::UpdateExistance()
{
    _bExists = std::filesystem::exists(*_path);
}

CGoogleDirectoryReference::CGoogleDirectoryReference(const CString& InPath) : Super(InPath)
{
    //
}