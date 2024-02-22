#pragma once

#include "Chromosome.h"
#include "Cell.h"
#include "ConnectivityMatrix.h"
#include "Population.h"


class GA_Driver {

	struct GA_Params {
		size_t m_itersCount = 1000;
		double filterPopulationCoeff = 0.5;
		double m_crossoverProbability;
		double m_mutationProbability;
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

public:

	GA_Driver(size_t populationSize) : m_population(populationSize) {}

	static Chromosome Crossover(const Chromosome& p1, const Chromosome& p2, double probabilityOfP1, const Scheme& scheme);

	void run(const Scheme& scheme);
};
