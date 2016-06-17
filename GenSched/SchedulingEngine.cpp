#include "pch.h"
#include "SchedulingEngine.h"

SchedulingEngine::~SchedulingEngine()
{
}

ScheduleData SchedulingEngine::buildSchedule(AvailabilityData availabilityData, size_t iNumberOfSchedulesToBuild)
{
	
	scheduleData.wstrSchedules = new pair<wstring, wstring>*[iNumberOfSchedulesToBuild];
	scheduleData = buildScheduleShell(availabilityData);
	fillScheduleShell(availabilityData, scheduleData, iNumberOfSchedulesToBuild);

	return scheduleData;
}

ScheduleData SchedulingEngine::buildScheduleShell(AvailabilityData availabilityData)
{
	//Build array of correct size to hold schedule
	scheduleData.iTotalNumberOfSubPeriods = availabilityData.iNumberOfAvailabilityPeriods;
	scheduleData.iNumberOfSubPeriods = new size_t[availabilityData.iNumberOfAvailabilityPeriods];
	for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityPeriods;i++)
	{
		scheduleData.iNumberOfSubPeriods[i] = 1;//Initialize iNumberOfSubPeriods to 1
	}
	for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityPeriods;i++)
	{
		for (size_t j=0;j<availabilityData.iNumberOfAvailabilityDataRows;j++)
		{
			if (availabilityData.ppIntAvailabilityTypeArray[i][j]==AvailabilityData::DINNER_AND_MOVIE && scheduleData.iNumberOfSubPeriods[i] < 2)
			{ 
				scheduleData.iNumberOfSubPeriods[i] = 2;//Set number of sub periods to 2 for any Dinner & Movie Availability
				scheduleData.iTotalNumberOfSubPeriods++;//Increase total number of sub-periods by 1
			}
		}
	}
	scheduleData.wstrScheduleShell = new pair<wstring, wstring>[scheduleData.iTotalNumberOfSubPeriods];

	return scheduleData;
}
