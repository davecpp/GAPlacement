#pragma once
#include "Utils.h"

#include <vector>
#include <string>


using CellID = int_t;
using Point = std::pair<int_t, int_t>;
using PolygonT = std::vector<Point>;

struct Cell
{
	static constexpr CellID fillerID = -1;
	static constexpr CellID invalidID = -2;

	Cell() = default;
	Cell(CellID i_id) : m_id(i_id) {};
	Cell(std::string i_name, CellID i_id) : m_name(i_name), m_id(i_id) {}

	std::string m_name;
	CellID m_id = invalidID;
	int_t width = 8;
	int_t height = 1;


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

	PolygonT getPolygon(int_t x, int_t y) const
	{
		return PolygonT{
			{x, y},
			{x, y + height},
			{x + width, y + height},
			{x + width, y},
			{x, y}
		};
	}

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
		cells[i].width = std::rand() % 4 + 1;
		cells[i].height = std::rand() % 4 + 1;
	}

	return cells;
}




