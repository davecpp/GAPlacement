#pragma once
#include "Cell.h"
#include "InputDriver.h"

#include <map>

//multimap is for Fillers cells, because their id is -1 for each filler Cell
using PlacementMap = std::multimap<Cell, Coord>;
using ChromosomeType = std::vector<Coord>;


PlacementMap generate_random_placement(const Scheme& scheme, bool fillersAllowed);



class Chromosome
{
	//index is CellID, position is Coord
	using CodeType = std::vector<Coord>;

	CodeType m_code;
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

	const std::vector<Coord> getFillers()
	{
		return m_fillers;
	}

	size_t size() const
	{
		return m_code.size();
	}

	static Chromosome generate_random_code(const Scheme& scheme, bool fillersAllowed = true);
};
