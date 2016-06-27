#include "pch.h"
#include "EvolutionSchedulingEngine.h"

using namespace std;
using namespace concurrency;

EvolutionSchedulingEngine::EvolutionSchedulingEngine()
{

}

void EvolutionSchedulingEngine::FillScheduleShell(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild)
{
	FindPossibleNamePairs(availabilityData, scheduleData, iNumberOfSchedulesToBuild);
	BuildInitialPopulation(availabilityData, scheduleData, iNumberOfSchedulesToBuild);
	scoreSchedulePopulation(availabilityData, scheduleData, ppPairIntSchedulePopulation);
}

void EvolutionSchedulingEngine::FindPossibleNamePairs(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild)
{
	scheduleData.pPairWstrScheduleShell = new pair<wstring, wstring>[scheduleData.iTotalNumberOfSubPeriods];
	//lookup Brave1 integer key by name
	size_t iBrave1 = FindMapKeyFromValue(L"Brave1", availabilityData.mapNumberQualType);
	//Go through each period (i) and build possible name pairs
	for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityPeriods;i++)
	{
		std::vector<std::pair<size_t, size_t>> vctPairSubPeriod1NameCombinations;
		std::vector<std::pair<size_t, size_t>> vctPairSubPeriod2NameCombinations;
		for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityDataRows;j++)//Go through each name's availability
		{
			if (availabilityData.ppBoolQualArray[j][iBrave1] == true)//Make sure Brave1 positions only filled by Brave1 qualified pilots
			{
				if (scheduleData.iNumberOfSubPeriods[i] == 1)
				{

					if (availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.ALERT_PLEASE ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.ALERT_IS_FINE ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.DONT_CARE ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.RATHER_NOT ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.PLEASE_NO)
					{
						for (size_t k = 0;k < availabilityData.iNumberOfAvailabilityDataRows;k++)//Go through each possible Brave2 name's availability
						{
							if (j != k && (availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.ALERT_PLEASE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.ALERT_IS_FINE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.DONT_CARE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.RATHER_NOT ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.PLEASE_NO)
								)
							{

								vctPairSubPeriod1NameCombinations.push_back(std::pair<size_t, size_t>(j, k));//add pair to perid vector
							}
						}
					}
				}
				else if (scheduleData.iNumberOfSubPeriods[i] == 2)
				{
					if (availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.ALERT_PLEASE ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.ALERT_IS_FINE ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.DONT_CARE ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.RATHER_NOT ||
						availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.PLEASE_NO)
					{
						for (size_t k = 0;k < availabilityData.iNumberOfAvailabilityDataRows;k++)//Go through each possible Brave2 name's availability
						{
							if (j != k && (availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.ALERT_PLEASE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.ALERT_IS_FINE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.DONT_CARE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.RATHER_NOT ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.PLEASE_NO)
								)
							{
								vctPairSubPeriod1NameCombinations.push_back(std::pair<size_t, size_t>(j, k));//add pair to perid vector
								vctPairSubPeriod2NameCombinations.push_back(std::pair<size_t, size_t>(j, k));//add pair to sub-perid vector
							}
							else if (j != k && availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.DINNER_AND_MOVIE)
							{
								vctPairSubPeriod2NameCombinations.push_back(std::pair<size_t, size_t>(j, k));
							}
						}
					}
					else if (availabilityData.ppIntAvailabilityTypeArray[j][i] == availabilityData.DINNER_AND_MOVIE)
					{
						for (size_t k = 0;k < availabilityData.iNumberOfAvailabilityDataRows;k++)//Go through each possible Brave2 name's availability
						{
							if (j != k && (availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.ALERT_PLEASE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.ALERT_IS_FINE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.DONT_CARE ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.RATHER_NOT ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.PLEASE_NO ||
								availabilityData.ppIntAvailabilityTypeArray[k][i] == availabilityData.DINNER_AND_MOVIE))
							{
								vctPairSubPeriod2NameCombinations.push_back(std::pair<size_t, size_t>(j, k));//add pair to sub-perid vector
							}
						}
					}
				}
			}
		}
		scheduleData.vctVctPairIntPossibleNameCombinations.push_back(vctPairSubPeriod1NameCombinations);//add pairs for period
		if (!vctPairSubPeriod2NameCombinations.empty()) {
			scheduleData.vctVctPairIntPossibleNameCombinations.push_back(vctPairSubPeriod2NameCombinations);//add pairs for sub-period
		}
		vctPairSubPeriod1NameCombinations.clear();
		vctPairSubPeriod2NameCombinations.clear();
	}
}

void EvolutionSchedulingEngine::BuildInitialPopulation(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild)
{
	ppPairIntSchedulePopulation = new pair<size_t, size_t>*[iPopulationSize];
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		ppPairIntSchedulePopulation[i] = new pair<size_t, size_t>[availabilityData.iNumberOfAvailabilityPeriods];
	}
	random_device rd;
	mt19937 gen(rd());
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++) 
		{
			uniform_int_distribution<> dist(0,scheduleData.vctVctPairIntPossibleNameCombinations[j].size()-1);
			ppPairIntSchedulePopulation[i][j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
		}
	}
}
void EvolutionSchedulingEngine::scoreSchedulePopulation(AvailabilityData &availabilityData, ScheduleData &scheduleData, pair<size_t, size_t>** ppPairIntSchedulePopulation)
{
	ScheduleScorer* scheduleScorer = new ScheduleScorer(availabilityData, scheduleData);
	size_t iNumberOfScoringFunctions = scheduleScorer->getFuncs().size();
	iScores = new int[iPopulationSize];
	std::future<size_t>* schedScoreFutures = new future<size_t>[iPopulationSize];
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		int iScore;
		schedScoreFutures[i] = std::async(std::launch::async, [&]()->size_t
		{
			std::future<size_t>* scoreFuncFutures = new std::future<size_t>[iNumberOfScoringFunctions];
			size_t j = 0;//keep track of iterator index
			for (std::function<size_t(std::pair<size_t, size_t>*)> &itr : scheduleScorer->getFuncs())
			{
				scoreFuncFutures[j] = std::async(std::launch::async, itr, ppPairIntSchedulePopulation[i]);
				j++;
			}
			for (j=0; j < iNumberOfScoringFunctions; j++)
			{
				iScore = scoreFuncFutures[j].get();
			}
			return iScore;
			iScores[i] += schedScoreFutures[i].get();
			wstring paraString = L"Shedule" + std::to_wstring(i) + L" scored. ";
			OutputDebugString(paraString.c_str());
		});
	}
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		iScores[i] += schedScoreFutures[i].get();
	}
}
size_t EvolutionSchedulingEngine::FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring> &mapToLookIn)
{
	const wstring nameToFind = wstrLookUp;
	auto findResult = std::find_if(std::begin(mapToLookIn), std::end(mapToLookIn), [&](const std::pair<int, wstring> &pair)
	{
		return pair.second == nameToFind;
	});

	size_t foundKey = -1; // Initialise this to an invalid value
	if (findResult != std::end(mapToLookIn))
	{
		foundKey = findResult->first;
	}
	return foundKey;
}