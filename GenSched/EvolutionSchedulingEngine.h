#pragma once
#include "SchedulingEngine.h"
class EvolutionSchedulingEngine:
	public SchedulingEngine
{
public:
	EvolutionSchedulingEngine();
private:
	virtual size_t* buildScheduleShell(AvailabilityData);
	virtual void fillScheduleShell(AvailabilityData, size_t*);
};

