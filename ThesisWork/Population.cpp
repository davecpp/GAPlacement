#include "Population.h"


void Population::fillWithRandomPlacements(const Scheme& scheme)
{
	for (size_t i = m_population.size(); i < m_populationSizeParam; ++i)
	{
		Chromosome placement(scheme);
		placement.generate_random_code(scheme);
		addChromosome(std::move(placement), Calc_Fitness(placement, scheme));
	}
}


//filter out population, leave there the most adapted individuals, will return the number of filtered individuals
size_t Population::filterPopulation(double filtrationCoeff)
{
	assert(filtrationCoeff >= 0.0 && filtrationCoeff <= 1.0);
	size_t newPopulationSize = static_cast<size_t>((1 - filtrationCoeff) * m_population.size());

	//sort population for filtering unadapted individuals
	sortPopulation();

	//remove all elements with the lower fitness
	for (size_t i = 0; i < m_populationSizeParam - newPopulationSize; i++)
		m_population.pop_back();

	assert(newPopulationSize == m_population.size());
	return missingIndivids();//m_populationSizeParam - newPopulationSize;
}

//
void Population::sortPopulation()
{
	std::sort(m_population.begin(), m_population.end(), [](const auto& individ1, const auto& individ2) {
		return individ1.second < individ2.second; // sort by fitness function
	});
}


std::pair<int, int> Population::pickParentsPair(const std::vector<double>& parentsProbabilities) {
	// Randomly sample two indices based on the probabilities
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist({ parentsProbabilities.begin(), parentsProbabilities.end() });
	int index1 = dist(gen);
	int index2 = dist(gen);

	// Return the pair of elements corresponding to the sampled indices
	return { index1, index2 };
}

std::vector<double> Population::getParentsProbabilities()
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
std::vector<std::pair<size_t, size_t>> Population::getParentsPairs()
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

double Population::calculatePopulationFitness()
{
	double F = 0;
	for (size_t i = 0; i < m_population.size(); i++)
	{
		F += m_population[i].second;
	}
	return F / m_population.size();
}

double Population::Calc_Fitness(const Chromosome& chromosome, const Scheme& scheme)
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
