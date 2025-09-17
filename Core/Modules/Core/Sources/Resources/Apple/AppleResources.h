#pragma once

#include "Resources/Resources.h"

class CAppleResources : public CResources
{
public:
    CORE_API ~CAppleResources() override = default;

    CORE_API SFileRef GetResource(const CString& InResourceName) const override;
};

typedef CAppleResources CPlatformResources;