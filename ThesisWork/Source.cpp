#include <iostream>
#include <iomanip>
#include "Utils.h"
#include "Cell.h"
#include "Placement.h"
#include "Genetic.h"
#include "InputDriver.h"

template<typename T>
void printMatrix(const MatrixT<T>& matrix)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			std::cout << std::right << std::setw(4) << matrix[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n";
}



MatrixT<CellID> chromosomeToComutField(const Chromosome& chromosome, size_t row, size_t col)
{
	auto res = makeMatrix<MatrixT<CellID>>(row, col, Cell::invalidID);

	for (size_t cellID = 0; cellID < chromosome.size(); cellID++)
	{
		Coord t = chromosome[cellID];
		if (t)
			res[t.x][t.y] = cellID;
	}

	const auto& fillers = chromosome.getFillers();
	for (size_t i = 0; i < fillers.size(); i++)
	{
		Coord t = fillers[i];
		if (t)
			res[t.x][t.y] = Cell::fillerID;
	}

	return res;
}




int main()
{
	Scheme scheme;
	scheme.setCells(generate_cells(100));
	scheme.setFieldParams(ComutFieldParams(11, 11));
	auto connections = generate_random_adjacency_matrix(scheme.getCells().size());
	scheme.setConnections(connections);

	Chromosome chromosome;
	chromosome.generate_random_code(scheme, true);

	printMatrix(chromosomeToComutField(chromosome, scheme.getRows(), scheme.getCols()));

	std::cout << "Fitness = " << Calc_Fitness(chromosome, connections);

}
