#pragma once

#include "Resources/Resources.h"

class CAndroidResources : public CResources
{
public:
    CORE_API ~CAndroidResources() override = default;

    CORE_API SFileRef GetResource(const CString& InResourceName) const override;
};

typedef CAndroidResources CPlatformResources;

extern const CAndroidResources& GAndroidResources;