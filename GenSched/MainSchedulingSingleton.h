#pragma once
class MainSchedulingSingleton
{
public:
	static MainSchedulingSingleton* Instance();
	void ImportSchedulingData();
	void BuildSchedules();
	void ForceCompleteSchedulingProcess();
protected:
	MainSchedulingSingleton();
	~MainSchedulingSingleton();
private:
	static MainSchedulingSingleton* _instance;
	size_t iNumberOfSchedulesToBuild = 10;
	SchedulingEngineFactory* schedulingEngineFactory;
	SchedulingEngine* schedulingEngine;
	pair<wstring, wstring>** wstrSchedules;
	EvoSchedulingProcessData* evoSchedulingProcessData;
	ScheduleData scheduleData;
	ScheduleScoreData scheduleScoreData;
	AvailabilityData* availabilityData;
	Windows::Storage::Streams::DataReader^ reader;

};

