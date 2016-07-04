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
	void scoreSchedulePopulation(AvailabilityData & availabilityData, ScheduleData & scheduleData, std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> &vctScoreAndSchedulePopulation);
	void FindPossibleNamePairs(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild);
	void SortPopulationByScore();
	void SpawnNewPopulation();
	size_t FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring> &mapToLookIn);
	std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> vctScoreAndSchedulePopulation;
	std::vector<size_t> topScores;
	size_t iPopulationSize = 1000;//size of breeding population 
	double iMutationRate = 3;//mutaion rate integer percentag
	double iLikelyhoodOfCrossover = 5;//likelhood of crossover at any gene as a percentage
	double iLikelyhoodOfChopping = 5;//likelhood of chopping at any gene as a percentage
	double iGuaranteedClonePercentage=10;//top percent of population that is cloned with no mutation
	double iClonedPercentage = 10;//percent of population that is cloned by tringular distribution selection with no mutation
	double iMateGuaranteed = 10;//top percent of population that is guarenteed to breed
	double iRandomPercentage = 10;//number of new populations that are randomly generated
	double iClonedWithMutationPercentage = 10;//number of population cloned with mutation
	double iSexualWithoutCrossoverPercentage = 10;//number of population that will sexually reproduce without crossover
	double iSexualWithChoppingPercentage = 10;//number of population that sexually reproduce by chopping
	int* iScores = nullptr; //holds schedule scores
};


