#pragma once

#include "Cell.h"
#include "Coord.h"
#include "InputDriver.h"

//multimap is for Fillers cells, because their id is -1 for each filler Cell
using ChromosomeType = std::vector<Coord>;


class Chromosome
{
public:
	//index is CellID, position is Coord
	using CodeType = std::vector<Coord>;
	using FillersType = std::vector<Coord>;
private:
	//Cell-Coord coding system
	CodeType m_code;
	//Filler Cells, if the Commutation Field is bigger we can fill it with fillers
	FillersType m_fillers;
public:

	Chromosome(const Scheme& scheme)
	{
		//init (cells-coordinate) code with invalid Coordinates
		m_code = CodeType(scheme.getCells().size(), Coord::invalidCoord());
	}

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

	const FillersType& getFillers() const
	{
		return m_fillers;
	}

	//only for optimization purpose
	void reservePlaceForFillers(size_t size)
	{
		m_fillers.reserve(size);
	}
	void addFiller(FillersType::value_type coord)
	{
		m_fillers.push_back(coord);
	}


	//random Chromosome generation, with code and fillers if they allowed
	void generate_random_code(const Scheme& scheme);

	//Construct and return corresponding commutation field for Chromosome
	MatrixT<CellID> getCorrespondingComutField(size_t row, size_t col) const;
	MatrixT<CellID> getCorrespondingComutField(const Scheme& scheme) const;

	//check the chromosome validity
	bool is_valid(const Scheme& scheme);

private:
	//The random placement algorithm
	void do_random_placement(const Scheme& scheme, CellsContainer& allCells);

};
