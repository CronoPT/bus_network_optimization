#ifndef __GRID_CPP__
#define __GRID_CPP__

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

	const std::set<int>& square::get_bus() const {
		return _bus;
	}

	const std::set<int>& square::get_metro() const {
		return _metro;
	}

	grid::grid():
	 _squares(), 
	 _min_lon(std::numeric_limits<double>::max()), 
	 _min_lat(std::numeric_limits<double>::max()), 
	 _max_lon(std::numeric_limits<double>::lowest()),
	 _max_lat(std::numeric_limits<double>::lowest()), 
	 _lon_step(0), _lat_step(0) {
		/* Do Nothing */
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

			if (!exists_square(sq.first, sq.second)) {
				_squares[sq.first][sq.second] = square(
					sq.first, sq.second
				);
			}
			
			_squares[sq.first][sq.second].add_bus(attr.get_stop_id());
		}

		for (auto& m_node: metro.get_nodes()) {
			auto& attr  = m_node.second.get_attributes(); 
			auto sq = coordinates_to_squares(attr.get_lon(), attr.get_lat());

			if (!exists_square(sq.first, sq.second)) {
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

	void grid::print_report() const {
		int total_squares = 0;
		for (auto& column: _squares) {
			int i = column.first;
			for (auto& line: column.second) {
				int j = line.first;
				std::cout << "(" << i << ",";
				std::cout << j << ") {bus:[";
				for (auto& bus: line.second.get_bus()) {
					std::cout << bus << ", ";
				}
				std::cout << "], metro:[";
				for (auto& metro: line.second.get_metro()) {
					std::cout << metro << ", ";
				}
				std::cout << "]}" << std::endl;
				total_squares += 1;
			}
		}
		std::cout << "Total: " << total_squares << std::endl;
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

	bool grid::element_in(int element, std::set<int> vec) {
		return std::find(vec.begin(), vec.end(), element) != vec.end();
	}

	std::unordered_map<int, std::unordered_map<int, square>>& grid::get_squares() {
		return _squares;
	}

	std::string grid::bus_or_metro(
		int node_id, 
		bus_network bus,
		metro_network metro
	) {
		if (metro.get_nodes().find(node_id) != metro.get_nodes().end()) {
			return "metro";
		} else { return "bus"; }
	}

	bool grid::needs_penalty_bus(net::edge<bus_edge> edge, std::string prev_itinerary) {
		bool diff_routes  = std::to_string(edge.get_attributes().get_route_id()) != prev_itinerary;
		bool not_walking  = prev_itinerary != "walk";
		bool not_starting = prev_itinerary != "start";
		return diff_routes && not_walking && not_starting;
	}

	bool grid::needs_penalty_metro(net::edge<metro_edge> edge, std::string prev_itinerary) {
		bool diff_routes = edge.get_attributes().get_line_color() != prev_itinerary;
		bool not_walking = prev_itinerary != "walk";
		bool not_starting = prev_itinerary != "start";
		return diff_routes && not_walking && not_starting;
	}

	std::pair<std::vector<int>, std::vector<float>> grid::best_path_between(
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
		auto mode  = std::unordered_map<int, std::string>();
		auto prev_mode = std::unordered_map<int, std::string>();
		auto prev_itinerary = std::unordered_map<int, std::string>();
		auto explo = std::set<int>();

		const int undefined = -1; 
		const int transfer_penalty = 10*60;

		// considering bus stops as starting points
		for (auto origin_point: origin_square.get_bus()) {
			prev[origin_point] = undefined;
			dist[origin_point] = 0.0;
			mode[origin_point] = "bus";
			prev_mode[origin_point] = "start";
			prev_itinerary[origin_point] = "start";
			queue.push(origin_point, 0.0);
		}

		// considering metro stations as starting points
		for (auto origin_point: origin_square.get_metro()) {
			prev[origin_point] = undefined;
			dist[origin_point] = 0.0;
			mode[origin_point] = "metro";
			prev_mode[origin_point] = "start";
			prev_itinerary[origin_point] = "start";
			queue.push(origin_point, 0.0);
		}

		const auto& destin_bus   = destin_square.get_bus();
		const auto& destin_metro = destin_square.get_metro();
		int destin_id = undefined; 
		while (!queue.empty()) {

			auto u = queue.pop();
			explo.insert(u);

			// we found our way, u is somewhere in the destiny square
			if (element_in(u, destin_bus) || element_in(u, destin_metro)) {
				destin_id = u;
				break;
			}

			if (mode[u] == "bus") {
				auto& u_node_bus  = bus.get_nodes()[u];
				auto& u_node_walk = walk.get_nodes()[u];

				// checking for connectivity through the bus network
				for (auto& edge_info: u_node_bus.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin(); 

					if (queue.contains(v)) {
						float alt;
						if (needs_penalty_bus(edge, prev_itinerary[u])) {
							alt = dist[u] + edge.get_attributes().get_time_taken() +
							      transfer_penalty;
						} else {
							alt = dist[u] + edge.get_attributes().get_time_taken();
						}
						if (alt < dist[v]) {
							dist[v] = alt;
							prev[v] = u;
							mode[v] = "bus";
							prev_mode[v] = "bus";
							prev_itinerary[v] = std::to_string(edge.get_attributes().get_route_id());
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						if (needs_penalty_bus(edge, prev_itinerary[u])) {
							dist[v] = dist[u] + edge.get_attributes().get_time_taken() +
							      transfer_penalty;
						} else {
							dist[v] = dist[u] + edge.get_attributes().get_time_taken();
						}
						prev[v] = u;
						mode[v] = "bus";
						prev_mode[v] = "bus";
						prev_itinerary[v] = std::to_string(edge.get_attributes().get_route_id());
						queue.push(v, dist[v]);
					}
				}

				// checking for connectivity through the walking network
				for (auto& edge_info: u_node_walk.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();

					if (queue.contains(v)) {
						float alt = dist[u] + edge.get_attributes().get_time_taken();
						if (alt < dist[v] && prev_itinerary[v]!="walk") {
							dist[v] = alt;
							prev[v] = u;
							mode[v] = bus_or_metro(v, bus, metro);
							prev_mode[v] = "walk";
							prev_itinerary[v] = "walk";
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						dist[v] = dist[u] + edge.get_attributes().get_time_taken();
						prev[v] = u;
						mode[v] = bus_or_metro(v, bus, metro);
						prev_mode[v] = "walk";
						prev_itinerary[v] = "walk";
						queue.push(v, dist[v]);
					}
				}

			} else if (mode[u] == "metro") {
				auto& u_node_metro  = metro.get_nodes()[u];
				auto& u_node_walk = walk.get_nodes()[u];

				// checking for connectivity through the bus network
				for (auto& edge_info: u_node_metro.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin(); 

					if (queue.contains(v)) {
						float alt;
						if (needs_penalty_metro(edge, prev_itinerary[u])) {
							alt = dist[u] + edge.get_attributes().get_time_taken() +
							      transfer_penalty;
						} else {
							alt = dist[u] + edge.get_attributes().get_time_taken();
						}
						if (alt < dist[v]) {
							dist[v] = alt;
							prev[v] = u;
							mode[v] = "metro";
							prev_mode[v] = "metro";
							prev_itinerary[v] = edge.get_attributes().get_line_color();
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						if (needs_penalty_metro(edge, prev_itinerary[u])) {
							dist[v] = dist[u] + edge.get_attributes().get_time_taken() +
							          transfer_penalty;
						} else {
							dist[v] = dist[u] + edge.get_attributes().get_time_taken();
						}
						
						prev[v] = u;
						mode[v] = "metro";
						prev_mode[v] = "metro";
						prev_itinerary[v] = edge.get_attributes().get_line_color();
						queue.push(v, dist[v]);
					}
				}

				// checking for connectivity through the walking network
				for (auto& edge_info: u_node_walk.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();

					if (queue.contains(v)) {
						float alt = dist[u] + edge.get_attributes().get_time_taken();
						if (alt < dist[v] && prev_itinerary[v]!="walk") {
							dist[v] = alt;
							prev[v] = u;
							mode[v] = bus_or_metro(v, bus, metro);
							prev_mode[v] = "walk";
							prev_itinerary[v] = "walk";
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						dist[v] = dist[u] + edge.get_attributes().get_time_taken();
						prev[v] = u;
						mode[v] = bus_or_metro(v, bus, metro);
						prev_mode[v] = "walk";
						prev_itinerary[v] = "walk";
						queue.push(v, dist[v]);
					}
				}
			}

		}

		float cost = dist[destin_id];
		auto stk   = std::stack<int>();
		auto stk_d = std::stack<float>();  //TODO remove after testing 
		int  u = destin_id;
		if (prev[u]!=undefined || element_in(u, destin_bus) || 
		    element_in(u, destin_metro)) {
			while (u != undefined) {
				stk.push(u);
				stk_d.push(dist[u]); //TODO remove after testing
				u = prev[u];
			} 
		}

		auto path = std::vector<int>();
		auto costs = std::vector<float>(); //TODO remove after testing
		while (!stk.empty()) {
			path.push_back(stk.top());
			costs.push_back(stk_d.top()); //TODO remove after testing
			stk.pop();
			stk_d.pop();
		}

		return std::pair<std::vector<int>, std::vector<float>>(path, costs);
	}

	void grid::print_progress_bar(int iteration, int total) {
		float percent = 100 * (iteration/total);
		int filled_length = (100 * iteration) / total;
		std::string bar = std::string(filled_length, '=') + 
		                  std::string(100 - filled_length, '-');
		std::cout << "|" << bar << "| ";
		std::cout << iteration << "/" << total << "\r" << std::flush;
		if (iteration == total) { std::cout << std::endl; } 
	}

	int grid::get_total_squares() {
		int sum = 0;
		for (auto& column: _squares) {
			sum += column.second.size();
		}
		return sum;
	}

	void grid::predict_all_od_pairs(
		bus_network bus,
		metro_network metro,
		walking_network walk	
	) {
		int total_sq = get_total_squares();
		int counter  = 0;
		int total = total_sq * (total_sq-1); 
		for (auto& column: _squares) {
			int j = column.first;
			for (auto& line: column.second) {
				int i = line.first;

				for (auto& column_prime: _squares) {
					int j_prime = column_prime.first;
					for (auto& line_prime: column_prime.second) {
						int i_prime = line_prime.first;

						if (i!=i_prime || j!=j_prime) {
							std::cout << "Path number "   << counter++; 
							std::cout << " being computed" << std::endl;
							std::cout << "(" << i << "," << j << ") -> (";
							std::cout << i_prime << ", " << j_prime << ") ";

							auto report = best_path_between(
								std::pair<int, int>(i, j),
								std::pair<int, int>(i_prime, j_prime),
								bus, metro, walk
							);

							counter += 1;
							// print_progress_bar(counter, total);
							for (auto id: report.second) {
								std::cout << id << " ";
							}
							std::cout << "|";
							for (auto id: report.first) {
								std::cout << id << " ";
							}
							std::cout << std::endl;
						}

					}
				}
			}
		}
		
	}

} // namespace urban

#endif