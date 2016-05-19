/*
 * CSVDataReader.h
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#ifndef CSVDATAREADER_H_
#define CSVDATAREADER_H_

#include "DataReader.h"

using namespace std;

class CSVDataReader: public DataReader {
public:
	CSVDataReader();
	~CSVDataReader();
	string** read_data();
private:
	string** ppDataArray;
	string sPath;
	string sFileName;
};

#endif /* CSVDATAREADER_H_ */
