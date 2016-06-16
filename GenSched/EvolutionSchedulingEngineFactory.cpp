#include "pch.h"
#include "EvolutionSchedulingEngineFactory.h"


EvolutionSchedulingEngineFactory::EvolutionSchedulingEngineFactory()
{

}

EvolutionSchedulingEngineFactory::~EvolutionSchedulingEngineFactory()
{
	throw ref new Platform::NotImplementedException();
}

EvolutionSchedulingEngine* EvolutionSchedulingEngineFactory::create_schedulingEngine()
{
	return new EvolutionSchedulingEngine();
}
