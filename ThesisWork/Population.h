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

	void fillWithRandomPlacements(const Scheme& scheme)
	{
		for (size_t i = m_population.size(); i < m_populationSizeParam; ++i)
		{
			Chromosome placement(scheme);
			placement.generate_random_code(scheme);
			addChromosome(std::move(placement), Calc_Fitness(placement, scheme));
		}
	}

	void addChromosome(Chromosome&& chromosome, double fitness)
	{
		m_population.emplace_back(std::move(chromosome), fitness);
	}

	//filter out population, leave there the most adapted individuals, will return the number of filtered individuals
	size_t filterPopulation(double coeff)
	{
		assert(coeff >= 0.0 && coeff <= 1.0);
		size_t newPopulationSize = static_cast<size_t>(coeff * m_population.size());

		//
		sortPopulation();

		//remove all elements with the lower fitness
		for (size_t i = 0; i < m_populationSizeParam - newPopulationSize; i++)
			m_population.pop_back();

		assert(newPopulationSize == m_population.size());
		return missingIndivids();//m_populationSizeParam - newPopulationSize;
	}

	//
	void sortPopulation()
	{
		std::sort(m_population.begin(), m_population.end(), [](const auto& individ1, const auto& individ2) {
			return individ1.second < individ2.second; // sort by fitness function
		});
	}

	size_t missingIndivids()
	{
		return m_populationSizeParam - m_population.size();
	}

	std::pair<int, int> pickParentsPair(const std::vector<double>& parentsProbabilities) {
		// Randomly sample two indices based on the probabilities
		std::random_device rd;
		std::mt19937 gen(rd());
		std::discrete_distribution<> dist({ parentsProbabilities.begin(), parentsProbabilities.end()});
		int index1 = dist(gen);
		int index2 = dist(gen);

		// Return the pair of elements corresponding to the sampled indices
		return { index1, index2 };
	}

	std::vector<double> getParentsProbabilities()
	{
		double popFitness = calculatePopulationFitness();
		std::vector<double> parentsProbabilities(m_population.size(), 0.0);
		for (size_t i = 0; i < parentsProbabilities.size(); i++)
		{
			parentsProbabilities[i] = m_population[i].second / popFitness;
		}
		return parentsProbabilities;
	}

	//get Parents pairs 
	std::vector<std::pair<size_t, size_t>> getParentsPairs()
	{
		auto parentsProbabilities = getParentsProbabilities();
		std::vector<std::pair<size_t, size_t>> parentsPairs;
		parentsPairs.reserve(missingIndivids());
		for (size_t i = 0; i < missingIndivids(); i++)
		{
			parentsPairs.push_back(pickParentsPair(parentsProbabilities));
		}
		return parentsPairs;
	}

	//Filter population and refill it with random chromosomes
	void filterAndReFillPopulation(double coeff, const Scheme& scheme)
	{
		filterPopulation(coeff);
		fillWithRandomPlacements(scheme);
	}

	double calculatePopulationFitness()
	{
		double F = 0;
		for (size_t i = 0; i < m_population.size(); i++)
		{
			F += m_population[i].second;
		}
		return F / m_population.size();
	}

	static double Calc_Fitness(const Chromosome& chromosome, const Scheme& scheme)
	{
		const AdjacencyMatrixT& adjMatrix = scheme.getConnections();
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
};
