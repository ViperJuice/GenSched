#include "pch.h"
#include "EvolutionSchedulingEngine.h"

using namespace std;
using namespace concurrency;

EvolutionSchedulingEngine::EvolutionSchedulingEngine()
{

}


size_t* EvolutionSchedulingEngine::buildScheduleShell(AvailabilityData)
{
	return nullptr;
}

void EvolutionSchedulingEngine::fillScheduleShell(AvailabilityData, size_t *)
{
	throw ref new Platform::NotImplementedException();
}
