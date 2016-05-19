#pragma once
#include "DataReaderFactory.h"
class CsvDataReaderFactory :
	public DataReaderFactory
{
public:
	CsvDataReaderFactory();
	~CsvDataReaderFactory();
	CSVDataReader* create_dataReader();

	
};

