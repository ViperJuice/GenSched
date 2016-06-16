#ifndef SCHEDULEDATA_H_
#define SCHEDULEDATA_H_
//Struct to contain schedule data
struct ScheduleData
{
	std::pair<std::wstring, std::wstring>** wstrSchedules;
	size_t* iNumberOfSubPeriods; //denotes number of subperiods in any scheduling period. 
	size_t iTotalNumberOfSubPeriods;
	std::pair<std::wstring, std::wstring>* wstrScheduleShell;
};
#endif /* SCHEDULEDATA_H_ */
