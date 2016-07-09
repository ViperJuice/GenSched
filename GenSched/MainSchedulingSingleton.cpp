#include "pch.h"
#include "MainSchedulingSingleton.h"
using namespace Windows::Foundation;
using namespace std;
using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;

MainSchedulingSingleton::MainSchedulingSingleton() 
{
}
MainSchedulingSingleton::~MainSchedulingSingleton()
{
	delete schedulingEngine;
	delete evoSchedulingProcessData;
}
MainSchedulingSingleton* MainSchedulingSingleton::_instance = 0;

MainSchedulingSingleton* MainSchedulingSingleton::Instance() {
	if (_instance == 0) {
		_instance = new MainSchedulingSingleton;
	}
	return _instance;
}

//starts and runs the scheduling program
void MainSchedulingSingleton::RunSchedulingProcess()
{
	auto reader = std::make_shared<Windows::Storage::Streams::DataReader^>(nullptr);
	FileOpenPicker^ picker = ref new FileOpenPicker();
	picker->FileTypeFilter->Append(".csv");
	auto fileTask = create_task(picker->PickSingleFileAsync());
	fileTask.then([&, this](StorageFile^ file)
	{
		if (file)
		{
			return file->OpenAsync(FileAccessMode::Read);
		}
		else
		{
			throw task_canceled();
		}
	})
		.then([this](Windows::Storage::Streams::IRandomAccessStream^ stream)
	{
		try
		{
			unique_ptr<DataReaderFactory> dataReaderFactory(new CsvDataReaderFactory());
			unique_ptr<DataReader> dataReader(dataReaderFactory->create_dataReader());
			AvailabilityData availabilityData = dataReader->read_data(stream);
			return availabilityData;

		}
		catch (Platform::Exception^ e)
		{
			Platform::String^ msg = "Platform Exception";
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown  Exception";
			OutputDebugString(msg->Data());
		}
	}, task_continuation_context::use_arbitrary())
		.then([this](AvailabilityData availabilityData)
	{
		EvoSchedulingProcessData* evoSchedulingProcessData = new EvoSchedulingProcessData();
		unique_ptr<SchedulingEngineFactory> schedulingEngineFactory(new EvolutionSchedulingEngineFactory());
		schedulingEngine = schedulingEngineFactory->create_schedulingEngine();
		schedulingEngine->ConnectScheduleUpdateCallback([&evoSchedulingProcessData](std::vector<std::pair<int, std::vector<std::pair<wstring, wstring>>>> schedulesUpdateCallback)
		{
			return evoSchedulingProcessData->SchedulesUpdateCallback(schedulesUpdateCallback);
		});
		schedulingEngine->ConnectSchedulingProcessUpdateCallback([&evoSchedulingProcessData](std::pair<size_t, std::pair<int, int>> schedulingProcessUpdateCallback)
		{
			return evoSchedulingProcessData->EvolutionProcessUpdateCallback(schedulingProcessUpdateCallback);
		});
		scheduleData = schedulingEngine->BuildSchedule(availabilityData, iNumberOfSchedulesToBuild);
	});

}

void MainSchedulingSingleton::ForceCompleteSchedulingProcess()
{
	schedulingEngine->SetStopTheEngine(true);
}

