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
	.then([this](unsigned int bytes)
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
	//create vector to store availability data
	vector<vector<String^>> str2DVectorInputData;
	//Get iterator to string if .csv data
	const char16* ptrInputData = strInputData->Begin();
	//i=column index, j = row index
	unsigned int i = 0, j = 0;
	//vector to hold row of availability matrix
	vector<String^> strVct;
	//while not end of input string
	while (*ptrInputData != '\0')
	{
		//check for comma 

		if (*ptrInputData == ',')
		{
			//If first time into the data row
			if (i == 0 || *(ptrInputData + 1) == ',')
			{
				//Add blank first cell
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
	int IDColumn;
	int FirstDayColumn;
	int LastDayColumn;
	int FirstDayOfMonthColumn;
	int LastDayOfMonthColumn;
	int FirstPairRequestColumn;
	int LastPairRequestColumn;
	int FirstDataRow;
	int LastDataRow;
	int FirstQualColumn;
	int LastQualColumn;
	int FirstPreferenceColumn;
	int LastPreferenceColumn;
	int SavedDateColumn;

	for (int i = 0;i < str2DVectorInputData[0].size();i++)
	{	//Capture Column and Row Index Data
		if (str2DVectorInputData[0][i] == "IDColumn"){IDColumn = i;}
		if (str2DVectorInputData[0][i] == "FirstDayColumn"){FirstDayColumn = i;}
		if (str2DVectorInputData[0][i] == "LastDayColumn"){LastDayColumn = i;}
		if (str2DVectorInputData[0][i] == "FirstDayOfMonthColumn"){FirstDayOfMonthColumn = i;}
		if (str2DVectorInputData[0][i] == "LastDayOfMonthColumn"){LastDayOfMonthColumn = i;}
		if (str2DVectorInputData[0][i] == "FirstPairRequestColumn"){FirstPairRequestColumn = i;}
		if (str2DVectorInputData[0][i] == "LastPairRequestColumn"){LastPairRequestColumn = i;}
		if (str2DVectorInputData[0][i] == "FirstQualColumn") { FirstQualColumn = i; }
		if (str2DVectorInputData[0][i] == "LastQualColumn") { LastQualColumn = i; }
		if (str2DVectorInputData[0][i] == "FirstPreferenceColumn") { FirstPreferenceColumn = i; }
		if (str2DVectorInputData[0][i] == "LastPreferenceColumn") { LastPreferenceColumn = i; }
		if (str2DVectorInputData[0][i] == "FirstDataRow"){FirstDataRow = i;}
		if (str2DVectorInputData[0][i] == "LastDataRow"){LastDataRow = i;}

		if (str2DVectorInputData[0][i] == "SavedDateColumn"){SavedDateColumn = i;}	
		if (str2DVectorInputData[0][i] == "Des-Num-Days_Score") 
		{	//Map Scoring Data to Key Map and then Key to Score
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Des-Num-Days_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Pref-Num-Days-In-Row_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Pref-Num-Days-In-Row_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Max-Num-Days-In-Row_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Max-Num-Days-In-Row_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "On-Off-On_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"On-Off-On_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Alert-Before-Gray-Day_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Alert-Before-Gray-Day_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Dinner-and-Movie_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Dinner-and-Movie_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "2-Supers_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"2-Supers_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "2-SOFs_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"2-SOFs_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Scheduled-On-Desired-Day_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Scheduled-On-Desired-Day_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Scheduled-On-UnDesired-Day_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Scheduled-On-UnDesired-Day_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "D&M-On-Desired-Day_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"D&M-On-Desired-Day_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int (str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Bucket-Scheduled-Difference_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Bucket-Scheduled-Difference_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}
		if (str2DVectorInputData[0][i] == "Too-Many-Grey-Days_Score")
		{
			size_t key = availabilityData.mapNumberScorableType.size();
			availabilityData.mapNumberScorableType
				.insert(std::pair<int, std::wstring>(key, L"Too-Many-Grey-Days_Score"));
			availabilityData.mapScorableNumToScore
				.insert(std::pair<int, int>(key, int(str2DVectorInputData[1][i]->Data())));
		}		
	}

	const size_t numberOfDayColumns = LastDayOfMonthColumn - FirstDayColumn;
	const size_t numberOfWingmanColumns = LastPairRequestColumn - FirstPairRequestColumn;
	const size_t numberOfIDColumns = 1;
	const size_t numberOfQualColumns = LastQualColumn - FirstQualColumn;
	const size_t numberOfDataRows = LastDataRow - FirstDataRow;

	std::wstring** wstrIntDateDayDayTypeArray = nullptr;
	std::wstring** wstrAvailabilityTypeArray = nullptr;
	std::wstring** wstrWingmanPrefArray = nullptr;
	std::wstring** wstrQualArray = nullptr;
	std::wstring** wstrPrefArray = nullptr;

	//convert Platform::String Array to std::wstring Array
	std::vector <std::vector<std::wstring>> ws2DVectorInputData;
	for (int i = 0; i < str2DVectorInputData.size();i++) 
	{ 
		for (int j = 0 ;j < str2DVectorInputData[i].size(); j++)
		{
			ws2DVectorInputData[i][j] = str2DVectorInputData[i][j]->Data();
		}
	}

	wstrIntDateDayDayTypeArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstDayColumn, LastDayOfMonthColumn, 0, 2);
	wstrAvailabilityTypeArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstDayColumn, LastDayOfMonthColumn, FirstDataRow, LastDataRow);
	wstrWingmanPrefArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstPairRequestColumn, LastPairRequestColumn, FirstDataRow, LastDataRow);
	wstrQualArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstQualColumn, LastQualColumn, FirstDataRow, LastDataRow);
	wstrPrefArray = vectorParser2D<std::wstring>(ws2DVectorInputData, FirstPreferenceColumn, LastPreferenceColumn, FirstDataRow, LastDataRow);

	int** ppIntDateDayDayTypeArray;
	int** ppIntAvailabilityTypeArray;
	int** ppIntWingmanPrefArray;
	int** ppIntQualArray;
	int** ppPrefArray;

	std::map<int, std::string> mapNumberName;
	std::map<int, std::string> mapNumberQualType;
	std::map<int, int> mapNameNumToBucket;

}
//Template to cut out a section of 2D vector and return it as a 2D Array
template<typename T>
inline T** FileFunctions::vectorParser2D(std::vector<std::vector<T>> vectorToParse,
	unsigned __int32 firstColumn, unsigned __int32 lastColumn, unsigned __int32 firstRow, unsigned __int32 lastRow)
{
	const __int16 numberOfRows = lastRow - firstRow;
	const __int16 numberOfColumns = lastColumn - firstColumn;
	T** returnArray = nullptr;
	returnArray = new T*[numberOfRows];
	for (int i = 0; i < numberOfRows; i++) { returnArray[i] = new T[numberOfColumns]; }
	for (int i = 0 ; i < numberOfRows ; i++)
	{
		for (int j = 0 ; numberOfColumns - 1 ; j++)
		{
			returnArray[i][j] = vectorToParse[i + firstRow][j + firstColumn];
		}
	}
	return returnArray;
}