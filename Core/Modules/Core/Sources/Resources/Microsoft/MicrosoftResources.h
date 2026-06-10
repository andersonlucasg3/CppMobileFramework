#pragma once

#if PLATFORM_WINDOWS

#include "Resources/Resources.h"

class CMicrosoftResources : public CResources
{
public:
	CORE_API ~CMicrosoftResources() override = default;
	
	CORE_API SFileRef GetResource(const CString& InResourceName) const override;
};

typedef CMicrosoftResources CPlatformResources;

#endif // PLATFORM_WINDOWS