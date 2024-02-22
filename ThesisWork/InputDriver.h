#pragma once

#include <cassert>

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

	size_t getFieldRows() const
	{
		return m_field.rows;
	}
	size_t getFieldCols() const
	{
		return m_field.cols;
	}

	size_t getFieldSize() const
	{
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
	const CellsContainer& getCells() const
	{
		return m_cells;
	}

	void setConnections(const AdjacencyMatrixT& connections)
	{
		m_connections = connections;
	}

	bool is_valid() const
	{
		return checkField() && checkCellsConnections();
	}

	CellsContainer getCellsWithFillers() const;

private:
	bool checkCellsConnections() const;
	bool checkField() const
	{
		return getFieldSize() >= m_cells.size();
	}
};



