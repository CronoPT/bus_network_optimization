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
	 _lon_step(0), _lat_step(0), _node_to_index(),
	 _total_nodes(0), _node_modes() {
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
	 _lon_step(0), _lat_step(0), _node_to_index(),
	 _total_nodes(0), _node_modes() {

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
			_node_to_index[b_node.first] = _total_nodes++;
			_node_modes[b_node.first] = BUS;
			std::cout << b_node.first << " -> " << _node_to_index[b_node.first] << std::endl;
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
			_node_to_index[m_node.first] = _total_nodes++;
			_node_modes[m_node.first] = METRO;
			std::cout << m_node.first << " -> " << _node_to_index[m_node.first] << std::endl;	
		}

		dist  = std::vector<float>(_total_nodes, 0);
		prev  = std::vector<int>(_total_nodes, 0);
		mode  = std::vector<int>(_total_nodes, 0);
		prev_mode = std::vector<int>(_total_nodes, 0); 
		prev_itinerary = std::vector<int>(_total_nodes, 0);

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
 	
	bool grid::needs_penalty_bus(net::edge<bus_edge>& edge, int prev_itinerary) {
		bool diff_routes  = edge.get_attributes().get_route_id() != prev_itinerary;
		bool not_walking  = prev_itinerary != WALK;
		bool not_starting = prev_itinerary != START;
		return diff_routes && not_walking && not_starting;
	}

	bool grid::needs_penalty_metro(net::edge<metro_edge>& edge, int prev_itinerary) {
		bool diff_routes = edge.get_attributes().get_line_color() != prev_itinerary;
		bool not_walking = prev_itinerary != WALK;
		bool not_starting = prev_itinerary != START;
		return diff_routes && not_walking && not_starting;
	}

	grid::all_paths_report grid::best_path_between_all(
		std::pair<int, int> origin, 
		bus_network bus,
		metro_network metro,
		walking_network walk
		) {
		
		auto& origin_square = _squares[origin.first][origin.second]; 

		int total_paths = 0;
		int total_paths_computed = 0;
		auto squares_check = std::unordered_map<int, std::unordered_map<int, bool>>();
		auto square_finale = std::unordered_map<int, std::unordered_map<int, int>>();
		for (auto& column: _squares) {
			int i = column.first;
			for (auto& line: column.second) {
				int j = line.first;
				squares_check[i][j] = false;
				total_paths += 1;
			}
		}

		auto queue = net::priority_queue<int>();
		auto explo = std::set<int>();

		const int undefined = -1; 
		const int transfer_penalty = 10*60;

		// considering bus stops as starting points
		for (auto origin_point: origin_square.get_bus()) {
			int origin_point_i = _node_to_index[origin_point];
			prev[origin_point_i] = undefined;
			dist[origin_point_i] = 0.0;
			mode[origin_point_i] = BUS;
			prev_mode[origin_point_i] = START;
			prev_itinerary[origin_point_i] = START;
			queue.push(origin_point, 0.0);
		}

		// considering metro stations as starting points
		for (auto origin_point: origin_square.get_metro()) {
			int origin_point_i = _node_to_index[origin_point];
			prev[origin_point_i] = undefined;
			dist[origin_point_i] = 0.0;
			mode[origin_point_i] = METRO;
			prev_mode[origin_point_i] = START;
			prev_itinerary[origin_point_i] = START;
			queue.push(origin_point, 0.0);
		}

		while (!queue.empty()) {

			auto u = queue.pop();
			explo.insert(u);

			std::pair<int, int> i_j;
			if (_node_modes[u] == BUS) {
				auto& u_node = bus.get_nodes()[u].get_attributes();
				i_j = coordinates_to_squares(u_node.get_lon(), u_node.get_lat());
			} else if (_node_modes[u] == METRO) {
				auto& u_node = metro.get_nodes()[u].get_attributes();
				i_j = coordinates_to_squares(u_node.get_lon(), u_node.get_lat());
			}

			int i = i_j.first;
			int j = i_j.second;
			if (!squares_check[i][j]) {
				squares_check[i][j] = true;
				square_finale[i][j] = u;
				total_paths_computed += 1;
			}

			if (total_paths_computed == total_paths) {
				break;
			}
			
			int u_i = _node_to_index[u];
			if (mode[u_i] == BUS) {
				auto& u_node_bus  = bus.get_nodes()[u];
				auto& u_node_walk = walk.get_nodes()[u];

				// checking for connectivity through the bus network
				for (auto& edge_info: u_node_bus.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin(); 
					int    v_i  = _node_to_index[v];

					if (queue.contains(v)) {
						float alt;
						if (needs_penalty_bus(edge, prev_itinerary[u_i])) {
							alt = dist[u_i] + edge.get_attributes().get_time_taken() +
							      transfer_penalty;
						} else {
							alt = dist[u_i] + edge.get_attributes().get_time_taken();
						}
						if (alt < dist[v_i]) {
							dist[v_i] = alt;
							prev[v_i] = u;
							mode[v_i] = BUS;
							prev_mode[v_i] = BUS;
							prev_itinerary[v_i] = edge.get_attributes().get_route_id();
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						if (needs_penalty_bus(edge, prev_itinerary[u_i])) {
							dist[v_i] = dist[u_i] + edge.get_attributes().get_time_taken() +
							      transfer_penalty;
						} else {
							dist[v_i] = dist[u_i] + edge.get_attributes().get_time_taken();
						}
						prev[v_i] = u;
						mode[v_i] = BUS;
						prev_mode[v_i] = BUS;
						prev_itinerary[v_i] = edge.get_attributes().get_route_id();
						queue.push(v, dist[v_i]);
					}
				}

				// checking for connectivity through the walking network
				for (auto& edge_info: u_node_walk.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();
					int    v_i  = _node_to_index[v];

					if (queue.contains(v)) {
						float alt = dist[u_i] + edge.get_attributes().get_time_taken();
						if (alt < dist[v_i] && prev_itinerary[v_i]!=WALK) {
							dist[v_i] = alt;
							prev[v_i] = u;
							mode[v_i] = _node_modes[v];
							prev_mode[v_i] = WALK;
							prev_itinerary[v_i] = WALK;
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						dist[v_i] = dist[u_i] + edge.get_attributes().get_time_taken();
						prev[v_i] = u;
						mode[v_i] = _node_modes[v];
						prev_mode[v_i] = WALK;
						prev_itinerary[v_i] = WALK;
						queue.push(v, dist[v_i]);
					}
				}

			} else if (mode[u_i] == METRO) {
				auto& u_node_metro = metro.get_nodes()[u];
				auto& u_node_walk  = walk.get_nodes()[u];

				// checking for connectivity through the bus network
				for (auto& edge_info: u_node_metro.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();
					int    v_i  = _node_to_index[v]; 

					if (queue.contains(v)) {
						float alt;
						if (needs_penalty_metro(edge, prev_itinerary[u_i])) {
							alt = dist[u_i] + edge.get_attributes().get_time_taken() +
							      transfer_penalty;
						} else {
							alt = dist[u_i] + edge.get_attributes().get_time_taken();
						}
						if (alt < dist[v_i]) {
							dist[v_i] = alt;
							prev[v_i] = u;
							mode[v_i] = METRO;
							prev_mode[v_i] = METRO;
							prev_itinerary[v_i] = edge.get_attributes().get_line_color();
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						if (needs_penalty_metro(edge, prev_itinerary[u_i])) {
							dist[v_i] = dist[u_i] + edge.get_attributes().get_time_taken() +
							          transfer_penalty;
						} else {
							dist[v_i] = dist[u_i] + edge.get_attributes().get_time_taken();
						}
						
						prev[v_i] = u;
						mode[v_i] = METRO;
						prev_mode[v_i] = METRO;
						prev_itinerary[v_i] = edge.get_attributes().get_line_color();
						queue.push(v, dist[v_i]);
					}
				}

				// checking for connectivity through the walking network
				for (auto& edge_info: u_node_walk.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();
					int    v_i  = _node_to_index[v];

					if (queue.contains(v)) {
						float alt = dist[u_i] + edge.get_attributes().get_time_taken();
						if (alt < dist[v_i] && prev_itinerary[v_i]!=WALK) {
							dist[v_i] = alt;
							prev[v_i] = u;
							mode[v_i] = _node_modes[v];
							prev_mode[v_i] = WALK;
							prev_itinerary[v_i] = WALK;
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						dist[v_i] = dist[u_i] + edge.get_attributes().get_time_taken();
						prev[v_i] = u;
						mode[v_i] = _node_modes[v];
						prev_mode[v_i] = WALK;
						prev_itinerary[v_i] = WALK;
						queue.push(v, dist[v_i]);
					}
				}
			}
		}

		auto result = all_paths_report();
		for (auto& column: square_finale) {
			auto i = column.first;
			for (auto& line: column.second) {
				auto j = line.first;
				
				auto stk   = std::stack<int>();
				int u      = line.second;
				int u_i    = _node_to_index[u]; 
				float cost = dist[u_i];
				if (prev[u_i]!=undefined) {
					while (u != undefined) {
						stk.push(u);
						u = prev[u_i];
						u_i = _node_to_index[u];	
					} 
				}

				auto path = std::vector<int>();
				while (!stk.empty()) {
					path.push_back(stk.top());
					stk.pop();
				}

				auto destin_sq = std::pair<int, int>(i, j);
				auto single_report = std::pair<std::vector<int>, float>(path, cost);
				auto report_pair = std::pair<std::pair<int, int>, std::pair<std::vector<int>, float>>(
					destin_sq, single_report
				);
				result.push_back(report_pair);
			}
		}
		return result;
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
			int i = column.first;
			for (auto& line: column.second) {
				int j = line.first;
				auto report = best_path_between_all(
					std::pair<int, int>(i, j),
					bus, metro, walk
				);

				// for (auto& individual_report: report) {
				// 	auto& destin = individual_report.first;
				// 	auto& path_info = individual_report.second;
				// 	std::cout << "(" << i << "," << j;
				// 	std::cout << ") -> (" << destin.first;
				// 	std::cout << "," << destin.second << ") | cost: ";
				// 	std::cout << path_info.second << " | path: ";
				// 	for (auto node: path_info.first) {
				// 		std::cout << node << " ";
				// 	}
				// 	std::cout << std::endl;
				// }

				counter += 1;
				print_progress_bar(counter, total_sq);
			}
		}
		
	}

} // namespace urban

#endif