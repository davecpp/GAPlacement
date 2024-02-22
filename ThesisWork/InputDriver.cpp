#include "InputDriver.h"


CellsContainer Scheme::getCellsWithFillers() const
{
	assert(checkField());
	auto cellsWithFilers = m_cells;
	cellsWithFilers.resize(getFieldRows() * getFieldCols(), Cell::fillerID);
	return cellsWithFilers;
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
