#include "pch.h"
#include "EvolutionSchedulingEngine.h"

using namespace std;
using namespace concurrency;

EvolutionSchedulingEngine::~EvolutionSchedulingEngine()
{

}

EvolutionSchedulingEngine::EvolutionSchedulingEngine()
{

}

EvolutionSchedulingEngine::EvolutionSchedulingEngine(size_t iPopulationSize, size_t iNumberOfGenerationsToRun): iPopulationSize(iPopulationSize), iNumberOfGenerationsToRun(iNumberOfGenerationsToRun)
{

}

void EvolutionSchedulingEngine::ConnectScheduleUpdateCallback(std::function<void(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>>)> schedulesUpdateCallback)
{
	EvolutionSchedulingEngine::schedulesUpdateCallback = schedulesUpdateCallback;
}

void EvolutionSchedulingEngine::ConnectSchedulingProcessUpdateCallback(std::function<void(std::pair<size_t, std::pair<int, int>>)> schedulingProcessUpdateCallback)
{
	EvolutionSchedulingEngine::schedulingProcessUpdateCallback = schedulingProcessUpdateCallback;
}

void EvolutionSchedulingEngine::ConnectScheduleScoreDataProcessUpdate(std::function<void(ScheduleScoreData)> scheduleScoreDataUpdateCallback)
{
	EvolutionSchedulingEngine::scheduleScoreDataUpdateCallback = scheduleScoreDataUpdateCallback;
}


void EvolutionSchedulingEngine::FillScheduleShell(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild)
{
	size_t iGenerationNumber = 0;
	FindPossibleNamePairs(availabilityData, scheduleData, iNumberOfSchedulesToBuild);
	BuildInitialPopulation(availabilityData, scheduleData, iNumberOfSchedulesToBuild);
	scoreSchedulePopulation(availabilityData, scheduleData, vctScoreAndSchedulePopulation, iPopulationSize, false);
	SortPopulationByScore();
	PassSchedulingProcessUpdate(availabilityData, 0, false);//Pass info back to main process
	size_t iGenNumber = 1;
	for (iGenNumber = 1;iGenNumber < iNumberOfGenerationsToRun && !bStopTheEngine;iGenNumber++)
	{
		SpawnNewPopulation();
		scoreSchedulePopulation(availabilityData, scheduleData, vctScoreAndSchedulePopulation, iPopulationSize, false);
		SortPopulationByScore();
		PassSchedulingProcessUpdate(availabilityData, iGenNumber, false);//Pass info back to main process
	}
	PassSchedulingProcessUpdate(availabilityData, iGenNumber, true);//Pass info back to main process
	//TODO Move this to FileFunctions
	std:vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> vctSchedulesToReturn;
	vctSchedulesToReturn.reserve(iNumberOfSchedulesToBuild);
	std::pair<wstring, wstring> pairGeneToAdd;
	std::pair<int, std::vector<pair<wstring, wstring>>> pairScoreAndGeneToAdd;
	pairScoreAndGeneToAdd.second.reserve(scheduleData.iTotalNumberOfSubPeriods);
	int iNumberOfSchedulesToBuildUnsigned = iNumberOfSchedulesToBuild;
	for (size_t j = 0;j < iNumberOfSchedulesToBuild;j++)
	{
		pairScoreAndGeneToAdd.first = vctScoreAndSchedulePopulation[j].first;
		pairScoreAndGeneToAdd.second.clear();
		for (size_t k = 0; k < scheduleData.iTotalNumberOfSubPeriods;k++)
		{
			pairGeneToAdd.first = availabilityData.mapNumberName.find(vctScoreAndSchedulePopulation[j].second[k].first)->second;
			pairGeneToAdd.second = availabilityData.mapNumberName.find(vctScoreAndSchedulePopulation[j].second[k].second)->second;
			pairScoreAndGeneToAdd.second.push_back(pairGeneToAdd);
		}
		vctSchedulesToReturn.push_back(pairScoreAndGeneToAdd);
	}

	for (size_t j=0;j < vctSchedulesToReturn.size();j++)
	{
		auto platformPath = Windows::Storage::ApplicationData::Current->RoamingFolder->Path;
		std::wstring wstrplatformPath = platformPath->Data();
		std::wstring wstrPlatformPathAndFilename = wstrplatformPath + L"\\" + availabilityData.month + L"_" + std::to_wstring(availabilityData.year) + L"_" + std::to_wstring(j) + L"_" + L"AlertScheduleOut.csv";
		std::string convertedPlatformPathandFilename(wstrPlatformPathAndFilename.begin(), wstrPlatformPathAndFilename.end());
		std::wofstream outFile(convertedPlatformPathandFilename, std::ofstream::out | std::ofstream::trunc);
		std::vector<std::pair<wstring, wstring>>::iterator pairItr;
		std::wstring strScheduleOutputString = L"";
		size_t iDinnerAndMovieCounter=0;
		strScheduleOutputString += L"Juice Torrence's Genetic Alert Scheduling Builder Output File Version 0.0"s + L"\n"
			+ availabilityData.month +L"," + std::to_wstring(availabilityData.year) + L"\n";
		for (size_t j = 0;j < scheduleData.iNumberOfAvailabilityPeriods;j++) 
		{
			strScheduleOutputString += std::to_wstring(j + 1);
			if (j != scheduleData.iNumberOfAvailabilityPeriods-1 || scheduleData.iNumberOfSubPeriods[j] == 2)
			{
				strScheduleOutputString += L",";
			}
			if (scheduleData.iNumberOfSubPeriods[j] == 2)
			{
				iDinnerAndMovieCounter++;
				strScheduleOutputString += L"D&M";
				if (j != scheduleData.iNumberOfAvailabilityPeriods - 1)
				{
					strScheduleOutputString += L",";
				}
			}
		}
		strScheduleOutputString += L"\n";
		for (pairItr = vctSchedulesToReturn[j].second.begin(); pairItr!=vctSchedulesToReturn[j].second.end(); pairItr++)
		{
			strScheduleOutputString += pairItr->first;
			if ((pairItr + 1) != vctSchedulesToReturn[j].second.end())
			{
				strScheduleOutputString += L",";
			}
		}
		strScheduleOutputString += L"\n";
		for (pairItr = vctSchedulesToReturn[j].second.begin(); pairItr!=vctSchedulesToReturn[j].second.end(); pairItr++)
		{
			strScheduleOutputString += pairItr->second;
			if ((pairItr + 1) != vctSchedulesToReturn[j].second.end())
			{
				strScheduleOutputString += L",";
			}
		}
		strScheduleOutputString += L"\n";
		std::vector<std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>>>::iterator itrDateAndName;
		for (itrDateAndName = scheduleScoreData.vctDateAndNameSpecific.begin();itrDateAndName != scheduleScoreData.vctDateAndNameSpecific.end();itrDateAndName++)
		{
			for (int i=0;i<scheduleData.iTotalNumberOfSubPeriods;i++)
			{
				if (i != itrDateAndName->first.first)
				{
					strScheduleOutputString += L",";
				}
				else
				{
					strScheduleOutputString += std::to_wstring(itrDateAndName->second.second) + L",";
				}
			}
			strScheduleOutputString += availabilityData.mapNumberName.find(itrDateAndName->first.second)->second 
				+ L"," + itrDateAndName->second.first + L"\n";
		}
		std::vector<std::pair<std::size_t, std::pair<std::wstring, int>>>::iterator itrDate;
		for (itrDate = scheduleScoreData.vctDateSpecific.begin();itrDate != scheduleScoreData.vctDateSpecific.end();itrDate++)
		{
			for (int i = 0;i<scheduleData.iTotalNumberOfSubPeriods;i++)
			{
				if (i != itrDate->first)
				{
					strScheduleOutputString += L",";
				}
				else
				{
					strScheduleOutputString += std::to_wstring(itrDate->second.second) + L",";
				}
			}
			strScheduleOutputString += 
				+ L",," + itrDate->second.first + L"\n";
		}
		std::vector<std::pair<std::pair<size_t, std::wstring>, int>>::iterator itrName;
		for (itrName = scheduleScoreData.vctNameSpecific.begin();itrName != scheduleScoreData.vctNameSpecific.end();itrName++)
		{
			strScheduleOutputString += availabilityData.mapNumberName.find(itrName->first.first)->second + L"," + itrDate->second.first 
				+ L"," + std::to_wstring(itrDate->second.second) + L"\n";
		}
		std::vector<std::pair<std::wstring, int>>::iterator itrCalendar;
		for (itrCalendar = scheduleScoreData.vctScheduleSpecific.begin();itrCalendar != scheduleScoreData.vctScheduleSpecific.end();itrCalendar++)
		{
			strScheduleOutputString += itrCalendar->first+ L"," + std::to_wstring(itrCalendar->second) + L"\n";
		}
		outFile << strScheduleOutputString;
		outFile.flush();
		outFile.close();
	}
}

void EvolutionSchedulingEngine::FindPossibleNamePairs(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild)
{
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
void EvolutionSchedulingEngine::BuildInitialPopulation
(
	AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild
)
{
	mt19937 gen(rd());
	vctScoreAndSchedulePopulation.reserve(iPopulationSize);
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		std::pair<int, std::vector<std::pair<size_t, size_t>>> pair;
		pair.first = 0;//initialize score to zero
		pair.second.reserve(scheduleData.iTotalNumberOfSubPeriods);
		vctScoreAndSchedulePopulation.push_back(pair);
	}
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++) 
		{
			uniform_int_distribution<> dist(0,scheduleData.vctVctPairIntPossibleNameCombinations[j].size()-1);
			vctScoreAndSchedulePopulation[i].second.push_back(scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)]);
		}
	}
}
void EvolutionSchedulingEngine::scoreSchedulePopulation(AvailabilityData &availabilityData, ScheduleData &scheduleData, std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> &vctScoreAndSchedulePopulation, size_t iScoredPopulationSize, bool bFinalSchedules)
{
	ScheduleScorer scheduleScorer(availabilityData, scheduleData);
	scheduleScorer.SetFinalScheduleFlag(bFinalSchedules);
	size_t iNumberOfScoringFunctions = scheduleScorer.getFuncs().size();
	std::unique_ptr<std::future<size_t>[]> schedScoreFutures(new future<size_t>[iScoredPopulationSize]);
	for (size_t i = 0;i < iScoredPopulationSize;i++)
	{
		std::vector<std::pair<size_t, size_t>> vectScheduleToScore = vctScoreAndSchedulePopulation[i].second;;
		try
		{
			schedScoreFutures[i] = std::async(std::launch::async, [&, this](size_t iPopulationIndex, std::vector<std::pair<size_t, size_t>> vectScheduleToScore)->size_t
			{
				std::unique_ptr<std::future<size_t>[]> scoreFuncFutures(new std::future<size_t>[iNumberOfScoringFunctions]);
				size_t j = 0;//keep track of iterator index
				for (std::function<size_t(std::vector<std::pair<size_t, size_t>> scheduleToScore)> &itr : scheduleScorer.getFuncs())
				{
					try
					{
						scoreFuncFutures[j] = std::async(std::launch::deferred, itr, vectScheduleToScore);
					}
					catch (std::future_error const & e)
					{
						Platform::String^ msg("Future Error = ");

						OutputDebugString(msg->Data());
					}
					catch (std::exception const & e)
					{
						Platform::String^ msg("Standard Error = ");
						OutputDebugString(msg->Data());
					}
					catch (...)
					{
						Platform::String^ msg = "Unkown Exception";
						OutputDebugString(msg->Data());
					}
					j++;
				}
				int iScore = 0;
				for (j = 0; j < iNumberOfScoringFunctions; j++)
				{
					iScore += scoreFuncFutures[j].get();
				}
				wstring paraString = std::to_wstring(iPopulationIndex) + L" " +  std::to_wstring(iScore) + L", ";
				OutputDebugString(paraString.c_str());
				return iScore;
			}, i, vectScheduleToScore);
		}
		catch (std::future_error const & e)
		{
			Platform::String^ msg("Future Error = ");

			OutputDebugString(msg->Data());
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	}
	for (size_t i = 0;i < iScoredPopulationSize;i++)
	{
		try {
			vctScoreAndSchedulePopulation[i].first += schedScoreFutures[i].get();
		}
		catch (Platform::Exception^ e)
		{
			Platform::String^ msg("Platform Error = ");
			OutputDebugString(msg->Data());
			
		}
		catch (std::exception e)
		{
			std::cout << e.what();
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
			::OutputDebugStringA(e.what());

		}
		catch (...)
		{
			Platform::String^ msg("Unkown Error = ");
			OutputDebugString(msg->Data());
		}
	}
}
void EvolutionSchedulingEngine::SortPopulationByScore()
{
	std::sort
	(
		vctScoreAndSchedulePopulation.begin(),
		vctScoreAndSchedulePopulation.end(),
		[&]
	(
		const std::pair<int, std::vector<std::pair<size_t, size_t>>>& firstElem,
		const std::pair<int, std::vector<std::pair<size_t, size_t>>>& secondElem) ->bool
	{
		return firstElem.first > secondElem.first;
	});
}
void EvolutionSchedulingEngine::SpawnNewPopulation()
{
	std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> vctNewScheduleAndScorePopulation;
	vctNewScheduleAndScorePopulation.reserve(iPopulationSize);
	size_t iNewPopulation = 0;//tracks how many new offspring have been created
	//clone top percentage of population into next generation
	size_t newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i<newPopulationSize +(iPopulationSize*iGuaranteedClonePercentage/100);i++)
	{
		vctNewScheduleAndScorePopulation.push_back(vctScoreAndSchedulePopulation[i]);//copy old population to new
		vctNewScheduleAndScorePopulation[i].first = 0;//initialize score
		iNewPopulation++;//increment the population size
	}
	//generate random members of new population
	mt19937 gen(rd());
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i< newPopulationSize+(iPopulationSize*iRandomPercentage/100);i++)
	{
		std::pair<size_t,std::vector<std::pair<size_t, size_t>>> vctTmp;//vector to hold new schedule temporarily
		vctTmp.first = 0;//initialize score
		vctTmp.second.reserve(scheduleData.iTotalNumberOfSubPeriods);//reserve space for schedule in temp vector
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
			vctTmp.second.push_back(scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)]);//insert random genes into new schedules
		}
		vctNewScheduleAndScorePopulation.push_back(vctTmp);
		iNewPopulation++;//increment new population size
	}
	//generate new population by cloning
	uniform_real_distribution<> dist1(0, 1);
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i<newPopulationSize + (iPopulationSize*iClonedPercentage / 100);i++)
	{
		//converts uniform distribution to triangular distribution a=0 c=0 b=iPopulationSize
		size_t  iIndexToClone =  (iPopulationSize-1)*(1 - (sqrt(1 - dist1(gen))));
		vctNewScheduleAndScorePopulation.push_back(vctScoreAndSchedulePopulation[iIndexToClone]);//copy old to new population
		vctNewScheduleAndScorePopulation[i].first = 0;//initialize score
		iNewPopulation++;//increment new population size
	}
	//generate new population by cloning with mutation
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i< newPopulationSize + (iPopulationSize * iClonedWithMutationPercentage/100);i++)
	{
		//converts uniform distribution to triangular distribution a=0 c=0 b=iPopulationSize
		size_t  iIndexToClone = (iPopulationSize - 1)*(1 - (sqrt(1 - dist1(gen))));
		std::pair<size_t, std::vector<std::pair<size_t, size_t>>> vctTmp;
		vctTmp = vctScoreAndSchedulePopulation[iIndexToClone];
		Mutate(vctTmp);//insert mutations
		vctTmp.first = 0;//initialize score
		vctNewScheduleAndScorePopulation.push_back(vctTmp);
		iNewPopulation++;//increment new population size
	}
	//Build Breeding Population
	std::vector<std::vector<std::pair<size_t, size_t>>>  vctVctPairBreeders(2*(iPopulationSize - iNewPopulation));
	size_t iBreedingPopulationSize=0;//tracks breeding population size
	for (size_t i = 0;i < (iMateGuaranteed / 100)*iPopulationSize;i++)
	{
		vctVctPairBreeders[iBreedingPopulationSize]=vctScoreAndSchedulePopulation[i].second;//add guarenteed breeders
		iBreedingPopulationSize++;
	}
	size_t iBreedersToAdd = 2 * (iPopulationSize - iNewPopulation) - iBreedingPopulationSize;
	for (size_t i = 0; i < iBreedersToAdd;i++)
	{
		//converts uniform distribution to triangular distribution a=0 c=0 b=iPopulationSize
		size_t  iIndexToBreed = (vctScoreAndSchedulePopulation.size() - 1 )*(1 - (sqrt(1 - dist1(gen))));

		vctVctPairBreeders[iBreedingPopulationSize] = vctScoreAndSchedulePopulation[iIndexToBreed].second;
		iBreedingPopulationSize++;
	}
	//Sexual Reproduction Without Crossover
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i < newPopulationSize  + (iPopulationSize*iSexualWithoutCrossoverPercentage / 100);i += 2)
	{
		uniform_int_distribution<> dist(0, vctVctPairBreeders.size()-1);
		size_t iIndex1 = dist(gen);
		size_t iIndex2 = dist(gen);
		while (iIndex1 == iIndex2)//Ensure different indexes
		{
			iIndex2 = dist(gen);
		}
		std::pair<size_t ,std::vector<std::pair<size_t, size_t>>> pairPairVctNewScoreAndOffspring;
		std::vector<std::pair<size_t, size_t>> vctPairOffspring1;
		vctPairOffspring1.reserve(scheduleData.iTotalNumberOfSubPeriods);
		std::pair<size_t, size_t> pairGene1;
		std::vector<std::pair<size_t, size_t>> vctPairOffspring2;
		vctPairOffspring2.reserve(scheduleData.iTotalNumberOfSubPeriods);
		std::pair<size_t, size_t> pairGene2;
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			pairGene1.first = vctVctPairBreeders[iIndex1][j].first;
			pairGene1.second = vctVctPairBreeders[iIndex2][j].second;
			vctPairOffspring1.push_back(pairGene1);
			pairGene2.first = vctVctPairBreeders[iIndex1][j].second;
			pairGene2.second = vctVctPairBreeders[iIndex2][j].first;
			vctPairOffspring2.push_back(pairGene2);
		}
		pairPairVctNewScoreAndOffspring.first=0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring1;
		Mutate(pairPairVctNewScoreAndOffspring);//insert mutations
		CheckValidNamePair(pairPairVctNewScoreAndOffspring);//make sure crossover and mutation end up with valid name pairs
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		
		pairPairVctNewScoreAndOffspring.first = 0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring2;
		Mutate(pairPairVctNewScoreAndOffspring);//insert mutations
		CheckValidNamePair(pairPairVctNewScoreAndOffspring);//make sure crossover and mutation end up with valid name pairs
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
	

		//remove used breeders
		if (iIndex1>iIndex2)
		{
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex1);
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex2);
		}
		else
		{
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex2);
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex1);
		}
	}
	//Sexual Reproduction By Chopping
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i < newPopulationSize + (iPopulationSize*iSexualWithChoppingPercentage / 100);i += 2)
	{
		uniform_int_distribution<> dist(0, vctVctPairBreeders.size() - 1);
		size_t iIndex1 = dist(gen);
		size_t iIndex2 = dist(gen);
		while (iIndex1 == iIndex2)//Ensure different indexes
		{
			iIndex2 = dist(gen);
		}
		std::pair<size_t, std::vector<std::pair<size_t, size_t>>> pairPairVctNewScoreAndOffspring;
		std::vector<std::pair<size_t, size_t>> vctPairOffspring1;
		vctPairOffspring1.reserve(scheduleData.iTotalNumberOfSubPeriods);
		std::pair<size_t, size_t> pairGene1;
		std::vector<std::pair<size_t, size_t>> vctPairOffspring2;
		vctPairOffspring2.reserve(scheduleData.iTotalNumberOfSubPeriods);
		std::pair<size_t, size_t> pairGene2;
		uniform_real_distribution<> dist1(0,1);
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen)>iLikelyhoodOfChopping/100) 
			{
				pairGene1.first = vctVctPairBreeders[iIndex1][j].first;
				pairGene1.second = vctVctPairBreeders[iIndex1][j].second;
				vctPairOffspring1.push_back(pairGene1);
				pairGene2.first = vctVctPairBreeders[iIndex2][j].first;
				pairGene2.second = vctVctPairBreeders[iIndex2][j].second;
				vctPairOffspring2.push_back(pairGene2);
			}
			else
			{
				pairGene1.first = vctVctPairBreeders[iIndex2][j].first;
				pairGene1.second = vctVctPairBreeders[iIndex2][j].second;
				vctPairOffspring1.push_back(pairGene1);
				pairGene2.first = vctVctPairBreeders[iIndex1][j].first;
				pairGene2.second = vctVctPairBreeders[iIndex1][j].second;
				vctPairOffspring2.push_back(pairGene2);
			}
		}
		pairPairVctNewScoreAndOffspring.first = 0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring1;
		Mutate(pairPairVctNewScoreAndOffspring);//insert mutation
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		
		pairPairVctNewScoreAndOffspring.first = 0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring2;
		Mutate(pairPairVctNewScoreAndOffspring);//insert mutation
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		
		//remove used breeders
		if (iIndex1>iIndex2)
		{
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex1);
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex2);
		}
		else
		{
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex2);
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex1);
		}
	}
	//Sexual Reproduction With Crossover
	size_t iOffspringLeftToCreate = iPopulationSize - iNewPopulation;
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize; i < newPopulationSize + iOffspringLeftToCreate;i += 2)
	{
		uniform_int_distribution<> dist(0, vctVctPairBreeders.size() - 1);
		size_t iIndex1 = dist(gen);
		size_t iIndex2 = dist(gen);
		while (iIndex1 == iIndex2)//Ensure different indexes
		{
			iIndex2 = dist(gen);
		}
		std::pair<size_t, std::vector<std::pair<size_t, size_t>>> pairPairVctNewScoreAndOffspring;
		std::vector<std::pair<size_t, size_t>> vctPairOffspring1;
		vctPairOffspring1.reserve(scheduleData.iTotalNumberOfSubPeriods);
		std::pair<size_t, size_t> pairGene1;
		std::vector<std::pair<size_t, size_t>> vctPairOffspring2;
		vctPairOffspring2.reserve(scheduleData.iTotalNumberOfSubPeriods);
		std::pair<size_t, size_t> pairGene2;
		uniform_real_distribution<> dist1(0, 1);
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			//Swap Indicies if crossover
			size_t iTmp = 0;
			if (dist1(gen) < iLikelyhoodOfCrossover / 100)
			{
				iTmp = iIndex1;
				iIndex1 = iIndex2;
				iIndex2 = iTmp;
			}
			pairGene1.first = vctVctPairBreeders[iIndex1][j].first;
			pairGene1.second = vctVctPairBreeders[iIndex2][j].second;
			vctPairOffspring1.push_back(pairGene1);
			pairGene2.first = vctVctPairBreeders[iIndex2][j].first;
			pairGene2.second = vctVctPairBreeders[iIndex1][j].second;
			vctPairOffspring2.push_back(pairGene2);
		}
		//remove used breeders
		if (iIndex1>iIndex2)
		{
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex1);
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex2);
		}
		else
		{
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex2);
			vctVctPairBreeders.erase(vctVctPairBreeders.begin() + iIndex1);
		}
		//Offspring1
		pairPairVctNewScoreAndOffspring.first = 0;//initialize score for new schedule
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring1;//add genome to new schedule
		Mutate(pairPairVctNewScoreAndOffspring);//insert mutations
		CheckValidNamePair(pairPairVctNewScoreAndOffspring);//check to ensure no invalid names due to crossover or mutation
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		//Offspring1
		pairPairVctNewScoreAndOffspring.first = 0;//initialize score for new schedule
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring2;
		Mutate(pairPairVctNewScoreAndOffspring);//insert mutations
		CheckValidNamePair(pairPairVctNewScoreAndOffspring);
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
	}
	vctScoreAndSchedulePopulation = vctNewScheduleAndScorePopulation;
}
void EvolutionSchedulingEngine::PassSchedulingProcessUpdate(AvailabilityData &availabilityData, size_t iGenerationNumber, bool bFinalPopulation)
{
	//Update Scheduling process metadata via callback to main process scheduling data object
	std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> vctSchedulesToReturn;//For Display
	std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> vctIntSchedulesToReturn;//For Schedule scoreData
	vctSchedulesToReturn.reserve(iNumberOfSchedulesToBuild);
	std::pair<wstring, wstring> pairGeneToAdd;
	std::pair<int, std::vector<pair<wstring, wstring>>> pairScoreAndGeneToAdd;
	pairScoreAndGeneToAdd.second.reserve(scheduleData.iTotalNumberOfSubPeriods);
	int iNumberOfSchedulesToBuildUnsigned = iNumberOfSchedulesToBuild;
	for (size_t j = 0;j < iNumberOfSchedulesToBuild;j++)
	{
		vctIntSchedulesToReturn.push_back(vctScoreAndSchedulePopulation[j]);
		pairScoreAndGeneToAdd.first = vctScoreAndSchedulePopulation[j].first;
		for (size_t k = 0; k < scheduleData.iTotalNumberOfSubPeriods;k++)
		{
			pairGeneToAdd.first = availabilityData.mapNumberName.find(vctScoreAndSchedulePopulation[j].second[k].first)->second;
			pairGeneToAdd.second = availabilityData.mapNumberName.find(vctScoreAndSchedulePopulation[j].second[k].second)->second;
			pairScoreAndGeneToAdd.second.push_back(pairGeneToAdd);
		}
		vctSchedulesToReturn.push_back(pairScoreAndGeneToAdd);
	}
	scoreSchedulePopulation(availabilityData, scheduleData, vctIntSchedulesToReturn, iNumberOfSchedulesToBuild, bFinalPopulation);
	schedulesUpdateCallback(vctSchedulesToReturn);
	int iAverageScore = 0;
	std::pair <size_t, std::pair<int, int>> pairToReturn;
	pairToReturn.first = iGenerationNumber;
	pairToReturn.second.first = vctScoreAndSchedulePopulation[0].first;
	for (size_t j = 0;j < iNumberOfSchedulesToBuild;j++)
	{
		iAverageScore += vctScoreAndSchedulePopulation[j].first;
	}
	iAverageScore = iAverageScore / iNumberOfSchedulesToBuildUnsigned;
	pairToReturn.second.second = iAverageScore;
	schedulingProcessUpdateCallback(pairToReturn);
	scheduleScoreDataUpdateCallback(scheduleScoreData);
	//End callbacks and updates to main process
}

void EvolutionSchedulingEngine::Mutate(std::pair<size_t, std::vector<std::pair<size_t, size_t>>> &pairPairVctNewScoreAndOffspring)
{
	mt19937 gen(rd());
	uniform_int_distribution<> dist1(0, 1);
	for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
	{
		if (dist1(gen) < iMutationRate / 100)
		{
			uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
			pairPairVctNewScoreAndOffspring.second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
		}
	}
}

void EvolutionSchedulingEngine::CheckValidNamePair(std::pair<size_t, std::vector<std::pair<size_t, size_t>>>& pairPairVctNewScoreAndOffspring)
{
	mt19937 gen(rd());
	for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
	{
		bool bValidPair = false;
		for (size_t k = 0; k < scheduleData.vctVctPairIntPossibleNameCombinations[j].size();k++)
		{
			if (pairPairVctNewScoreAndOffspring.second[j] == scheduleData.vctVctPairIntPossibleNameCombinations[j][k])
			{
				bValidPair = true;
				k = scheduleData.vctVctPairIntPossibleNameCombinations[j].size();//exit for
			}
		}
		if (!bValidPair)//try swapping Brave1 and Brave2 to create valid pairing
		{
			size_t iTmp;
			iTmp = pairPairVctNewScoreAndOffspring.second[j].first;
			pairPairVctNewScoreAndOffspring.second[j].first =
				pairPairVctNewScoreAndOffspring.second[j].second;
			pairPairVctNewScoreAndOffspring.second[j].second = iTmp;
		}
		for (size_t k = 0; k < scheduleData.vctVctPairIntPossibleNameCombinations[j].size();k++)
		{
			if (pairPairVctNewScoreAndOffspring.second[j] == scheduleData.vctVctPairIntPossibleNameCombinations[j][k])
			{
				bValidPair = true;
				k = scheduleData.vctVctPairIntPossibleNameCombinations[j].size();//exit for
			}
		}
		if (!bValidPair)//Swap in new random valid pair if unable to create valid pair from original pilots
		{
			uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
			pairPairVctNewScoreAndOffspring.second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
		}
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