#pragma once

#include "Placement.h"
#include "Cell.h"
#include "ConnectivityMatrix.h"
#include "Population.h"



int_t calc_dist(Coord c1, Coord c2)
{
	return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
}

int_t calc_dist(const PlacementMap& placement, CellID cell1, CellID cell2)
{
	auto c1 = placement.find(cell1), c2 = placement.find(cell2);

	if (c1 == placement.end() || c2 == placement.end())
		throw std::exception("there is no such cell in placement");
	return calc_dist(c1->second, c2->second);
}

int_t calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2)
{
	assert(chromosome.size() > cell1 && chromosome.size() > cell2);

	return calc_dist(chromosome[cell1], chromosome[cell2]);
}


double Calc_Fitness(const PlacementMap& placement, const AdjacencyMatrixT& adjMatrix)
{
	double f = 0;
	const size_t cellsCount = adjMatrix.size();
	for (size_t i = 0; i < cellsCount; i++)
	{
		assert(cellsCount == adjMatrix[i].size());

		for (size_t j = 0; j < cellsCount; j++)
		{
			f += static_cast<double>(adjMatrix[i][j]) * calc_dist(placement, i, j);
		}
	}

	return f / placement.size();
}

double Calc_Fitness(const Chromosome& chromosome, const AdjacencyMatrixT& adjMatrix)
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

