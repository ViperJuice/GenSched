#pragma once
#include "SchedulingEngine.h"
class EvolutionSchedulingEngine:
	public SchedulingEngine
{
public:
	~EvolutionSchedulingEngine();
	EvolutionSchedulingEngine();
	EvolutionSchedulingEngine(size_t iPopulationSize, size_t iNumberOfGenerationsToRun);
	void ConnectScheduleUpdateCallback(std::function<void(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>>)> schedulesUpdateCallback);
	void ConnectSchedulingProcessUpdateCallback(std::function<void(std::pair<size_t, std::pair<int, int>>)> schedulingProcessUpdateCallback);
	void ConnectScheduleScoreDataProcessUpdate(std::function<void(std::vector<ScheduleScoreData>)> scheduleScoreDataUpdateCallback);

private:
	random_device rd;
	virtual void FillScheduleShell(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild) override;
	void BuildInitialPopulation(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild);
	void scoreSchedulePopulation(AvailabilityData &availabilityData, ScheduleData & scheduleData, std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> &vctScoreAndSchedulePopulation, size_t iScoredPopulationSize, bool bFinalSchedules);
	void FindPossibleNamePairs(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild);
	void SortPopulationByScore();
	void EliminateClones(AvailabilityData &availabilityData, ScheduleData &scheduleData);
	void SpawnNewPopulation();
	void PassSchedulingProcessUpdate(AvailabilityData &availabilityData, size_t iGenerationNumber, bool bFinalPopulation);
	void Mutate(std::pair<size_t, std::vector<std::pair<size_t, size_t>>> &pairPairVctNewScoreAndOffspring);
	void CheckValidNamePair(std::pair<size_t, std::vector<std::pair<size_t, size_t>>> &pairPairVctNewScoreAndOffspring);
	std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> GenerateReadableSchedule(const AvailabilityData &availabilityData);
	wstring GenerateCSVOutputString(std::pair<int, std::vector<std::pair<wstring, wstring>>> &pairScheduleToReturn, ScheduleScoreData &scheduleScoreData, const AvailabilityData &availabilityData, size_t iScheduleNumber);
	std::function<void(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>>)> schedulesUpdateCallback;
	std::function<void(std::pair<size_t, std::pair<int, int>>)> schedulingProcessUpdateCallback;
	std::function<void(std::vector<ScheduleScoreData>)> scheduleScoreDataUpdateCallback;
	ScheduleScorer* scheduleScorer = nullptr;
	std::vector<ScheduleScoreData> vctScheduleScoreData;
	size_t FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring> &mapToLookIn);
	std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> vctScoreAndSchedulePopulation;
	std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> vctSchedulesToReturn;
	std::vector<size_t> topScores;
	size_t iPopulationSize = 1000;//size of breeding population 
	size_t iNumberOfGenerationsToRun = 1000;//number of generations to run
	size_t iNumberOfSchedulesToBuild = 10;//number of schedule options to build
	double iMutationRate = 3;//mutaion rate integer percentage
	double iCloneEliminationMutationRate = 10;//muation rate used to elminate clone if a clone is found
	double iLikelyhoodOfCrossover = 5;//likelhood of crossover at any gene as a percentage
	double iLikelyhoodOfChopping = 5;//likelhood of chopping at any gene as a percentage
	size_t iGuaranteedClonePercentage=10;//top percent of population that is cloned with no mutation
	size_t iMateGuaranteed = 10;//top percent of population that is guarenteed to breed
	size_t iClonedPercentage = 10;//percent of population that is cloned by tringular distribution selection with no mutation
	size_t iRandomPercentage = 10;//number of new populations that are randomly generated
	size_t iClonedWithMutationPercentage = 10;//number of population cloned with mutation
	size_t iSexualWithoutCrossoverPercentage = 10;//number of population that will sexually reproduce without crossover
	size_t iSexualWithChoppingPercentage = 10;//number of population that sexually reproduce by chopping
	int* iScores = nullptr; //holds schedule scores
	std::pair<int, std::vector<std::pair<size_t, size_t>>>*  arrayNewScheduleAndScorePopulation = nullptr;

	std::mutex mx;
	std::mutex mx1;
	std::vector<std::vector<std::pair<size_t, size_t>>>  vctVctPairBreeders;

};


