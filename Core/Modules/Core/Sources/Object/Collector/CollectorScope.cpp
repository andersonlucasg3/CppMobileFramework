#include "CollectorScope.h"

#include "ObjectCollector.h"

SCollectorScope::SCollectorScope()
{
	GObjectCollector.PushCollector(this);
}

SCollectorScope::~SCollectorScope()
{
	GObjectCollector.PopCollector();
}
