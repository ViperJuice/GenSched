#pragma once
#include "SchedulingEngine.h"
class EvolutionSchedulingEngine:
	public SchedulingEngine
{
public:
	EvolutionSchedulingEngine();
private:
	virtual void FillScheduleShell(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild) override;
	void BuildInitialPopulation(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild);
	void scoreSchedulePopulation(AvailabilityData & availabilityData, ScheduleData & scheduleData, pair<size_t, size_t>** ppPairIntSchedulePopulation);
	void FindPossibleNamePairs(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild);
	size_t FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring> &mapToLookIn);
	std::pair<size_t, size_t>** ppPairIntSchedulePopulation;
	std::vector<size_t> topScores;
	size_t iMutationRate = 3;//mutaion rate integer percentag
	size_t iPopulationSize = 1000;//size of breeding population size
	size_t iClonedPercentage = 10;//top percent of population that is cloned
	size_t iMateGuarnateed = 10;//top percent of population that is guarenteed to breed
};


