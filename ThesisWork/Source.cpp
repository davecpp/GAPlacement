#include <iostream>
#include <iomanip>
#include <fstream>


#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "Utils.h"
#include "Cell.h"
#include "Chromosome.h"
#include "Genetic.h"
#include "InputDriver.h"
//#include "SFML_printer.h"
#include "toGeoJSON.h"


void printMatrix(const MatrixT<Cell>& matrix)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j].getID() == -1)
				std::cout << std::right << std::setw(4) << " ";
			else
				std::cout << std::right << std::setw(4) << matrix[i][j].getID();
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n";
}


void save(const std::vector<std::vector<size_t>>& data, const std::string& filename) {
	// Open an output file stream
	std::ofstream ofs(filename);
	if (!ofs) {
		std::cerr << "Could not open file for save." << std::endl;
		return;
	}

	// Create a text archive and serialize the data
	boost::archive::text_oarchive oa(ofs);
	oa << data;
}

void load(std::vector<std::vector<size_t>>& data, const std::string& filename) {
	// Open an input file stream
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cerr << "Could not open file for load." << std::endl;
		return;
	}

	// Create a text archive and deserialize the data
	boost::archive::text_iarchive ia(ifs);
	ia >> data;
}



int main(int argc, char* argv[])
{
	Scheme scheme;
	scheme.fillersAllowed(true);
	scheme.setCells(generate_cells(100));
	scheme.setFieldParams(ComutFieldParams(30, 30));
	auto connections = generate_random_adjacency_matrix(scheme.getCells().size());
	scheme.setConnections(connections);



	//save(connections, "example1.txt");
	//load(connections, "example1.txt");

	//Chromosome parent1(scheme);

	//parent1.generate_random_code(scheme);

	////printMatrix(parent1.getCorrespondingComutField(scheme));
	////return 1;

	//Chromosome parent2(scheme);
	//parent2.generate_random_code(scheme);

	////printMatrix(parent2.getCorrespondingComutField(scheme));

	//Chromosome child = GA_Driver::Crossover(parent1, parent2, 0.5, scheme);

	////printMatrix(child.getCorrespondingComutField(scheme));

	//std::cout << "Fitness1 = " << Population::Calc_Fitness(parent1, scheme) << std::endl;
	//std::cout << "Fitness2 = " << Population::Calc_Fitness(parent2, scheme) << std::endl;
	//std::cout << "Child Fitness = " << Population::Calc_Fitness(child, scheme) << std::endl;

	GA_Driver driver(25);
	auto BestIndivid = driver.run(scheme, 1);
	{
		auto str = cells_to_geojson(BestIndivid.getCorrespondingComutField(scheme));
		std::ofstream("cells_initial.geojson") << str;
	}
	//printSFMLMatrix(BestIndivid.getCorrespondingComutField(scheme), scheme);

	//std::cout << "Best possible Fitness = " << scheme.BestFitness() << std::endl;
	//printMatrix(connections);


	BestIndivid = driver.run(scheme, 100);
	printMatrix(BestIndivid.getCorrespondingComutField(scheme));


	auto str = cells_to_geojson(BestIndivid.getCorrespondingComutField(scheme));
	std::ofstream("cells.geojson") << str;

}




