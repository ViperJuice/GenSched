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
	for (size_t i = 0;i< 10000;i++)
	{
		scoreSchedulePopulation(availabilityData, scheduleData, vctScoreAndSchedulePopulation);
		SortPopulationByScore();
		SpawnNewPopulation();
	}
	
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
void EvolutionSchedulingEngine::BuildInitialPopulation
(
	AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild
)
{
	vctScoreAndSchedulePopulation.reserve(iPopulationSize);
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		std::pair<int, std::vector<std::pair<size_t, size_t>>> pair;
		pair.first = 0;//initialize score to zero
		pair.second.reserve(scheduleData.iTotalNumberOfSubPeriods);
		vctScoreAndSchedulePopulation.push_back(pair);
	}
	random_device rd;
	mt19937 gen(rd());
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++) 
		{
			uniform_int_distribution<> dist(0,scheduleData.vctVctPairIntPossibleNameCombinations[j].size()-1);
			vctScoreAndSchedulePopulation[i].second.push_back(scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)]);
		}
	}
}
void EvolutionSchedulingEngine::scoreSchedulePopulation(AvailabilityData &availabilityData, ScheduleData &scheduleData, std::vector<std::pair<int, std::vector<std::pair<size_t, size_t>>>> &vctScoreAndSchedulePopulation)
{
	ScheduleScorer* scheduleScorer = new ScheduleScorer(availabilityData, scheduleData);
	size_t iNumberOfScoringFunctions = scheduleScorer->getFuncs().size();
	std::future<size_t>* schedScoreFutures = new future<size_t>[iPopulationSize];
	for (size_t i = 0;i < iPopulationSize;i++)
	{
		std::vector<std::pair<size_t, size_t>> vectScheduleToScore = vctScoreAndSchedulePopulation[i].second;;
		try
		{
			schedScoreFutures[i] = std::async(std::launch::async, [&, this](size_t iPopulationIndex, std::vector<std::pair<size_t, size_t>> vectScheduleToScore)->size_t
			{
				std::future<size_t>* scoreFuncFutures = new std::future<size_t>[iNumberOfScoringFunctions];
				size_t j = 0;//keep track of iterator index
				for (std::function<size_t(std::vector<std::pair<size_t, size_t>> scheduleToScore)> &itr : scheduleScorer->getFuncs())
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
	for (size_t i = 0;i < iPopulationSize;i++)
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
	//clone top pergentage of population into next generation
	size_t newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i<newPopulationSize +(iPopulationSize*iGuaranteedClonePercentage/100);i++)
	{
		vctNewScheduleAndScorePopulation.push_back(vctScoreAndSchedulePopulation[i]);
		vctNewScheduleAndScorePopulation[i].first = 0;//initialize score
		iNewPopulation++;//increment tne population size
	}
	//generate random members of new population
	random_device rd;
	mt19937 gen(rd());
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i< newPopulationSize+(iPopulationSize*iRandomPercentage/100);i++)
	{
		std::pair<size_t,std::vector<std::pair<size_t, size_t>>> vctTmp;
		vctTmp.first = 0;//initialize score
		vctTmp.second.reserve(scheduleData.iTotalNumberOfSubPeriods);
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
			vctTmp.second.push_back(scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)]);
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
		vctNewScheduleAndScorePopulation.push_back(vctScoreAndSchedulePopulation[iIndexToClone]);
		vctNewScheduleAndScorePopulation[i].first = 0;//initialize score
		iNewPopulation++;//increment new population size
	}
	//generate new population by cloning with mutation
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i< newPopulationSize + (iPopulationSize * iClonedWithMutationPercentage/100);i++)
	{
		//converts uniform distribution to triangular distribution a=0 c=0 b=iPopulationSize
		size_t  iIndexToClone = (iPopulationSize - 1)*(1 - (sqrt(1 - dist1(gen))));
		vctNewScheduleAndScorePopulation.push_back(vctScoreAndSchedulePopulation[iIndexToClone]);
		//insert mutations
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{

			uniform_real_distribution<> dist(0, 1);
			if (dist(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size()-1);
				vctNewScheduleAndScorePopulation[i].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
		}
		vctNewScheduleAndScorePopulation[i].first = 0;//initialize score
		iNewPopulation++;//increment new population size
	}
	//Build Breeding Population
	std::vector<std::vector<std::pair<size_t, size_t>>>  vctVctPairBreeders(2*(iPopulationSize - iNewPopulation));
	size_t iBreedingPopulationSize=0;
	for (size_t i = 0;i < (iMateGuaranteed / 100)*iPopulationSize;i++)
	{
		vctVctPairBreeders[iBreedingPopulationSize]=vctScoreAndSchedulePopulation[i].second;
		iBreedingPopulationSize++;
	}
	uniform_int_distribution<> dist(0, vctScoreAndSchedulePopulation.size()-1);
	size_t iBreedersToAdd = 2 * (iPopulationSize - iNewPopulation) - iBreedingPopulationSize;
	for (size_t i = 0; i < iBreedersToAdd;i++)
	{
		vctVctPairBreeders[iBreedingPopulationSize] = vctScoreAndSchedulePopulation[dist(gen)].second;
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
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		//insert mutations
		uniform_real_distribution<> dist1(0, 1);
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[iNewPopulation-1].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
		}
		pairPairVctNewScoreAndOffspring.first = 0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring2;
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[iNewPopulation-1].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
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
	}
	//Sexual Reproduction By Chopping
	newPopulationSize = iNewPopulation;
	for (size_t i = newPopulationSize;i < newPopulationSize + (iPopulationSize*iSexualWithChoppingPercentage / 100);i += 2)
	{
		uniform_int_distribution<> dist(0, vctVctPairBreeders.size() - 1);
		size_t iIndex1 = dist(gen);
		size_t iIndex2 = dist(gen);
		while (iIndex1 == iIndex2)//Insure different indexes
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
				pairGene2.first = vctVctPairBreeders[iIndex1][j].first;
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
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		//insert mutations
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[i].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
		}
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring2;
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		//insert mutations
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[i].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
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
		pairPairVctNewScoreAndOffspring.first = 0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring1;
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		//insert mutations
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[iNewPopulation-1].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
		}
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			bool bValidPair= false;
			for (size_t k = 0; k < scheduleData.vctVctPairIntPossibleNameCombinations[j].size();k++)
			{
				if (vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] == scheduleData.vctVctPairIntPossibleNameCombinations[j][k])
				{
					bValidPair = true;
				}
			}
			if (!bValidPair)
			{
				size_t iTmp;
				iTmp=vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].first;
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].first =
					vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].second;
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].second = iTmp;
			}
			for (size_t k = 0; k < scheduleData.vctVctPairIntPossibleNameCombinations[j].size();k++)
			{
				if (vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] == scheduleData.vctVctPairIntPossibleNameCombinations[j][k])
				{
					bValidPair = true;
				}
			}
			if (!bValidPair)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
			

			
		}
		pairPairVctNewScoreAndOffspring.first = 0;
		pairPairVctNewScoreAndOffspring.second = vctPairOffspring2;
		vctNewScheduleAndScorePopulation.push_back(pairPairVctNewScoreAndOffspring);
		iNewPopulation++;
		//insert mutations
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			if (dist1(gen) < iMutationRate / 100)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}
		}
		for (size_t j = 0;j < scheduleData.iTotalNumberOfSubPeriods;j++)
		{
			bool bValidPair = false;
			for (size_t k = 0; k < scheduleData.vctVctPairIntPossibleNameCombinations[j].size();k++)
			{
				if (vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] == scheduleData.vctVctPairIntPossibleNameCombinations[j][k])
				{
					bValidPair = true;
				}
			}
			if (!bValidPair)
			{
				size_t iTmp;
				iTmp = vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].first;
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].first =
					vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].second;
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j].second = iTmp;
			}
			for (size_t k = 0; k < scheduleData.vctVctPairIntPossibleNameCombinations[j].size();k++)
			{
				if (vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] == scheduleData.vctVctPairIntPossibleNameCombinations[j][k])
				{
					bValidPair = true;
				}
			}
			if (!bValidPair)
			{
				uniform_int_distribution<> dist(0, scheduleData.vctVctPairIntPossibleNameCombinations[j].size() - 1);
				vctNewScheduleAndScorePopulation[iNewPopulation - 1].second[j] = scheduleData.vctVctPairIntPossibleNameCombinations[j][dist(gen)];
			}



		}
	}
	vctScoreAndSchedulePopulation = vctScoreAndSchedulePopulation;
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