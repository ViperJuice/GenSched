/*
 * DataReader.h
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 *  Virtual class acts as base class for different data readers.
 */

#ifndef DATAREADER_H_
#define DATAREADER_H_

using namespace std;

class DataReader {
public:
	virtual ~DataReader()=0;
	virtual string** read_data()=0;

private:
	string** sDataArray;
};



#endif /* DATAREADER_H_ */
