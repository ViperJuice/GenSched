#ifndef SCHEDULEDATA_H_
#define SCHEDULEDATA_H_
//Struct to contain schedule data
struct ScheduleData
{
	std::pair<std::wstring, std::wstring>** wstrSchedules = nullptr;
	size_t* iNumberOfSubPeriods = nullptr; //denotes number of subperiods in any scheduling period. 
	size_t iTotalNumberOfSubPeriods;//total numner or sub-periods in entire schedule
	size_t iNumberOfAvailabilityPeriods;//Number of whole periods (days) in scheduling shell;
	std::vector<std::vector<std::pair<size_t, size_t>>> vctVctPairIntPossibleNameCombinations;//contains all name possible combinations for each sub period
};
#endif /* SCHEDULEDATA_H_ */
