#include "Genetic.h"

#include <random>
#include <iostream>

int_t GA_Driver::calc_dist(const Chromosome& chromosome, CellID cell1, CellID cell2)
{
	assert(chromosome.size() > cell1 && chromosome.size() > cell2);

	return Coord::distance(chromosome[cell1], chromosome[cell2]);
}


Chromosome::CodeType::value_type GA_Driver::getGene(const Chromosome& chromosome, MatrixT<bool>& usedPlaces, size_t& index)
{
	assert(index <= chromosome.size());
	for (; index < chromosome.size(); ++index)
	{
		const Coord coord = chromosome[index];
		if (usedPlaces[coord.x()][coord.y()])
			continue;

		usedPlaces[coord.x()][coord.y()] = true;
		return chromosome[index++];

	}
	//there is no genes left
	assert(false);
	return Coord::invalidCoord();
}

bool GA_Driver::probabilityChooser(double prob)
{
	assert(prob >= 0.0 && prob <= 1.0);
	static std::knuth_b rand_engine;
	static std::uniform_real_distribution<> uniform_zero_to_one(0.0, 1.0);

	return uniform_zero_to_one(rand_engine) >= prob;
}

Chromosome GA_Driver::Crossover(const Chromosome& parent1, const Chromosome& parent2, double probabilityOfP1, const Scheme& scheme)
{
	assert(parent1.size() == parent2.size());
	assert(parent1.getFillers().size() == parent2.getFillers().size());

	Chromosome child(scheme);
	auto usedPlaces = makeMatrix <MatrixT<bool>>(scheme.getFieldRows(), scheme.getFieldCols(), false);

	const size_t csize = parent1.size();
	size_t index1 = 0, index2 = 0;

	//for each gene
	for (size_t i = 0; i < csize; ++i)
	{
		if (probabilityChooser(probabilityOfP1)) // from parent1
			child[i] = getGene(parent1, usedPlaces, index1);
		else // from parent2
			child[i] = getGene(parent2, usedPlaces, index2);
	}

	//if fillers allowed, fill them to all free spaces
	if (scheme.is_fillersAllowed())
	{
		child.reservePlaceForFillers(parent1.getFillers().size());

		for (size_t i = 0; i < scheme.getFieldRows(); i++)
		{
			for (size_t j = 0; j < scheme.getFieldCols(); j++)
			{
				if (!usedPlaces[i][j])
					child.addFiller(Coord(i, j));
			}
		}
	}

	return std::move(child);

}

void GA_Driver::Mutate(Chromosome& chromosome, double chromosomeMutProbability)
{
	double geneMutationProbability = chromosomeMutProbability / chromosome.size();
	for (size_t i = 0; i < chromosome.size(); i++)
	{
		if (probabilityChooser(geneMutationProbability))
		{
			auto swapIndex = std::rand() % chromosome.size();
			std::swap(chromosome[i], chromosome[swapIndex]);
		}
	}
}

void GA_Driver::run(const Scheme& scheme)
{
	double prevFitness = 0.0;
	for (size_t i = 0; i < m_params.m_itersCount; i++)
	{
		m_population.fillWithRandomPlacements(scheme);
		auto filteredsCount = m_population.filterPopulation(m_params.filterPopulationCoeff);

		auto parentsPairs = m_population.getParentsPairs();
		assert(parentsPairs.size() == filteredsCount);

		for (auto it : parentsPairs)
		{
			double parent1Fitness = m_population[it.first].second;
			double parent2Fitness = m_population[it.second].second;

			double parent1Probability = parent1Fitness / (parent1Fitness + parent2Fitness);
			Chromosome child = Crossover(m_population[it.first].first, m_population[it.second].first, parent1Probability, scheme);
			Mutate(child, m_params.m_mutationProbability);
			double f = Population::Calc_Fitness(child, scheme);
			m_population.addChromosome(std::move(child), f);
		}

		double F = m_population.calculatePopulationFitness();
		if (100 * std::abs(prevFitness - F) / F < m_params.epsilon)
			break;

		prevFitness = F;
		std::cout << "Fitness = " << F << "\t";
		m_population.sortPopulation();
		std::cout << "Best individ = " << m_population[0].second << "\titeration = " << i << std::endl;
	}
}
