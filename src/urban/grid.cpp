#ifndef __PREDICTOR_CPP__
#define __PREDICTOR_CPP__

#include "grid.hpp"

namespace urban {

	square::square():
	 _i(-1), _j(-1), _bus(), _metro() {
		/* Do Nothing */
	}

	square::square(int i, int j):
	 _i(i), _j(j), _bus(), _metro() {
		/* Do Nothing */
	}

	void square::add_bus(int stop_id) {
		_bus.insert(stop_id);
	}

	void square::add_metro(int station_id) {
		_metro.insert(station_id);
	}

	grid::grid(
		osm_net::osm_net road,
		bus_network bus,
		metro_network metro
	): 
	 _squares(), 
	 _min_lon(std::numeric_limits<double>::max()), 
	 _min_lat(std::numeric_limits<double>::max()), 
	 _max_lon(std::numeric_limits<double>::lowest()),
	 _max_lat(std::numeric_limits<double>::lowest()), 
	 _lon_step(0), _lat_step(0) {
		for (auto& r_node: road.get_nodes()) {
			auto& attr = r_node.second.get_attributes();
			if (attr.get_lon() < _min_lon)
				_min_lon = attr.get_lon();
			if (attr.get_lat() < _min_lat)
				_min_lat = attr.get_lat();
			if (attr.get_lon() > _max_lon)
				_max_lon = attr.get_lon();
			if (attr.get_lat() > _max_lat)
				_max_lat = attr.get_lat();
		}

		_lon_step = (_max_lon-_min_lon)/_divisions;
		_lat_step = (_max_lat-_min_lat)/_divisions;

		for (auto& b_node: bus.get_nodes()) {
			auto& attr = b_node.second.get_attributes(); 
			auto sq = coordinates_to_squares(attr.get_lon(), attr.get_lat());

			if (!exists_square(attr.get_lon(), attr.get_lat())) {
				_squares[sq.first][sq.second] = square(
					sq.first, sq.second
				);
			}
			
			_squares[sq.first][sq.second].add_bus(attr.get_stop_id());
		}

		for (auto& m_node: metro.get_nodes()) {
			auto& attr  = m_node.second.get_attributes(); 
			auto sq = coordinates_to_squares(attr.get_lon(), attr.get_lat());

			if (!exists_square(attr.get_lon(), attr.get_lat())) {
				_squares[sq.first][sq.second] = square(
					sq.first, sq.second
				);
			}

			_squares[sq.first][sq.second].add_metro(attr.get_station_id());	
		}
	}

	std::pair<int, int> grid::coordinates_to_squares(double lon, double lat) const {
		int i = (lon-_min_lon)/_lon_step;
		int j = (lat-_min_lat)/_lat_step;
		i = (i < _divisions) ? i : i-1;
		j = (j < _divisions) ? j : j-1;
		return std::pair<int, int>(i, j);
	}

	bool grid::exists_square(int i, int j) const {
		bool exists_column = _squares.find(i) != _squares.end();
		if (!exists_column) { return false; }
		bool exists_square = _squares.find(i)->second.find(j) != 
		                     _squares.find(i)->second.end();
		return exists_square;
	}

	void grid::generate_geojson() const {
		nlohmann::json json_grid;

		for (int i=0; i<_divisions+1; i++) {	
			// horizontal line
			json_grid.push_back({
				{_min_lon, _min_lat + i*_lat_step},
				{_max_lon, _min_lat + i*_lat_step}
			});

			// vertical line
			json_grid.push_back({
				{_min_lon + i*_lon_step, _min_lat},
				{_min_lon + i*_lon_step, _max_lat}
			});
		}
		
		std::ofstream o("../data/geojson/grid.geojson");
		o << "{\n\t" << "\"type\": \"MultiLineString\",\n\t";
		o << "\"coordinates\": ";
		o << std::setw(4) << json_grid << std::endl;
		o << "}" << std::endl;
	}

	void grid::best_path_between(
		std::pair<int, int> origin, 
		std::pair<int, int> destin, 
		bus_network bus,
		metro_network metro,
		walking_network walk
		) {
		
		auto& origin_square = _squares[origin.first][origin.second];
		auto& destin_square = _squares[destin.first][destin.second];

		auto queue = net::priority_queue<int>();
		auto dist  = std::unordered_map<int, float>();
		auto prev  = std::unordered_map<int, int>();
		auto explo = std::set<int>();

		const int undefined = -1; 
	}

} // namespace urban

#endif