﻿//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "CsvDataReaderFactory.h"

namespace GenSched
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		Platform::String^ strInputData;
		size_t iNumberOfSchedulesToBuild = 10;
		pair<wstring,wstring>** wstrSchedules;
		ScheduleData scheduleData;
		AvailabilityData availabilityData;
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		Windows::Storage::Streams::DataReader^ reader;
	};
}
