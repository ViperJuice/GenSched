#pragma once
class DataReaderFactory
{
public:
	virtual ~DataReaderFactory()=0;
	virtual DataReader* create_dataReader()=0;
};

