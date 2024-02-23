#pragma once

#include "Chromosome.h"
#include "Cell.h"
#include "InputDriver.h"

//#include <list>
#include <cassert>

using IndividualType = std::pair<Chromosome, double /*fitness*/>;
using PopulationT = std::vector<IndividualType>;

inline PopulationT generate_random_population(size_t populationSize, const Scheme& scheme)
{
	PopulationT population;

	for (size_t i = 0; i < populationSize; ++i)
	{
		Chromosome placement(scheme);
		placement.generate_random_code(scheme);
		population.push_back(std::make_pair(placement, -1.));
	}
}


class Population
{
	PopulationT m_population;
	//the size which population will be feet
	size_t m_populationSizeParam = 0;
	double popFitness = 0;

	static int_t calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2)
	{
		assert(chromosome.size() > cell1 && chromosome.size() > cell2);

		return Coord::distance(chromosome[cell1], chromosome[cell2]);
	}




public:
	Population(size_t popSizeParam) :m_populationSizeParam(popSizeParam) { m_population.reserve(m_populationSizeParam); };

	const IndividualType& operator[](size_t i)
	{
		return m_population[i];
	}

	size_t size() const
	{
		return m_population.size();
	}
	size_t capacity() const
	{
		return m_populationSizeParam;
	}
	size_t missingIndivids()
	{
		return this->capacity() - this->size();
	}


	void addChromosome(Chromosome&& chromosome, double fitness)
	{
		m_population.emplace_back(std::move(chromosome), fitness);
	}

	void fillWithRandomPlacements(const Scheme& scheme);

	//filter out population, leave there the most adapted individuals, will return the number of filtered individuals
	size_t filterPopulation(double filtrationCoeff);

	//sort population individuals according to their fitness functions
	void sortPopulation();


	std::pair<int, int> pickParentsPair(const std::vector<double>& parentsProbabilities);

	std::vector<double> getParentsProbabilities();

	//get Parents pairs 
	std::vector<std::pair<size_t, size_t>> getParentsPairs();

	//Filter population and refill it with random chromosomes
	void filterAndReFillPopulation(double filtrationCoeff, const Scheme& scheme)
	{
		filterPopulation(filtrationCoeff);
		fillWithRandomPlacements(scheme);
	}

	double calculatePopulationFitness();

	static double Calc_Fitness(const Chromosome& chromosome, const Scheme& scheme);
};
