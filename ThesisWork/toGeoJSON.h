#pragma once

#include "Cell.h"
#include <nlohmann/json.hpp>


nlohmann::json cell_to_geojson_feature(const Cell& cell, int_t x, int_t y);


std::string cells_to_geojson(const MatrixT<Cell>& cells);