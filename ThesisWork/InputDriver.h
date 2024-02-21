#pragma once
#include "Cell.h"
#include "ConnectivityMatrix.h"

struct ComutFieldParams
{
	ComutFieldParams(size_t i_rows, size_t i_cols) : rows(i_rows), cols(i_cols) {};
	ComutFieldParams() = default;
	size_t rows = 0;
	size_t cols = 0;
};

class Scheme {
	CellsContainer m_cells;
	AdjacencyMatrixT m_connections;
	ComutFieldParams m_field;

public:

	size_t getRows() const {
		return m_field.rows;
	}
	size_t getCols() const {
		return m_field.cols;
	}

	size_t getFieldSize() const {
		return m_field.rows * m_field.cols;
	}

	void setFieldParams(ComutFieldParams fieldparams)
	{
		m_field = fieldparams;
	}

	void setCells(const CellsContainer& cells)
	{
		m_cells = cells;
	}
	const CellsContainer& getCells() const {
		return m_cells;
	}

	void setConnections(const AdjacencyMatrixT& connections)
	{
		m_connections = connections;
	}

	CellsContainer getCellsWithFillers() const
	{
		assert(checkField());
		auto cellsWithFilers = m_cells;
		cellsWithFilers.resize(getRows() * getCols(), Cell::fillerID);
		return cellsWithFilers;
	}

	bool is_valid() const
	{
		return checkField() && checkCellsConnections();
	}


private:
	bool checkCellsConnections() const {
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
	bool checkField() const
	{
		return getFieldSize() >= m_cells.size();
	}
};



