#include "pch.h"
#include "CsvDataReaderFactory.h"


CsvDataReaderFactory::CsvDataReaderFactory()
{
}


CsvDataReaderFactory::~CsvDataReaderFactory()
{
	cout << "CSVFactory Destructor" << endl;
}

CSVDataReader* CsvDataReaderFactory::create_dataReader()
{
	return new CSVDataReader;
}
