#include "pch.h"
#include "SchedulingEngine.h"

SchedulingEngine::~SchedulingEngine()
{

}

ScheduleData SchedulingEngine::BuildSchedule(AvailabilityData &availabilityData, size_t iNumberOfSchedulesToBuild)
{
	if (scheduleData.wstrSchedules == nullptr)
	{
		scheduleData.wstrSchedules = new pair<wstring, wstring>*[iNumberOfSchedulesToBuild];
	}
	scheduleData = BuildScheduleShell(availabilityData);
	FillScheduleShell(availabilityData, scheduleData, iNumberOfSchedulesToBuild);

	return scheduleData;
}

void SchedulingEngine::SetStopTheEngine(const bool stopTheEngine)
{
	SchedulingEngine::bStopTheEngine = stopTheEngine;
}

ScheduleData SchedulingEngine::BuildScheduleShell(AvailabilityData &availabilityData)
{
	//Build array of correct size to hold schedule
	scheduleData.iTotalNumberOfSubPeriods = availabilityData.iNumberOfAvailabilityPeriods;
	if (scheduleData.iNumberOfSubPeriods == nullptr)
	{
		scheduleData.iNumberOfSubPeriods = new size_t[availabilityData.iNumberOfAvailabilityPeriods];
	}
	scheduleData.iNumberOfAvailabilityPeriods = availabilityData.iNumberOfAvailabilityPeriods;
	for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityPeriods;i++)
	{
		scheduleData.iNumberOfSubPeriods[i] = 1;//Initialize iNumberOfSubPeriods to 1
	}
	for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
	{
		for (size_t j=0;j<availabilityData.iNumberOfAvailabilityPeriods;j++)
		{
			if (availabilityData.ppIntAvailabilityTypeArray[i][j]==AvailabilityData::DINNER_AND_MOVIE && scheduleData.iNumberOfSubPeriods[i] < 2)
			{ 
				scheduleData.iNumberOfSubPeriods[j] = 2;//Set number of sub periods to 2 for any Dinner & Movie Availability
				scheduleData.iTotalNumberOfSubPeriods++;//Increase total number of sub-periods by 1
			}
		}
	}
	

	return scheduleData;
}
