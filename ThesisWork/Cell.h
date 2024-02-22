#pragma once
#include "Utils.h"

#include <vector>
#include <string>




using CellID = int_t;

struct Cell
{
	static constexpr CellID fillerID = -1;
	static constexpr CellID invalidID = -2;

	Cell() = default;
	Cell(CellID i_id) : m_id(i_id) {};
	Cell(std::string i_name, CellID i_id) : m_name(i_name), m_id(i_id) {}

	std::string m_name;
	CellID m_id = invalidID;

	bool operator<(const Cell& other) const
	{
		return this->m_id < other.m_id;
	}

	bool is_filler() const
	{
		return m_id == fillerID;
	}

	bool is_normalCell() const
	{
		return m_id >= 0;
	}

	const CellID getID() const
	{
		return m_id;
	}
	//int_t w = 1;
	//int_t h = 1;

	//operator CellID()
	//{
	//	return m_id;
	//}
};

using CellsContainer = std::vector<Cell>;

inline CellsContainer generate_cells(size_t cellsCount)
{
	CellsContainer cells(cellsCount);
	for (size_t i = 0; i < cellsCount; ++i)
	{
		cells[i].m_id = i;
	}

	return cells;
}




