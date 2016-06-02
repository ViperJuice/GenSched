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
	virtual AvailabilityData read_data(Windows::Storage::Streams::IRandomAccessStream^ stream)=0;

private:
	AvailabilityData availabilityData;
};



#endif /* DATAREADER_H_ */
