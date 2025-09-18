#pragma once

#include "Defines/Types.h"

#include "Templates/Rect.h"

#include "Points.h"
#include "Sizes.h"


namespace UI::Graphics
{
	typedef TRect<Int32, SPoint, SSize> SRect;
	typedef TRect<Float, SPointF, SSizeF> SRectF;
}
