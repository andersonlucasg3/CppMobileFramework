#pragma once

#include "Collector.h"

class CObject;

struct SCollectorScope : public SCollector
{
public:
	SCollectorScope();
	~SCollectorScope() override;
};
