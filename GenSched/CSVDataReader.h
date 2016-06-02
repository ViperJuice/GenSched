/*
 * CSVDataReader.h
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#ifndef CSVDATAREADER_H_
#define CSVDATAREADER_H_

#include "DataReader.h"
#include "FileFunctions.h"

using namespace std;

class CSVDataReader: public DataReader {
public:
	CSVDataReader();
	~CSVDataReader();
	AvailabilityData read_data(Windows::Storage::Streams::IRandomAccessStream^ stream);
private:
	AvailabilityData availabilityData;
	FileFunctions* fileFunctions;
	string sPath;
	string sFileName;
	string sPathandfFileName;
};

#endif /* CSVDATAREADER_H_ */
