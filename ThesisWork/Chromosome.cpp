#include "Chromosome.h"
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

	for (size_t i = 0; i < scheme.getFieldRows(); ++i)
	{
		for (size_t j = 0; j < scheme.getFieldCols(); ++j)
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


MatrixT<CellID> Chromosome::getCorrespondingComutField(size_t row, size_t col) const
{
	auto res = makeMatrix<MatrixT<CellID>>(row, col, Cell::invalidID);

	for (size_t cellID = 0; cellID < this->size(); cellID++)
	{
		Coord c = this->operator[](cellID);
		assert(c.isValidCoord());
		res[c.x()][c.y()] = cellID;
	}

	const auto& fillers = this->getFillers();
	for (size_t i = 0; i < fillers.size(); i++)
	{
		Coord c = fillers[i];
		assert(c.isValidCoord());
		res[c.x()][c.y()] = Cell::fillerID;
	}

	return res;
}

MatrixT<CellID> Chromosome::getCorrespondingComutField(const Scheme& scheme) const
{
	return getCorrespondingComutField(scheme.getFieldRows(), scheme.getFieldCols());
}
