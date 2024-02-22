#pragma once

#include "Cell.h"
#include "Coord.h"
#include "InputDriver.h"

//multimap is for Fillers cells, because their id is -1 for each filler Cell
using ChromosomeType = std::vector<Coord>;


class Chromosome
{
	//index is CellID, position is Coord
	using CodeType = std::vector<Coord>;
	//Cell-Coord coding system
	CodeType m_code;
	//Filler Cells, if the Commutation Field is bigger we can fill it with fillers
	std::vector<Coord> m_fillers;
public:

	//get Coordinate of corresponding cell
	Coord& operator[](size_t i)
	{
		return m_code[i];
	}
	const Coord& operator[](size_t i) const
	{
		return m_code[i];
	}

	size_t size() const
	{
		return m_code.size();
	}

	const std::vector<Coord>& getFillers() const
	{
		return m_fillers;
	}


	//random Chromosome generation, with code and fillers if they allowed
	void generate_random_code(const Scheme& scheme, bool fillersAllowed = true);

	//Construct and return corresponding commutation field for Chromosome
	MatrixT<CellID> getCorrespondingComutField(size_t row, size_t col) const;
	MatrixT<CellID> getCorrespondingComutField(const Scheme& scheme) const;
private:
	//The random placement algorithm
	void do_random_placement(const Scheme& scheme, CellsContainer& allCells);

};
