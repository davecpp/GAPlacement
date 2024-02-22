#pragma once

#include "Chromosome.h"
#include "Cell.h"
#include "ConnectivityMatrix.h"
#include "Population.h"


class GA_Driver {

	struct GA_Params {
		size_t m_populationSize;
		size_t m_itersCount;
		// ...
	};

	PoipulationT m_population;

	static int_t calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2);

public:

	static double Calc_Fitness(const Chromosome& chromosome, const AdjacencyMatrixT& adjMatrix);

};
