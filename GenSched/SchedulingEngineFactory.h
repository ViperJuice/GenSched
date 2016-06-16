#pragma once
class SchedulingEngineFactory
{
public:
	virtual ~SchedulingEngineFactory()=0;
	virtual SchedulingEngine* create_schedulingEngine()=0;
};

