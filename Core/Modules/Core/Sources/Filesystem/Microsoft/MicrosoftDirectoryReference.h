#pragma once

#if PLATFORM_WINDOWS

#include "Filesystem/DirectoryReference.h"

class CWindowsDirectoryReference : public CDirectoryReference
{
	using Super = CDirectoryReference;

	CString _path;

	CORE_API void UpdateExistance() override;

public:
	CORE_API explicit CWindowsDirectoryReference(const CString& InPath);
	CORE_API ~CWindowsDirectoryReference() override = default;

	CORE_API bool Create(bool InCreateIntermediates = false) override;
	CORE_API bool Delete() override;

	CORE_API SDirectoryRef Combine(const CString& InComponent) const override;
	CORE_API SDirectoryRef Combine(const TArray<CString>& InComponents) const override;
    CORE_API SFileRef CombineFile(const CString& InFilename) const override;
    CORE_API SFileRef CombineFile(const TArray<CString>& InComponents) const override;
};

typedef CWindowsDirectoryReference CPlatformDirectoryReference;

#endif // PLATFORM_WINDOWS