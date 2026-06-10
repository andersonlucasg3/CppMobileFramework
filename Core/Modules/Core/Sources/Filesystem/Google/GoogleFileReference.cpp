#include "GoogleFileReference.h"

#include <filesystem>

void CGoogleFileReference::UpdateExistance()
{
    _bExists = std::filesystem::exists(*_path);
}

CGoogleFileReference::CGoogleFileReference(const CString& InPath) : Super(InPath)
{
    //
}