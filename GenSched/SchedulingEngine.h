/*
* SchedulingEngine.h
*
*  Created on: June 16, 2016
*      Author: Jenner Torrence
*  Virtual class acts as base class for different scheduling engines.
*/

#ifndef SCHEDULINGENGINE_H_
#define SCHEDULINGENGINE_H_

using namespace std;
class SchedulingEngine
{
public:
	virtual ~SchedulingEngine()=0;
	virtual ScheduleData buildSchedule(AvailabilityData availabilityData, size_t iNumberOfSchedulesToBuild);

private:
	ScheduleData scheduleData;
	virtual ScheduleData buildScheduleShell(AvailabilityData availabilityData);
	virtual void fillScheduleShell(AvailabilityData availabilityData, ScheduleData scheduleData, size_t iNumberOfSchedulesToBuild)=0;

};
#endif /* SCHEDULINGENGINE_H_ */