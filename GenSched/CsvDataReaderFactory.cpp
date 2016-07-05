#include "pch.h"
#include "CsvDataReaderFactory.h"


CsvDataReaderFactory::CsvDataReaderFactory()
{
}


CsvDataReaderFactory::~CsvDataReaderFactory()
{

}

CSVDataReader* CsvDataReaderFactory::create_dataReader()
{
	return new CSVDataReader;
}
