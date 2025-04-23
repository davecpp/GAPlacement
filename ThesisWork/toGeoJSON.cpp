#include "toGeoJSON.h"


using json = nlohmann::json;

nlohmann::json cell_to_geojson_feature(const Cell& cell, int_t x, int_t y)
{


	json coords;
	for (const auto& [px, py] : cell.getPolygon(x, y))
	{
		coords.push_back({ px, py });
	}

	return {
		{"type", "Feature"},
		{"properties", {
			{"id", cell.getID()},
			{"name", cell.m_name},
			//{"width", cell.width},
			//{"height", cell.height},
			//{"type", cell.is_filler() ? "filler" : (cell.is_normalCell() ? "normal" : "invalid")}
		}},
		{"geometry", {
			{"type", "Polygon"},
			{"coordinates", { coords }}
		}}
	};
}

std::string cells_to_geojson(const MatrixT<Cell>& matrix)
{
	json geojson;
	geojson["type"] = "FeatureCollection";
	geojson["features"] = json::array();

	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix[i].size(); ++j)
		{
			const Cell& cell = matrix[i][j];
			if (cell.m_id != Cell::invalidID && !cell.is_filler())
			{
				geojson["features"].push_back(cell_to_geojson_feature(cell, j, i));
			}
		}
	}

	return geojson.dump(4);
}
