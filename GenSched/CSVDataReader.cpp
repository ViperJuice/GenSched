/*
 * CSVDataReader.cpp
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#include "CSVDataReader.h"
#include "FileFunctions.h"
#include "pch.h"
#include <string>
#include <iostream>
#include <windows.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
using namespace Windows::Storage;
using namespace std;
using namespace Windows::Storage::Pickers;
using namespace concurrency;
using namespace Windows::Devices::Enumeration;

CSVDataReader::CSVDataReader():ppDataArray(nullptr), sPath(""), sFileName("") {
	// TODO Auto-generated constructor stub

}

CSVDataReader::~CSVDataReader() {
	// TODO Auto-generated destructor stub
}
string** CSVDataReader::read_data(){
	FileFunctions* fileFunctions = new FileFunctions();

	if (fileFunctions->getActionCanceled() == false){
		ppDataArray = fileFunctions->getCSVData();
	}
	return ppDataArray;
}

 