#pragma once

#include "Graphics/Rects.h"

#include <CoreGraphics/CGGeometry.h>

namespace UI::Graphics
{
    inline CGRect ToCGRect(const SRectF& Rect)
    {
        return CGRectMake(Rect.X(), Rect.Y(), Rect.Width(), Rect.Height());
    }

    inline SRectF ToRectF(const CGRect& Rect)
    {
        return { (float)Rect.origin.x, (float)Rect.origin.y, (float)Rect.size.width, (float)Rect.size.height };
    }
}