#pragma once

#if PLATFORM_WINDOWS

#include "Resources/Resources.h"

class CWindowsResources : public CResources
{
public:
	CORE_API ~CWindowsResources() override = default;
	
	CORE_API SFileRef GetResource(const CString& InResourceName) const override;
};

typedef CWindowsResources CPlatformResources;

#endif // PLATFORM_WINDOWS