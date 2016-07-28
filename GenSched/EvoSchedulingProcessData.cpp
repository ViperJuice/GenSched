#include "pch.h"
#include "EvoSchedulingProcessData.h"


EvoSchedulingProcessData::EvoSchedulingProcessData()
{
}

void EvoSchedulingProcessData::EvolutionProcessUpdateCallback(std::pair<size_t, std::pair<int, int>> pairEvoProcessData)
{
	EvoSchedulingProcessData::mapEvoProcessData.insert(pairEvoProcessData);
}

void EvoSchedulingProcessData::SchedulesUpdateCallback(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> schedules)
{
	EvoSchedulingProcessData::schedules = schedules;
}

void EvoSchedulingProcessData::ScheduleScoreDataUpdataCallback(std::vector<ScheduleScoreData> vctScheduleScoreData)
{
	EvoSchedulingProcessData::vctScheduleScoreData = vctScheduleScoreData;
}

void EvoSchedulingProcessData::SetEvoProcessData(std::map<size_t, std::pair<int, int>> mapEvoProcessData)
{
	EvoSchedulingProcessData::mapEvoProcessData = mapEvoProcessData;
}

std::map<size_t, std::pair<int, int>> EvoSchedulingProcessData::GetEvoProcessData()
{
	return mapEvoProcessData;
}

void EvoSchedulingProcessData::SetSchedules(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> schedules)
{
	EvoSchedulingProcessData::schedules = schedules;
}

std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> EvoSchedulingProcessData::GetSchedules()
{
	return schedules;;
}

size_t EvoSchedulingProcessData::GetSchedulesToCreate()
{
	return iSchedulesToCreate;;
}
void EvoSchedulingProcessData::SetSchedulesToCreate(size_t iSchedulesToCreate)
{
	EvoSchedulingProcessData::iSchedulesToCreate = iSchedulesToCreate;
}

size_t EvoSchedulingProcessData::GetGenerationsToRun()
{
	return iGenerationsToRun;
}

void EvoSchedulingProcessData::SetGenerationsToRun(size_t iGenerationsToRun)
{
	EvoSchedulingProcessData::iGenerationsToRun = iGenerationsToRun;
}