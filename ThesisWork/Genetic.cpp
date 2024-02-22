#include "Genetic.h"

int_t GA_Driver::calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2)
{
	assert(chromosome.size() > cell1 && chromosome.size() > cell2);

	return Coord::distance(chromosome[cell1], chromosome[cell2]);
}

double GA_Driver::Calc_Fitness(const Chromosome& chromosome, const AdjacencyMatrixT& adjMatrix)
{
	double f = 0;
	const size_t cellsCount = chromosome.size();
	for (size_t i = 0; i < cellsCount; i++)
	{
		assert(cellsCount == adjMatrix[i].size());

		for (size_t j = 0; j < cellsCount; j++)
		{
			f += static_cast<double>(adjMatrix[i][j]) * calc_dist(chromosome, i, j);
		}
	}
	return f / chromosome.size();
}
