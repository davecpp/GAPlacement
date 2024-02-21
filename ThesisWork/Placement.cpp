#include "Placement.h"
#include "InputDriver.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <random>


PlacementMap generate_random_placement(const Scheme& scheme, bool fillersAllowed)
{
	assert(scheme.validate());
	CellsContainer cells = fillersAllowed ? scheme.getCellsWithFillers() : scheme.getCells();

	size_t rows = scheme.getRows();
	size_t cols = scheme.getCols();

	PlacementMap placement;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(cells.begin(), cells.end(), g);

	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			if (cells.empty())
				break;

			placement.emplace(cells.back().m_id, Coord(i, j));

			cells.pop_back();
		}
	}

	assert(cells.empty());

	return placement;
}


Chromosome Chromosome::generate_random_code(const Scheme& scheme, bool fillersAllowed)
{
	Chromosome chromosome;

	auto placement = generate_random_placement(scheme, fillersAllowed);
	size_t fillersCount = placement.count(Cell::fillerID);
	if (fillersAllowed)
		assert(scheme.getCells().size() == scheme.getFieldSize() - fillersCount);

	chromosome.m_code = CodeType(scheme.getCells().size(), Coord(-1, -1));
	chromosome.m_fillers.reserve(fillersCount);

	for (auto it = placement.begin(); it != placement.end(); ++it)
	{
		if (!it->first.is_filler())
			chromosome.m_code[it->first.getID()] = it->second;
		else // filler cell
			chromosome.m_fillers.push_back(it->second);
	}

	return chromosome;
}



