#include "Placement.h"
#include "InputDriver.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <random>


void Chromosome::do_random_placement(const Scheme& scheme, CellsContainer& allCells)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(allCells.begin(), allCells.end(), g);

	for (size_t i = 0; i < scheme.getRows(); ++i)
	{
		for (size_t j = 0; j < scheme.getCols(); ++j)
		{
			if (allCells.empty())
				return;
			if (allCells.back().is_normalCell())
			{
				//the Cell coordinate must be invalid(-1,-1)
				assert(this->operator[](allCells.back().getID()) == Coord::invalidCoord());
				this->operator[](allCells.back().getID()) = Coord(i, j);
			}
			else if (allCells.back().is_filler())
				this->m_fillers.push_back(Coord(i, j));

			allCells.pop_back();
		}
	}

	assert(allCells.empty());
}


void Chromosome::generate_random_code(const Scheme& scheme, bool fillersAllowed)
{
	//check the scheme validity
	assert(scheme.is_valid());

	//init (cells-coordinate) code with invalid Coordinates
	m_code = CodeType(scheme.getCells().size(), Coord::invalidCoord());

	//get Cells with Fillers if they allowed
	CellsContainer allCells = fillersAllowed ? scheme.getCellsWithFillers() : scheme.getCells();
	
	//just reserve the place, for optimization purpose
	size_t fillersCount = allCells.size() - scheme.getCells().size();
	m_fillers.reserve(fillersCount);

	//place the all Cells(with fillers) into the Commutation Field 
	do_random_placement(scheme, allCells);

	//just to be sure, everything worked correct
	assert(fillersCount == m_fillers.size());
	if (fillersAllowed)
		assert(scheme.getCells().size() == scheme.getFieldSize() - m_fillers.size());
}



