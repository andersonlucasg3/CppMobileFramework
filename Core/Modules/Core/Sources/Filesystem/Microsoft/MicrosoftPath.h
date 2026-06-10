#pragma once

#if PLATFORM_WINDOWS

#include "Filesystem/Path.h"

class CWindowsPath : public CPath
{
	CORE_API static CString FixPath(const CString& Path);

public:
	CORE_API ~CWindowsPath() override = default;

	CORE_API char PathSeparator() const override;

	CORE_API CString GetFullPath(const CString& InPath) const override;
	CORE_API CString GetPathRoot(const CString& InPath) const override;

	CORE_API CString Combine(const TArray<CString>& PathComponents) const override;

	CORE_API CString LastPathComponent(const CString& Path) const override;
	CORE_API CString RemoveLastPathComponent(const CString& Path) const override;

	CORE_API const CString& CachesPath() const override;
};

typedef CWindowsPath CPlatformPath;

#endif // PLATFORM_WINDOWS