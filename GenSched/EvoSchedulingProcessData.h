#pragma once
class EvoSchedulingProcessData
{
public:
	EvoSchedulingProcessData();
	void EvolutionProcessUpdateCallback(std::pair<size_t, std::pair<int, int>> pairEvoProcessData);
	void SchedulesUpdateCallback(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> schedules);
	std::map<size_t, std::pair<int, int>> GetEvoProcessData();
	size_t GetSchedulesToCreate();
	size_t GetGenerationsToRun();
	std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> GetSchedules();
	void SetSchedulesToCreate(size_t iSchedulesToCreate);
	void SetGenerationsToRun(size_t iGenerationsToRun);
private:
	//holds high score and average score for each generation
	std::map<size_t, std::pair<int,int>> mapEvoProcessData;
	std::vector<std::pair<int, std::vector<std::pair<wstring,wstring>>>> schedules;
	void SetEvoProcessData(std::map<size_t, std::pair<int, int>> mapEvoProcessData);
	void SetSchedules(std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> schedules);
	size_t iSchedulesToCreate = 10;
	size_t iGenerationsToRun = 1000;
	

};



