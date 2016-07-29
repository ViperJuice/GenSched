/*
 * FileFunctions.h
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#ifndef FILEFUNCTIONS_H_
#define FILEFUNCTIONS_H_
using namespace std;


class FileFunctions {
	public:
		FileFunctions();
		~FileFunctions();
		AvailabilityData getCSVData(Windows::Storage::Streams::IRandomAccessStream^ stream);
		Platform::String^ strInputData;
		template<typename T>
		T** vectorParser2D(std::vector<std::vector<T>> vectorToParse, unsigned __int32 firstColumn,
			unsigned __int32 lastColumn, unsigned __int32 firstRow, unsigned __int32 lastRow);
		bool wildcmp(const wchar_t* wild, const wchar_t* string);
		int WriteCVSFile(wstring wstrStringToWrite, wstring wstrFileName);

	private:
		string sPath;
		string sFileName;
		bool bActionCanceled;
		AvailabilityData availabilityData;
		vector<vector<Platform::String^>> str2DVectorInputData;
		Windows::Storage::Streams::DataReader^ dataReader;
		Windows::Storage::Streams::IRandomAccessStream^ stream;
		concurrency::task_completion_event<Platform::String^> fileSelectionComplete;
		void parseInputString();
		void buildAvailabilityData();


};


#endif /* FILEFUNCTIONS_H_ */