#pragma once

#include "Collector.h"

class CObject;

struct SCollectorScope : public SCollector
{
public:
	CORE_API SCollectorScope();
	CORE_API ~SCollectorScope() override;
};
