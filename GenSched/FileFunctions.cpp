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
	create_task([this, stream]()
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
	.then([](task<void> t) 
	{
		try {
			t.get();
		}
		catch (Platform::Exception^ e)
		{
			cout << e->Message->Data();
		}
	});
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
	availabilityData.year = int (str2DVectorInputData[0][1]->Data());
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

	std::wstring** wstrIntDateDayDayTypeArray = nullptr;
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

	wstrIntDateDayDayTypeArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstDayColumn - 1, LastDayOfMonthColumn - 1, 0, 2);
	wstrAvailabilityTypeArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstDayColumn - 1, LastDayOfMonthColumn - 1, FirstDataRow - 1, LastDataRow - 1);
	wstrWingmanPrefArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstPairRequestColumn - 1, LastPairRequestColumn - 1, FirstDataRow - 1, LastDataRow - 1);
	wstrQualArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstQualColumn - 1, LastQualColumn - 1, FirstDataRow - 1, LastDataRow - 1);
	wstrPrefArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstPreferenceColumn - 1, LastPreferenceColumn - 1, FirstDataRow - 1, LastDataRow - 1);

	int** ppIntDateDayDayTypeArray;
	int** ppIntAvailabilityTypeArray;
	int** ppIntWingmanPrefArray;
	int** ppIntQualArray;
	int** ppPrefArray;

	ppIntDateDayDayTypeArray = new int*[3];
	for (size_t i = 0;i < 3;i++)
	{
		ppIntDateDayDayTypeArray[i] = new int[numberOfDayColumns];
	}

	ppIntAvailabilityTypeArray = new int*[numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		ppIntAvailabilityTypeArray[i] = new int[numberOfDayColumns];
	}

	ppIntWingmanPrefArray = new int*[numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		ppIntWingmanPrefArray[i] = new int[numberOfWingmanColumns];
	}

	ppIntQualArray = new int*[numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		ppIntQualArray[i] = new int[numberOfQualColumns];
	}

	ppPrefArray = new int*[numberOfDataRows];
	for (size_t i = 0;i < numberOfDataRows;i++)
	{
		ppPrefArray[i] = new int[numberOfPrefColumns];
	}

	for(size_t i = FirstDataRow - 1;i < LastDataRow - 1;i++)
	{
		availabilityData.mapNumberName.insert(std::pair<size_t, wstring>(i - FirstDataRow, ws2DVectorInputData[i][IDColumn-1]));
	}

	for (size_t i = FirstQualColumn - 1; i < LastQualColumn - 1;i++)
	{
		availabilityData.mapNumberQualType.insert(std::pair<size_t, wstring>(i - FirstQualColumn, ws2DVectorInputData[QualRow][i]));
	}
	
	for (size_t i = FirstDataRow - 1;i < LastDataRow - 1;i++)
	{
		availabilityData.mapNameNumToBucket.insert(std::pair<size_t, size_t>(i - FirstDataRow, stoi(ws2DVectorInputData[i][BucketColumn-1])));
	}

	for (size_t i = FirstPreferenceColumn - 1;i < LastPreferenceColumn;i++)
	{
		availabilityData.mapNumberPrefType.insert(std::pair<size_t, wstring>(i - FirstPreferenceColumn, ws2DVectorInputData[PrefRow - 1][i]));
	}



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
		for (int j = 0 ; j < numberOfColumns - 1 ; j++)
		{
			returnArray[i][j] = vectorToParse[i + firstRow][j + firstColumn];
		}
	}
	return returnArray;
}


bool FileFunctions::wildcmp(const wchar_t *wild, const wchar_t *string) {
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
