#ifndef SCHEDULESCOREDATA_H_
#define SCHEDULESCOREDATA_H_
//Struct to contain schedule data
struct ScheduleScoreData
{
	std::vector<std::pair<std::wstring, std::wstring>> vctScheduleSpecific; //what and what score
	std::vector<std::pair<std::size_t, std::pair<std::wstring, std::wstring>>> vctDateSpecific;//date, what, what score
	std::vector<std::pair<std::wstring, std::wstring>> vctNameSpecific;//what and who
	std::vector<std::pair<std::pair<size_t, std::wstring>, std::pair<std::wstring, std::wstring>>> vctDateAndNameSpecific;//date, who, what, score

};
#endif /* SCHEDULESCOREDATA_H_ */