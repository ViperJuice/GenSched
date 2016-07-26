#ifndef SCHEDULESCOREDATA_H_
#define SCHEDULESCOREDATA_H_
//Struct to contain schedule data
struct ScheduleScoreData
{
	std::vector<std::pair<std::wstring, int>> vctScheduleSpecific; //what and what score
	std::vector<std::pair<std::size_t, std::pair<std::wstring, int>>> vctDateSpecific;//period, what, what score
	std::vector<std::pair<std::pair<size_t, std::wstring>, int>> vctNameSpecific;//who, what, score
	std::vector<std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>>> vctDateAndNameSpecific;//period, who, what, score

};
#endif /* SCHEDULESCOREDATA_H_ */