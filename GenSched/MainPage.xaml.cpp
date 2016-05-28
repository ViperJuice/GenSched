//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace GenSched;
using namespace std;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace concurrency;
using namespace Windows::Devices::Enumeration;
using namespace concurrency;
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
	FileOpenPicker^ picker = ref new FileOpenPicker();

	picker->FileTypeFilter->Append(".csv");
	auto fileTask = create_task(picker->PickSingleFileAsync());
	fileTask.then([this](StorageFile^ file) {
		return create_task(file->OpenSequentialReadAsync());
	})
	.then(
		[this](IInputStream^ stm){
		Windows::Storage::Streams::DataReader^ reader = ref new Windows::Storage::Streams::DataReader(stm);
		reader->UnicodeEncoding = UnicodeEncoding::Utf8;
		reader->ByteOrder = ByteOrder::LittleEndian;
	});


	//task<string**> openDataFile = create_task(
	//		[this]() ->string**
	//		{
	//			dataReaderFactory = new CsvDataReaderFactory;
	//			dataReader = dataReaderFactory->create_dataReader();
	//			return dataReader->read_data();
	//		});	
	//openDataFile.then([this](task<string**> T) {
	//	try{
	//		return T;
	//	}
	//	catch(const std::exception& e)
	//	{
	//		cout << "what?";
	//		return T;
	//	}
	//})
	//	.then([this](string** strInputDataArray) {
	//	this->strInputDataArray = strInputDataArray;
	//});
}
