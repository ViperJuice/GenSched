#pragma once
#include "SchedulingEngineFactory.h"
class EvolutionSchedulingEngineFactory:
	public SchedulingEngineFactory
{
public:
	EvolutionSchedulingEngineFactory();
	~EvolutionSchedulingEngineFactory();
	EvolutionSchedulingEngine* create_schedulingEngine();
};

