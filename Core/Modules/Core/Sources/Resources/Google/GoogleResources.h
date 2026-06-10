#pragma once

#include "Resources/Resources.h"

class CGoogleResources : public CResources
{
public:
    CORE_API ~CGoogleResources() override = default;

    CORE_API SFileRef GetResource(const CString& InResourceName) const override;
};

typedef CGoogleResources CPlatformResources;

extern const CGoogleResources& GAndroidResources;