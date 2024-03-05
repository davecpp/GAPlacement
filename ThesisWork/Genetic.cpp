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
	// Create a random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Generate a random number between 0 and 1
	std::uniform_real_distribution<> dis(0.0, 1.0);
	double random_value = dis(gen);

	// Return true if the random number is less than the probability
	return random_value < prob;
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

	assert(child.is_valid(scheme));
	return child;

}

void GA_Driver::Mutate(Chromosome& chromosome, double chromosomeMutProbability, const Scheme& scheme)
{
	double geneMutationProbability = chromosomeMutProbability / chromosome.size();
	for (size_t i = 0; i < chromosome.size(); i++)
	{
		if (probabilityChooser(geneMutationProbability))
		{
			if (scheme.is_fillersAllowed())
			{
				assert(chromosome.size() + chromosome.getFillers().size() == scheme.getFieldSize());
				if (probabilityChooser(static_cast<double>(chromosome.getFillers().size()) / scheme.getFieldSize())) //swap with filler
				{
					auto fillerSwapIndex = std::rand() % chromosome.getFillers().size();
					static_assert(std::is_same_v<Chromosome::CodeType::value_type, Chromosome::FillersType::value_type>,
						"Chromosome Code and Fillers must have the same value type");
					std::swap(chromosome[i], chromosome.getFillers()[fillerSwapIndex]);
					continue;
				}
			}

			auto swapIndex = std::rand() % chromosome.size();
			std::swap(chromosome[i], chromosome[swapIndex]);
		}
	}
}

Chromosome GA_Driver::run(const Scheme& scheme)
{
	double prevFitness = 0.0;
	for (size_t i = 0; i < m_params.m_itersCount; i++)
	{
		m_population.fillWithRandomPlacements(scheme);
		auto filteredsCount = m_population.filterPopulation(m_params.filterPopulationCoeff);

		auto parentsPairs = m_population.getParentsPairs();
		assert(parentsPairs.size() == filteredsCount);

		for (auto [parent1, parent2] : parentsPairs)
		{
			double parent1Fitness = m_population[parent1].fitness;
			double parent2Fitness = m_population[parent2].fitness;

			//lower fitness is must be chosen with bigger probability
			double parent1Probability = 1 - (parent1Fitness / (parent1Fitness + parent2Fitness));

			Chromosome child = Crossover(m_population[parent1].chromosome, m_population[parent2].chromosome, parent1Probability, scheme);
			Mutate(child, m_params.m_mutationProbability, scheme);
			double f = Population::Calc_Fitness(child, scheme);
			m_population.addChromosome(std::move(child), f);
		}

		double F = m_population.CalculatePopulationFitness();
		if (100 * std::abs(prevFitness - F) / F < m_params.epsilon)
			break;

		prevFitness = F;
		std::cout << "Fitness = " << std::fixed << F << "\t";
		m_population.sortPopulation();
		std::cout << "Best individ = " << m_population[0].fitness << "\titeration = " << i << std::endl;
	}
	return m_population[0].chromosome;
}
