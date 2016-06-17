#pragma once
#include "SchedulingEngine.h"
class EvolutionSchedulingEngine:
	public SchedulingEngine
{
public:
	EvolutionSchedulingEngine();
private:
	virtual void fillScheduleShell(AvailabilityData availabilityData, ScheduleData scheduleData, size_t* iNumberOfSchedulesToBuild);
};

