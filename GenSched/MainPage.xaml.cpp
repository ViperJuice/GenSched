//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace GenSched;
using namespace std;
using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
//using namespace Windows::Storage::Streams;
using namespace Windows::Devices::Enumeration;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}


void GenSched::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto reader = std::make_shared<Windows::Storage::Streams::DataReader^>(nullptr);
	FileOpenPicker^ picker = ref new FileOpenPicker();
	picker->FileTypeFilter->Append(".csv");
	auto fileTask = create_task(picker->PickSingleFileAsync());
	fileTask.then([this](StorageFile^ file)
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
			DataReaderFactory* dataReaderFactory = new CsvDataReaderFactory();
			DataReader* dataReader = dataReaderFactory->create_dataReader();
			return availabilityData = dataReader->read_data(stream);
		}
		catch (Platform::Exception^ e)
		{
			cout << e->Message->Data();
			return availabilityData;
		}
	})
	/*.then([this](Windows::Storage::Streams::IRandomAccessStream^ stream)
	{
		this->reader = ref new  Windows::Storage::Streams::DataReader(stream->GetInputStreamAt(0));
		return this->reader->LoadAsync(stream->Size);
	})
	.then([this](unsigned int bytes)
	{
		strInputData = this->reader->ReadString(bytes);
		cout << strInputData->Data();
	})*/
	.then([](AvailabilityData ad)
	{
		cout << ad.month.data();
	});
}