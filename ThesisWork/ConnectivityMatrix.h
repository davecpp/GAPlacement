#pragma once
#include "Cell.h"
#include <random>

using Connections = size_t;
constexpr size_t connection_max_weight = 5;
inline Connections generate_random_connection()
{
	return std::rand() % connection_max_weight;
}

using AdjacencyMatrixT = MatrixT<Connections>;



inline AdjacencyMatrixT generate_random_adjacency_matrix(size_t cells_count)
{
	auto connections = makeMatrix<AdjacencyMatrixT>(cells_count, cells_count, 0);

	for (size_t i = 0; i < cells_count; ++i)
	{
		for (size_t j = i + 1; j < cells_count; ++j)
		{
			connections[i][j] = generate_random_connection();
			connections[j][i] = connections[i][j];
		}

	}

	return connections;
}

