#pragma once
class MainSchedulingSingleton
{
public:
	static MainSchedulingSingleton* Instance();
	void RunSchedulingProcess();
protected:
	MainSchedulingSingleton();
private:
	static MainSchedulingSingleton* _instance;
	size_t iNumberOfSchedulesToBuild = 10;
	pair<wstring, wstring>** wstrSchedules;
	ScheduleData scheduleData;
	AvailabilityData availabilityData;
	Windows::Storage::Streams::DataReader^ reader;

};

