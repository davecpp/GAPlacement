#pragma once

#include "Chromosome.h"
#include "Cell.h"
#include "ConnectivityMatrix.h"
#include "Population.h"


class GA_Driver {

	struct GA_Params {
		//size_t m_itersCount = 100;
		double filterPopulationCoeff = 0.4;
		//double m_crossoverProbability;
		double m_mutationProbability = 0.75;
		double epsilon = 0.001;
		// ...
	};

	Population m_population;
	GA_Params m_params;

	static int_t calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2);

	//get gene from chromosome(it will be first free coord from the index)
	static Chromosome::CodeType::value_type getGene(const Chromosome& chromosome, MatrixT<bool>& usedPlaces, size_t& index);

	// choose the ParentGene with given probability
	static bool probabilityChooser(double prob);// probability between 0.0 and 1.0

	//Mutate the individual with given probability for chromosome
	static void Mutate(Chromosome& chromosome, double chromosomeMutProbability, const Scheme& scheme);

public:

	GA_Driver(size_t populationSize) : m_population(populationSize) {}

	//crossover p1 and p2 parents, tale genes from p1 with probabilityOfP1 probability, return child individ
	//TODO - make function private
	static Chromosome Crossover(const Chromosome& p1, const Chromosome& p2, double probabilityOfP1, const Scheme& scheme);

	// run Genetic Algorithm and return the most adapted individual with lower fitness
	Chromosome run(const Scheme& scheme, size_t iterationsCount);
};
