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
	scheme.fillersAllowed(true);
	scheme.setCells(generate_cells(25));
	scheme.setFieldParams(ComutFieldParams(15, 15));
	auto connections = generate_random_adjacency_matrix(scheme.getCells().size());
	scheme.setConnections(connections);

	Chromosome parent1(scheme);

	parent1.generate_random_code(scheme);

	//printMatrix(parent1.getCorrespondingComutField(scheme));
	//return 1;

	Chromosome parent2(scheme);
	parent2.generate_random_code(scheme);

	//printMatrix(parent2.getCorrespondingComutField(scheme));

	Chromosome child = GA_Driver::Crossover(parent1, parent2, 0.5, scheme);

	//printMatrix(child.getCorrespondingComutField(scheme));

	std::cout << "Fitness1 = " << Population::Calc_Fitness(parent1, scheme) << std::endl;
	std::cout << "Fitness2 = " << Population::Calc_Fitness(parent2, scheme) << std::endl;
	std::cout << "Child Fitness = " << Population::Calc_Fitness(child, scheme) << std::endl;

	GA_Driver driver(25);
	auto BestIndivid = driver.run(scheme);
	//std::cout << "Best possible Fitness = " << scheme.BestFitness() << std::endl;

	//printMatrix(connections);

	printMatrix(BestIndivid.getCorrespondingComutField(scheme));

}
