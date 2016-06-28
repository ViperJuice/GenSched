/*
 * FileFunctions.cpp
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#include "FileFunctions.h"
#include "pch.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <thread>

using namespace Windows::Storage;
using namespace std;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace concurrency;
using namespace Windows::Devices::Enumeration;
using namespace Platform;


FileFunctions::FileFunctions():sPath(""), sFileName(""), bActionCanceled(false)
{

}

FileFunctions::~FileFunctions()
{

}


AvailabilityData FileFunctions::getCSVData(Windows::Storage::Streams::IRandomAccessStream^ stream) 
{
	auto t = create_task([this, stream]()
	{
		this->dataReader = ref new  Windows::Storage::Streams::DataReader(stream->GetInputStreamAt(0));
		return this->dataReader->LoadAsync(stream->Size);
	})
	.then([this](size_t bytes)
	{
		strInputData = this->dataReader->ReadString(bytes);
		cout << strInputData->Data();
	})
	.then([this]()
	{
		this->parseInputString();
	})
	.then([this]() 
	{
		buildAvailabilityData();
	})
	.then([](task<void> tsk) 
	{
		try
		{
			tsk.get();
		}
		catch (Platform::Exception^ e)
		{
			cout << e->Message->Data();
		}
	});
	try 
	{
		t.wait();
	}
	catch (Platform::Exception^ e)
	{
		Platform::String^ msg("Platform Error = ");
		OutputDebugString(msg->Data());
	}
	catch (std::exception e)
	{
		Platform::String^ msg("Standard Error = ");
		OutputDebugString(msg->Data());
	}
	catch (...)
	{
		Platform::String^ msg("Unkown Error = ");
		OutputDebugString(msg->Data());
	}
	return availabilityData;
}


void FileFunctions::parseInputString()
{
	//Get iterator to string if .csv data
	const char16* ptrInputData = strInputData->Begin();
	//i=column index, j = row index
	size_t i = 0, j = 0;
	//vector to hold row of availability matrix
	vector<String^> strVct;
	//while not end of input string
	while (*ptrInputData != '\0')
	{
		//check for comma 

		if (*ptrInputData == ',')
		{
			//If first time into the data row and first charater is a comma
			if (i == 0)
			{
				//Add blank cell put don't increment string pointer ptrInputData
				strVct.push_back("");
				i++;
			}
			//If last char befor newline is a comma add a blank cell
			else if (*(ptrInputData + 1) == '\r' && *((ptrInputData + 2)) == '\n')
			{
				//Add blank cell
				strVct.push_back("");
				i++;
				ptrInputData++;
			}
			else if (*(ptrInputData + 1) == ',')
			{
				//Add blank cell
				strVct.push_back("");
				i++;
				ptrInputData++;
			}
			else
			{
				i++;
				ptrInputData++;
			}
		}
		else if ((*ptrInputData == '\r' && *(ptrInputData + 1) == '\n'))
		{
			str2DVectorInputData.push_back(strVct);
			ptrInputData++;
			ptrInputData++;
			j++;
			i = 0;
			strVct.clear();
		}
		else
		{
			String^ str;
			while (*ptrInputData != ','  && *ptrInputData != '\0' && !(*ptrInputData == '\r' && *(ptrInputData + 1) == '\n'))
			{
				str += *ptrInputData++;
			}
			strVct.push_back(str);
			i++;
		}
	}
}
void FileFunctions::buildAvailabilityData()
{	
	availabilityData.month = str2DVectorInputData[0][0]->Data();
	availabilityData.year = _wtoi(str2DVectorInputData[0][1]->Data());
	size_t IDColumn;
	size_t FirstDayColumn;
	size_t LastDayColumn;
	size_t FirstDayOfMonthColumn;
	size_t LastDayOfMonthColumn;
	size_t FirstPairRequestColumn;
	size_t LastPairRequestColumn;
	size_t BucketColumn;
	size_t FirstDataRow;
	size_t LastDataRow;
	size_t DayOfMonthRow;
	size_t WeekDayRow;
	size_t DayTypeRow;
	size_t PrefRow;
	size_t QualRow;
	size_t FirstQualColumn;
	size_t LastQualColumn;
	size_t FirstPreferenceColumn;
	size_t LastPreferenceColumn;
	size_t SavedDateColumn;
	for (int i = 0;i < str2DVectorInputData[0].size();i++)
	{	//Capture Column and Row Index Data
		if (str2DVectorInputData[0][i] == "IDColumn"){IDColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "FirstDayColumn"){FirstDayColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "LastDayColumn"){LastDayColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "FirstDayOfMonthColumn") {FirstDayOfMonthColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "LastDayOfMonthColumn") {LastDayOfMonthColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "FirstPairRequestColumn"){FirstPairRequestColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "LastPairRequestColumn"){LastPairRequestColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "FirstQualColumn") {FirstQualColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "LastQualColumn") {LastQualColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "FirstPreferenceColumn") {FirstPreferenceColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "LastPreferenceColumn") {LastPreferenceColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "BucketColumn") {BucketColumn = _wtoi(str2DVectorInputData[1][i]->Data()); }
		if (str2DVectorInputData[0][i] == "FirstDataRow"){FirstDataRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "LastDataRow"){LastDataRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "DayOfMonthRow") {DayOfMonthRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "WeekDayRow") {WeekDayRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "DayTypeRow") {DayTypeRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "PrefRow") {PrefRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "QualRow") {QualRow = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (str2DVectorInputData[0][i] == "SavedDateColumn"){SavedDateColumn = _wtoi(str2DVectorInputData[1][i]->Data());}
		if (wildcmp(L"*_Score", str2DVectorInputData[0][i]->Data()))
		{	//Map Scoring Data to Key Map and then Key to Score
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, str2DVectorInputData[0][i]->Data()));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, _wtoi(str2DVectorInputData[1][i]->Data())));
		}
	}

	const size_t numberOfDayColumns = (LastDayOfMonthColumn - FirstDayColumn) + 1;
	const size_t numberOfWingmanColumns = (LastPairRequestColumn - FirstPairRequestColumn) + 1;
	const size_t numberOfIDColumns = 1;
	const size_t numberOfQualColumns = (LastQualColumn - FirstQualColumn) + 1;
	const size_t numberOfPrefColumns = (LastPreferenceColumn - FirstPreferenceColumn) + 1;
	const size_t numberOfDataRows = (LastDataRow - FirstDataRow) + 1;

	std::wstring** wstrDateDayDayTypeArray = nullptr;
	std::wstring** wstrAvailabilityTypeArray = nullptr;
	std::wstring** wstrWingmanPrefArray = nullptr;
	std::wstring** wstrQualArray = nullptr;
	std::wstring** wstrPrefArray = nullptr;

	//convert Platform::String Array to std::wstring Array
	std::vector <std::vector<std::wstring>> ws2DVectorInputData;
	for (int i = 0;i < str2DVectorInputData.size();i++) 
	{ 
		std::vector<std::wstring> wstrVct;
		for (int j = 0;j < str2DVectorInputData[i].size();j++)
		{
			wstrVct.push_back(str2DVectorInputData[i][j]->Data());
		}
		ws2DVectorInputData.push_back(wstrVct);
		wstrVct.clear();
	}

	wstrDateDayDayTypeArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstDayColumn - 1, LastDayOfMonthColumn - 1, 0, 2);
	wstrAvailabilityTypeArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstDayColumn - 1, LastDayOfMonthColumn - 1, FirstDataRow - 1, LastDataRow - 1);
	wstrWingmanPrefArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstPairRequestColumn - 1, LastPairRequestColumn - 1, FirstDataRow - 1, LastDataRow - 1);
	wstrQualArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstQualColumn - 1, LastQualColumn - 1, FirstDataRow - 1, LastDataRow - 1);
	wstrPrefArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstPreferenceColumn - 1, LastPreferenceColumn - 1, FirstDataRow - 1, LastDataRow - 1);

	availabilityData.iNumberOfAvailabilityPeriods = numberOfDayColumns;
	availabilityData.iNumberOfAvailabilityDataRows = numberOfDataRows;

	for(size_t i = FirstDataRow - 1;i < LastDataRow;i++)
	{
		availabilityData.mapNumberName.insert(std::pair<size_t, wstring>(i - (FirstDataRow - 1), ws2DVectorInputData[i][IDColumn - 1]));
	}

	for (size_t i = FirstQualColumn - 1; i < LastQualColumn;i++)
	{
		availabilityData.mapNumberQualType.insert(std::pair<size_t, wstring>(i - (FirstQualColumn - 1), ws2DVectorInputData[QualRow - 1][i]));
	}
	
	for (size_t i = FirstDataRow - 1;i < LastDataRow;i++)
	{
		availabilityData.mapNameNumToBucket.insert(std::pair<size_t, size_t>(i - (FirstDataRow - 1), stoi(ws2DVectorInputData[i][BucketColumn - 1])));
	}

	for (size_t i = FirstPreferenceColumn - 1;i < LastPreferenceColumn;i++)
	{
		availabilityData.mapNumberPrefType.insert(std::pair<size_t, wstring>(i - (FirstPreferenceColumn - 1), ws2DVectorInputData[PrefRow - 1][i]));
	}
	for (auto i = availabilityData.mapNumberScorableType.begin();i != availabilityData.mapNumberScorableType.end();i++)
	{
		for (auto j = availabilityData.mapNumberPrefType.begin();j != availabilityData.mapNumberPrefType.end();j++) 
		{

			if (wildcmp((j->second + L"*").c_str(), i->second.c_str()))
			{
				availabilityData.mapPrefNumToScorableNum.insert(std::pair<size_t, size_t>(j->first, i->first));
			}
		}
		
	}

	availabilityData.ppIntDateDayDayTypeArray = nullptr;
	availabilityData.ppIntAvailabilityTypeArray = nullptr;
	availabilityData.ppIntWingmanPrefArray = nullptr;
	availabilityData.ppBoolQualArray = nullptr;
	availabilityData.ppIntPrefArray = nullptr;

	availabilityData.ppIntDateDayDayTypeArray = new int*[3];
	for (size_t i = 0;i < 3;i++)
	{
		availabilityData.ppIntDateDayDayTypeArray[i] = new int[numberOfDayColumns];
	}
	for (size_t i = 0;i < numberOfDayColumns; i++)
	{
		if (wstrDateDayDayTypeArray[0][i] == L"Sun")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Sun;
		}
		else if (wstrDateDayDayTypeArray[0][i] == L"Mon")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Mon;
		}
		else if (wstrDateDayDayTypeArray[0][i] == L"Tue")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Tue;
		}
		else if (wstrDateDayDayTypeArray[0][i] == L"Wed")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Wed;
		}
		else if (wstrDateDayDayTypeArray[0][i] == L"Thu")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Thu;
		}
		else if (wstrDateDayDayTypeArray[0][i] == L"Fri")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Fri;
		}
		else if (wstrDateDayDayTypeArray[0][i] == L"Sat")
		{
			availabilityData.ppIntDateDayDayTypeArray[0][i] = AvailabilityData::Sat;
		}
	}
	for (size_t i = 0;i < numberOfDayColumns; i++)
	{
		if (wstrDateDayDayTypeArray[1][i] == L"F")
		{
			availabilityData.ppIntDateDayDayTypeArray[1][i] = AvailabilityData::FLY_DAY;
		}
		else if (wstrDateDayDayTypeArray[1][i] == L"W")
		{
			availabilityData.ppIntDateDayDayTypeArray[1][i] = AvailabilityData::NO_FLY_WORK_DAY;
		}
		else if (wstrDateDayDayTypeArray[1][i] == L"G")
		{
			availabilityData.ppIntDateDayDayTypeArray[1][i] = AvailabilityData::GREY_DAY;
		}
	}
	for (size_t i = 0;i < numberOfDayColumns; i++)
	{
		availabilityData.ppIntDateDayDayTypeArray[2][i] = stoi(wstrDateDayDayTypeArray[2][i]);
	}

	availabilityData.ppIntAvailabilityTypeArray = new int*[numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		availabilityData.ppIntAvailabilityTypeArray[i] = new int[numberOfDayColumns];
		for (size_t j = 0; j < numberOfDayColumns;j++)
		{
			if (wstrAvailabilityTypeArray[i][j] == L"-2")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::PLEASE_NO;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"-1")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::RATHER_NOT;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"0")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::DONT_CARE;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"1")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::ALERT_IS_FINE;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"2")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::ALERT_PLEASE;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"D")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::DINNER_AND_MOVIE;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"X")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::UNAVAILABLE;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"T")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::TDY;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"L")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::LEAVE;
			}
			else if (wstrAvailabilityTypeArray[i][j] == L"F")
			{
				availabilityData.ppIntAvailabilityTypeArray[i][j] = AvailabilityData::CT_FLY_DAY;
			}

		}
	}

	availabilityData.ppIntWingmanPrefArray = new int*[numberOfDataRows];
	//Convert wstring array to int array with int key from mapNumToNameMap
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		availabilityData.ppIntWingmanPrefArray[i] = new int[numberOfWingmanColumns];
		for (size_t j = 0;j < numberOfWingmanColumns;j++)
		{
			if (wstrWingmanPrefArray[i][j] != L"" && wstrWingmanPrefArray[i][j] != L"0")
			{
				//lookup integer key by name
				const wstring nameToFind = wstrWingmanPrefArray[i][j];
				auto findResult = std::find_if(std::begin(availabilityData.mapNumberName), std::end(availabilityData.mapNumberName), [&](const std::pair<int, wstring> &pair)
				{
					return pair.second == nameToFind;
				});

				size_t foundKey = -1; // You might want to initialise this to a value you know is invalid in your map

				if (findResult != std::end(availabilityData.mapNumberName))
				{
					foundKey = findResult->first;

					availabilityData.ppIntWingmanPrefArray[i][j] = foundKey;
				}
			}
			else
			{
				availabilityData.ppIntWingmanPrefArray[i][j] = -1;
			}
		}
	}

	availabilityData.ppBoolQualArray = new bool* [numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		availabilityData.ppBoolQualArray[i] = new bool[numberOfQualColumns];
		for (size_t j = 0;j < numberOfQualColumns;j++)
		{
			if (wstrQualArray[i][j] == L"True" || wstrQualArray[i][j] == L"true" || wstrQualArray[i][j] == L"T" || wstrQualArray[i][j] == L"t" ||
				wstrQualArray[i][j] == L"1" || wstrQualArray[i][j] == L"Yes" || wstrQualArray[i][j] == L"yes" || wstrQualArray[i][j] == L"Y" || wstrQualArray[i][j] == L"y")
			{
				availabilityData.ppBoolQualArray[i][j] = true;
			}
			else
			{
				availabilityData.ppBoolQualArray[i][j] = false;
			}
		}

	}

	availabilityData.ppIntPrefArray = new int*[numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		availabilityData.ppIntPrefArray[i] = new int[numberOfPrefColumns];
		for (size_t j = 0;j < numberOfPrefColumns;j++)
		{
			try 
			{
				availabilityData.ppIntPrefArray[i][j] = stoi(wstrPrefArray[i][j]);
			}
			catch (invalid_argument e) //Handles Preferences that are not numeric
			{
				if (wstrPrefArray[i][j] == L"Hate It!") 
				{
					availabilityData.ppIntPrefArray[i][j] = AvailabilityData::HATE_IT;
				}
				else if (wstrPrefArray[i][j] == L"Don't Like It")
				{
					availabilityData.ppIntPrefArray[i][j] = AvailabilityData::DONT_LIKE_IT;
				}
				else if (wstrPrefArray[i][j] == L"Don't Care")
				{
					availabilityData.ppIntPrefArray[i][j] = AvailabilityData::DONT_CARE;
				}
				else if (wstrPrefArray[i][j] == L"Like It")
				{
					availabilityData.ppIntPrefArray[i][j] = AvailabilityData::LIKE_IT;
				}
				else if (wstrPrefArray[i][j] == L"Love It!")
				{
					availabilityData.ppIntPrefArray[i][j] = AvailabilityData::LOVE_IT;
				}
			}
		}
	}
	//TODO add flying event capture and parsing
}


//Template to cut out a section of 2D vector and return it as a 2D Array
template<typename T>
inline T** FileFunctions::vectorParser2D(std::vector<std::vector<T>> vectorToParse,
	unsigned __int32 firstColumn, unsigned __int32 lastColumn, unsigned __int32 firstRow, unsigned __int32 lastRow)
{
	const __int16 numberOfRows = (lastRow - firstRow) + 1;
	const __int16 numberOfColumns = (lastColumn - firstColumn) + 1;
	T** returnArray = nullptr;
	returnArray = new T*[numberOfRows];
	for (int i = 0; i < numberOfRows; i++) {returnArray[i] = new T[numberOfColumns]; }
	for (int i = 0 ; i < numberOfRows ; i++)
	{
		for (int j = 0 ; j < numberOfColumns; j++)
		{
			returnArray[i][j] = vectorToParse[i + firstRow][j + firstColumn];
		}
	}
	return returnArray;
}


bool FileFunctions::wildcmp(const wchar_t *wild, const wchar_t *string) 
{
	// Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
	const wchar_t *cp = NULL, *mp = NULL;

	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return false;
		}
		wild++;
		string++;
	}

	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return true;
			}
			mp = wild;
			cp = string + 1;
		}
		else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		}
		else {
			wild = mp;
			string = cp++;
		}
	}

	while (*wild == '*') {
		wild++;
	}
	return !*wild;
}
