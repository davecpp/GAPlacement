#pragma once
#include "Utils.h"


class Coord
{
	static constexpr int_t invalidCoordinate = -1;
	int_t m_x = invalidCoordinate;
	int_t m_y = invalidCoordinate;
public:
	Coord(int_t i_x, int_t i_y) : m_x(i_x), m_y(i_y) {}
	//Coord() = default;

	bool operator==(const Coord& other)
	{
		return m_x == other.m_x && m_y == other.m_y;
	}

	int_t x() const {
		return m_x;
	}
	int_t y() const {
		return m_y;
	}

	bool isValidCoord()
	{
		return m_x >= 0 && m_y >= 0;
	}

	static Coord invalidCoord()
	{
		return Coord(invalidCoordinate, invalidCoordinate);
	}

	//calculate the Manhattan distance between c1 and c2
	static int_t manh_distance(Coord c1, Coord c2)
	{
		return std::abs(c1.x() - c2.x()) + std::abs(c1.y() - c2.y());
	}
	//default function for distance calculation
	static int_t distance(Coord c1, Coord c2)
	{
		return manh_distance(c1, c2);
	}
};


