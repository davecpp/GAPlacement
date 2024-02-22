#include <iostream>
#include <iomanip>
#include "Utils.h"
#include "Cell.h"
#include "Chromosome.h"
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



int main()
{
	Scheme scheme;
	scheme.setCells(generate_cells(100));
	scheme.setFieldParams(ComutFieldParams(11, 11));
	auto connections = generate_random_adjacency_matrix(scheme.getCells().size());
	scheme.setConnections(connections);

	Chromosome chromosome;
	chromosome.generate_random_code(scheme, true);

	printMatrix(chromosome.getCorrespondingComutField(scheme));

	std::cout << "Fitness = " << GA_Driver::Calc_Fitness(chromosome, connections);

}
