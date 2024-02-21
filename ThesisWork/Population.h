#pragma once

#include "Placement.h"
#include "Cell.h"
#include "InputDriver.h"

#include <list>
#include <cassert>

using PoipulationT = std::list<Chromosome>;

inline PoipulationT generate_random_population(size_t populationSize, const Scheme& scheme)
{
	PoipulationT population;

	for (size_t i = 0; i < populationSize; ++i)
	{
		Chromosome placement = Chromosome::generate_random_code(scheme);
		population.push_back(placement);
	}
}


