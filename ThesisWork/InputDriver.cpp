#include "InputDriver.h"
#include "Coord.h"

#include <iostream>
#include <iomanip>

CellsContainer Scheme::getCellsWithFillers() const
{
	assert(checkField());
	auto cellsWithFilers = m_cells;
	cellsWithFilers.resize(getFieldRows() * getFieldCols(), Cell::fillerID);
	return cellsWithFilers;
}


//Experimental Function
double Scheme::BestFitness()
{
	double F = 0.0;

	const size_t N = m_cells.size();
	const size_t rows = static_cast<size_t>(std::ceil(std::sqrt(N)));
	const size_t cols = rows;

	auto getPos = [this, rows, cols](size_t n)
	{
		auto x = std::ceil(static_cast<double>(n / rows));
		auto y = (n % cols);
		return Coord(x, y);
	};


	MatrixT<size_t> distanceMatrix = makeMatrix<MatrixT<size_t>>(N, N, 0);
	std::vector<size_t> distances;
	distances.reserve(N);
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{

			distanceMatrix[i][j] = Coord::distance(getPos(i), getPos(j));
		}
	}
	for (size_t i = 0; i < distanceMatrix.size(); i++)
	{
		for (size_t j = i + 1; j < distanceMatrix[i].size(); j++)
		{
			auto d = Coord::distance(getPos(i), getPos(j));
			if (d != 0)
				distances.emplace_back(d);
		}
	}

	std::sort(distances.begin(), distances.end());

	std::vector<Connections> connections;
	connections.reserve(m_connections.size() * m_connections.size());
	for (size_t i = 0; i < m_connections.size(); i++)
	{

		for (size_t j = i + 1; j < m_connections.size(); j++)
		{
			//if (m_connections[i][j] != 0)
			connections.emplace_back(m_connections[i][j]);
		}
	}

	std::sort(connections.begin(), connections.end(), [](auto c1, auto c2) {return c1 > c2; });

	for (size_t i = 0; i < std::min(connections.size(), distances.size()); i++)
	{
		F += distances[i] * connections[i];
	}
	return F / m_connections.size();
}

bool Scheme::checkCellsConnections() const {
	if (m_connections.empty() && m_connections.size() != m_cells.size())
		return false;
	auto cols = m_connections.size();
	for (size_t i = 0; i < m_connections.size(); ++i)
	{
		if (m_connections[i].size() != cols)
			return false;
	}
	return true;
}
