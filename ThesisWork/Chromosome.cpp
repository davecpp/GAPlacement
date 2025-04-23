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

	auto placeElement = [&allCells, this](size_t i, size_t j)
		{
			if (allCells.empty())
				return false;

			if (allCells.back().is_normalCell())
			{
				//the Cell coordinate must be invalid(-1,-1)
				assert(this->operator[](allCells.back().getID()) == Coord::invalidCoord());
				this->operator[](allCells.back().getID()) = Coord(i, j);
			}
			else if (allCells.back().is_filler())
				addFiller(Coord(i, j));

			allCells.pop_back();
			return true;
		};

#define SQUARETRAVERSINGALGORITHM
#if defined(SQUARETRAVERSINGALGORITHM)
	for (size_t level = 0; level < std::max(scheme.getFieldRows(), scheme.getFieldCols()); ++level)
	{
		if (level < scheme.getFieldRows())
			for (size_t j = 0; j < std::min(level + 1, scheme.getFieldCols()); j++)
				if (!placeElement(level, j))
					return;

		if (level < scheme.getFieldCols())
			for (size_t i = 0; i < std::min(level + 1, scheme.getFieldRows()); i++)
				if (i != (level) && !placeElement(i, level))
					return;
	}
#else
	for (size_t i = 0; i < scheme.getFieldRows(); ++i)
		for (size_t j = 0; j < scheme.getFieldCols(); ++j)
			placeElement(i, j);
#endif

	assert(allCells.empty());
}


void Chromosome::generate_random_code(const Scheme& scheme)
{
	//check the scheme validity
	assert(scheme.is_valid());

	//get Cells with Fillers if they allowed
	CellsContainer allCells = scheme.is_fillersAllowed() ? scheme.getCellsWithFillers() : scheme.getCells();

	//just reserve the place, for optimization purpose
	size_t fillersCount = allCells.size() - scheme.getCells().size();
	reservePlaceForFillers(fillersCount);

	//place the all Cells(with fillers) into the Commutation Field 
	do_random_placement(scheme, allCells);

	//just to be sure, everything worked correct
	assert(fillersCount == m_fillers.size());
	assert(this->is_valid(scheme));
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


MatrixT<Cell> Chromosome::getCorrespondingComutField(const Scheme& scheme) const
{

	auto row = scheme.getFieldRows();
	auto col = scheme.getFieldCols();

	auto res = makeMatrix<MatrixT<Cell>>(row, col, Cell{ Cell::invalidID });
	const auto& cells = scheme.getCells();

	for (size_t cellID = 0; cellID < this->size(); cellID++)
	{
		Coord c = this->operator[](cellID);
		assert(c.isValidCoord());
		// check that every Cell in CellsContainer is in the same place with his cellID
		assert(cellID == cells[cellID].getID());

		res[c.x()][c.y()] = cells[cellID];
	}

	const auto& fillers = this->getFillers();
	for (size_t i = 0; i < fillers.size(); i++)
	{
		Coord c = fillers[i];
		assert(c.isValidCoord());
		res[c.x()][c.y()] = Cell{ Cell::fillerID };
	}

	return res;
}


bool Chromosome::is_valid(const Scheme& scheme)
{
	assert(scheme.is_valid());
	if (scheme.is_fillersAllowed() &&
		(scheme.getCells().size() != scheme.getFieldSize() - m_fillers.size()))
		return false;

	//there is no normal Cell(gene) with invalid Coord
	for (size_t i = 0; i < this->size(); i++)
	{
		if (!this->operator[](i).isValidCoord())
			return false;
	}

	//there is no filler Cell with invalid coord
	for (size_t i = 0; i < m_fillers.size(); i++)
	{
		if (!m_fillers[i].isValidCoord())
			return false;
	}

	return true;

}
