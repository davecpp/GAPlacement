#pragma once
#include "Cell.h"
#include "InputDriver.h"

#include <map>

//multimap is for Fillers cells, because their id is -1 for each filler Cell
using PlacementMap = std::multimap<Cell, Coord>;
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

	Coord& operator[](size_t i)
	{
		return m_code[i];
	}

	const Coord& operator[](size_t i) const
	{
		return m_code[i];
	}

	const std::vector<Coord>& getFillers() const
	{
		return m_fillers;
	}

	size_t size() const
	{
		return m_code.size();
	}


	//random Chromosome generation, with code and fillers if they allowed
	void generate_random_code(const Scheme& scheme, bool fillersAllowed = true);

private:
	//The random placement algorithm
	void do_random_placement(const Scheme& scheme, CellsContainer& allCells);

};
