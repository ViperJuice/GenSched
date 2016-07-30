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
	SchedulingEngineFactory* schedulingEngineFactory = nullptr;
	SchedulingEngine* schedulingEngine = nullptr;
	pair<wstring, wstring>** wstrSchedules = nullptr;
	EvoSchedulingProcessData* evoSchedulingProcessData = nullptr;
	ScheduleData scheduleData;
	AvailabilityData* availabilityData = nullptr;
	Windows::Storage::Streams::DataReader^ reader;

};

