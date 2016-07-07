#pragma once
class MainSchedulingSingleton
{
public:
	static MainSchedulingSingleton* Instance();
	void RunSchedulingProcess();
protected:
	MainSchedulingSingleton();
	~MainSchedulingSingleton();
private:
	static MainSchedulingSingleton* _instance;
	size_t iNumberOfSchedulesToBuild = 10;
	pair<wstring, wstring>** wstrSchedules;
	EvoSchedulingProcessData* evoSchedulingProcessData;
	ScheduleData scheduleData;
	AvailabilityData availabilityData;
	Windows::Storage::Streams::DataReader^ reader;

};

