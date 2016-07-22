#ifndef SCHEDULESCOREDATA_H_
#define SCHEDULESCOREDATA_H_
//Struct to contain schedule data
struct ScheduleScoreData
{
	std::vector<std::pair<std::wstring, int>> vctScheduleSpecific;
	std::vector<std::pair<std::wstring, std::pair<size_t, size_t>>> vctDateSpecific;
	std::vector<std::pair<std::wstring, std::wstring>> vctNameSpecific;
	std::vector<std::pair<std::pair<std::wstring, std::wstring>, std::pair<size_t, size_t>>> vctDateAndNameSpecific;

};
#endif /* SCHEDULESCOREDATA_H_ */