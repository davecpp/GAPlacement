#pragma once

#include "Chromosome.h"
#include "Cell.h"
#include "InputDriver.h"

//#include <list>
#include <cassert>

struct IndividualType
{
	Chromosome chromosome;
	double fitness;
	IndividualType(Chromosome&& i_chromosome, double i_fitness) :chromosome(std::move(i_chromosome)), fitness(i_fitness) {}
};
using PopulationT = std::vector<IndividualType>;



class Population
{
	PopulationT m_population;
	//the size which population will be feet
	size_t m_populationCapacity = 0;
	double popFitness = 0;


public:
	Population(size_t popSizeParam) :m_populationCapacity(popSizeParam) { m_population.reserve(m_populationCapacity); };

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
		return m_populationCapacity;
	}
	size_t missingIndividsCount() const
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

	//get Parents pairs 
	std::vector<std::pair<size_t, size_t>> getParentsPairs() const;


	//the sum of all individuals fitness
	double CalcFitnessSum() const;
	//the average fitness function for population - SumOfFitness/size
	double CalculatePopulationFitness() const;

	static double Calc_Fitness(const Chromosome& chromosome, const Scheme& scheme);

private:
	std::pair<int, int> pickParentsPair(const std::vector<double>& parentsProbabilities) const;

	std::vector<double> getProbabilitiesOfBeingParents() const;

	//Filter population and refill it with random chromosomes
	void filterAndReFillPopulation(double filtrationCoeff, const Scheme& scheme)
	{
		filterPopulation(filtrationCoeff);
		fillWithRandomPlacements(scheme);
	}

	static int_t calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2)
	{
		assert(chromosome.size() > cell1 && chromosome.size() > cell2);

		return Coord::distance(chromosome[cell1], chromosome[cell2]);
	}

};
