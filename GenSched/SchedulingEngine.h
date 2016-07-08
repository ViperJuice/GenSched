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
	virtual ScheduleData BuildSchedule(AvailabilityData availabilityData, size_t iNumberOfSchedulesToBuild);
	virtual void ConnectScheduleUpdateCallback(std::function<void(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>>)> schedulesUpdateCallback)=0;// TODO is this abstract enought for non-evolutionary build method? May need to move to child calss
	virtual void ConnectSchedulingProcessUpdateCallback(std::function<void(std::pair<size_t, std::pair<int, int>>)> schedulingProcessUpdateCallback)=0;// TODO is this abstract enought for non-evolutionary build method? May need to move to child calss

private:

	virtual ScheduleData BuildScheduleShell(AvailabilityData availabilityData);
	virtual void FillScheduleShell(AvailabilityData &availabilityData, ScheduleData &scheduleData, size_t &iNumberOfSchedulesToBuild)=0;
protected:
	ScheduleData scheduleData;
};
#endif /* SCHEDULINGENGINE_H_ */
