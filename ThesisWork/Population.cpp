#include "Population.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

void Population::fillWithRandomPlacements(const Scheme& scheme)
{
	for (size_t i = m_population.size(); i < m_populationCapacity; ++i)
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
	for (size_t i = 0; i < m_populationCapacity - newPopulationSize; i++)
		m_population.pop_back();

	assert(newPopulationSize == m_population.size());
	return missingIndividsCount();//m_populationSizeParam - newPopulationSize;
}

//
void Population::sortPopulation()
{
	std::sort(m_population.begin(), m_population.end(), [](const auto& individ1, const auto& individ2) {
		return individ1.fitness < individ2.fitness; // sort by fitness function
		});
}


std::pair<int, int> Population::pickParentsPair(const std::vector<double>& parentsProbabilities)  const
{
	assert(parentsProbabilities.size() > 1);
	// Randomly sample two indices based on the probabilities
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist({ parentsProbabilities.begin(), parentsProbabilities.end() });
	int index1 = dist(gen);
	int index2 = dist(gen);
	// Ensure index1 and index2 are distinct
	while (index2 == index1) {
		index2 = dist(gen);
	}

	// Return the pair of elements corresponding to the sampled indices
	return { index1, index2 };
}

std::vector<double> Population::getProbabilitiesOfBeingParents() const
{
	//The sum of all individuals fitness
	double popFitnessSum = CalcFitnessSum();

	std::vector<double> parentsProbabilities(m_population.size(), 0.0);
	for (size_t i = 0; i < parentsProbabilities.size(); i++)
	{
		parentsProbabilities[i] = //1.0 / m_population.size();
			(popFitnessSum - m_population[i].fitness) / (popFitnessSum * (m_population.size() - 1));
	}
	return parentsProbabilities;
}

//get Parents pairs 
std::vector<std::pair<size_t, size_t>> Population::getParentsPairs() const
{
	auto parentsProbabilities = getProbabilitiesOfBeingParents();
	std::vector<std::pair<size_t, size_t>> parentsPairs;
	parentsPairs.reserve(missingIndividsCount());

	for (size_t i = 0; i < missingIndividsCount(); i++)
	{
		parentsPairs.push_back(pickParentsPair(parentsProbabilities));
	}
	return parentsPairs;
}

double Population::CalcFitnessSum() const
{
	double F = 0.0;
	for (const auto it : m_population)
		F += it.fitness;
	return F;
}

double Population::CalculatePopulationFitness() const
{
	return CalcFitnessSum() / m_population.size();
}



// Define Boost types.
namespace bg = boost::geometry;
using BoostPoint = bg::model::d2::point_xy<int>;
using BoostPolygon = bg::model::polygon<BoostPoint>;

// Convert a vector of Coord to a Boost.Geometry polygon.
BoostPolygon toBoostPolygon(const PolygonT& pts) {
	BoostPolygon poly;
	for (const auto& p : pts) {
		bg::append(poly.outer(), BoostPoint(p.first, p.second));
	}
	bg::correct(poly); // Closes polygon and ensures correct ordering.
	return poly;
}

double Population::Calc_Fitness(const Chromosome& chromosome, const Scheme& scheme)
{
	const auto& cells = scheme.getCells();


	const AdjacencyMatrixT& adjMatrix = scheme.getConnections();
	double f = 0;
	const size_t cellsCount = chromosome.size();
	for (size_t i = 0; i < cellsCount; i++)
	{
		assert(cellsCount == adjMatrix[i].size());

		for (size_t j = 0; j < cellsCount; j++)
		{
			f += static_cast<double>(adjMatrix[i][j]) * calc_dist(chromosome, i, j);

			//auto poly1 = toBoostPolygon(cells[i].getPolygon(j, i));
			//auto poly2 = toBoostPolygon(cells[j].getPolygon(j, i));
			//// Compute the intersection and store results in a vector.
			//std::vector<BoostPolygon> output;
			//bg::intersection(poly1, poly2, output);

			//if (!output.empty()) {
			//	double area = bg::area(output[0]);
			//	f += area;
			//}

		}
	}
	return f / chromosome.size();
}
