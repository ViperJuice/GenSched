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
	virtual ScheduleData buildSchedule(AvailabilityData, size_t);

private:
	ScheduleData scheduleData;
	virtual size_t* buildScheduleShell(AvailabilityData)=0;
	virtual void fillScheduleShell(AvailabilityData, size_t*)=0;

};
#endif /* SCHEDULINGENGINE_H_ */
