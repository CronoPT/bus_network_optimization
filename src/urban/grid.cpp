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

		// Check for the grid limits
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

		// Place every bus node in the grid and initialize
		// the respective auxiliary structures.
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
		}

		// Place every metro node in the grid and initialize
		// the respective auxiliary structures.
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
		}

		// The auxiliary structures for the path finding algorithm
		// are initialized here. There is no need to realocate
		// memory each and every time we want to compute the 
		// shortest paths.
		dist  = std::vector<float>(_total_nodes, 0);
		prev  = std::vector<int>(_total_nodes, 0);
		mode  = std::vector<int>(_total_nodes, 0);
		prev_mode = std::vector<int>(_total_nodes, 0); 
		prev_itinerary = std::vector<int>(_total_nodes, 0);

	}

	/**
	 * Translate a given coordinate pair to a square.
	 * 	@param lon: longitude of the point to translate
	 * 	@param lat: latitude of the point to translate
	 * 
	 * 	@return <i, j>: the column and line of the respective 
	 * 	square in the grid
	*/
	std::pair<int, int> grid::coordinates_to_squares(double lon, double lat) const {
		int i = (lon-_min_lon)/_lon_step;
		int j = (lat-_min_lat)/_lat_step;
		i = (i < _divisions) ? i : i-1;
		j = (j < _divisions) ? j : j-1;
		return std::pair<int, int>(i, j);
	}

	/**
	 * See if a given square already registered in the grid.
	 * 	@param i: the column of the square
	 * 	@param j: the line of the square
	 * 
	 * 	@return bool: whether the square exists or not
	*/
	bool grid::exists_square(int i, int j) const {
		bool exists_column = _squares.find(i) != _squares.end();
		if (!exists_column) { return false; }
		bool exists_square = _squares.find(i)->second.find(j) != 
		                     _squares.find(i)->second.end();
		return exists_square;
	}

	/**
	 * Print a summary of the divisions on the grid.
	 * For each square, show the bus stops and metro
	 * stations in it.
	 * 	@return void
	*/
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

	/**
	 * Generate a .geojson file to allow for a visualization
	 * of the division of te physical space that the grid 
	 * is currently providing.
	 * 	@return void
	*/
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

	std::unordered_map<int, std::unordered_map<int, square>>& grid::get_squares() {
		return _squares;
	}
 	
	/**
	 * (Version for bus edges)
	 * Check if following an edge coming from a given route or metro line implies
	 * the transfer time penalty. Coming from the start or from walking does not
	 * trigger the penalty. Walking implies a penalty but that was already taxed 
	 * when the path switched from a bus-route/metro-line to a walking edge.
	 * 	@param edge: the edge we are considering taking
	 * 	@param prev_itinerary: the bus-route/metro-line we came from
	 * 
	 * 	@return whether or not a time penalty is needed
	*/
	bool grid::needs_penalty_bus(net::edge<bus_edge>& edge, int prev_itinerary) {
		bool diff_routes  = edge.get_attributes().get_route_id() != prev_itinerary;
		bool not_walking  = prev_itinerary != WALK;
		bool not_starting = prev_itinerary != START;
		return diff_routes && not_walking && not_starting;
	}

	/**
	 * (Version for metro edges)
	 * Check if following an edge coming from a given route or metro line implies
	 * the transfer time penalty. Coming from the start or from walking does not
	 * trigger the penalty. Walking implies a penalty but that was already taxed 
	 * when the path switched from a bus-route/metro-line to a walking edge.
	 * 	@param edge: the edge we are considering taking
	 * 	@param prev_itinerary: the bus-route/metro-line we came from
	 * 	@return whether or not a time penalty is needed
	*/
	bool grid::needs_penalty_metro(net::edge<metro_edge>& edge, int prev_itinerary) {
		bool diff_routes = edge.get_attributes().get_line_color() != prev_itinerary;
		bool not_walking = prev_itinerary != WALK;
		bool not_starting = prev_itinerary != START;
		return diff_routes && not_walking && not_starting;
	}

	/**
	 * This function computes all the shortest paths between one square in
	 * the grid and all other squares. We do not have a big network with all
	 * the modes (bus, metro and walking) because the bus network has to be
	 * detacheable since this will mainly be a way of computing the cost 
	 * function for an optimization problem which aims to optimize the bus
	 * network.
	 * 	@param origin: the square from which to compute the paths
	 *	@param bus: the bus network (will probably change frequently)
	 * 	@param metro: the metro network (will probably remain the same)
	 * 	@param walk: the walking network (will probably remain the same)
	 * 	
	 * 	@return for each square in the grid, the shortest path and the cost 
	 * 
	 * There where several optimization concerns in this algorithm, perhaps
	 * one that might seem confusing, is the fact that vectors are being used
	 * as the auxiliary structures and this forces us to always convert a node
	 * id to an index. This happens because our nodes' ids are not a sequencial
	 * identifier, so it is necessary to keep a map with the right indexes. 
	 * A map could be used instead of the vector but because there are so many
	 * read and write operations in the auxiliary structures, a vector with 
	 * O(1) access goes a long way compared with a O(1) amortized access that
	 * is provided by the std::unordered_map.  
	*/
	grid::all_paths_report grid::best_path_between_all(
		std::pair<int, int> origin, 
		bus_network bus,
		metro_network metro,
		walking_network walk
		) {
		
		auto& origin_square = _squares[origin.first][origin.second]; 

		int total_paths = 0;
		int total_paths_computed = 0;
		
		// Auxiliary structure to check, for every square, if a path was found
		// from the origin to it. 
		auto squares_check = std::unordered_map<int, std::unordered_map<int, bool>>();

		// Auxiliary structure to save, for every square, the last node on the 
		// shortest path from the origin to it.
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
			// We found the shortes path from the origin to the
			// square <i, j>. Let's register the finding.
			if (!squares_check[i][j]) {
				squares_check[i][j] = true;
				square_finale[i][j] = u;
				total_paths_computed += 1;
			}

			// We can leave the cycle once a path was found from
			// the origin to every other square.
			if (total_paths_computed == total_paths) {
				break;
			}
			
			/**
			 * There are two options for a node, it is either a bus node
			 * or a metro node. Then, in either case, we have to check
			 * for connectivity in the respective network but we also 
			 * have to check for connectivity via walking, that is why
			 * the function is so long, but in reallity it is just a 
			 * repetition for the metro case and the bus case, and then
			 * for the walking case in both modes of transportation
			*/
			int u_i = _node_to_index[u];
			if (mode[u_i] == BUS) { // the node popped is from the bus network
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

			} else if (mode[u_i] == METRO) { // the node popped is from the metro network
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

		// This will build the paths from the prev vector.
		// A path is built for each square, hence the complex
		// return type. It is basically the path constrution
		// bit in the Dijskstra's Algorithm but for every
		// path between the origin and a given square.
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

	/**
	 * Print a simple progress bar. Used mainly to see the
	 * progress of the path finding algorithm.
	 * 	@param iteration: the current progress (step)
	 * 	@param total: the total number of steps
	 * 
	 * 	@return void
	*/
	void grid::print_progress_bar(int iteration, int total) {
		float percent = 100 * (iteration/total);
		int filled_length = (100 * iteration) / total;
		std::string bar = std::string(filled_length, '=') + 
		                  std::string(100 - filled_length, '-');
		std::cout << "|" << bar << "| ";
		std::cout << iteration << "/" << total << "\r" << std::flush;
		if (iteration == total) { std::cout << std::endl; } 
	}

	/**
	 * Get the total number of squares with content in the grid.
	 * 	@return number of squares containing something in the grid.
	*/
	int grid::get_total_squares() {
		int sum = 0;
		for (auto& column: _squares) {
			sum += column.second.size();
		}
		return sum;
	}

	/**
	 * Given an origin and the result for the path finding
	 * algorithm, print the paths to the console.
	 * 	@param origin: the origin square for all paths
	 * 	@param report: the paths and costs to every square
	 * 
	 * 	@return void
	*/
	void grid::print_report(std::pair<int, int> origin, all_paths_report report) {
		int i = origin.first;
		int j = origin.second;
		for (auto& individual_report: report) {
			auto& destin = individual_report.first;
			auto& path_info = individual_report.second;
			std::cout << "(" << i << "," << j;
			std::cout << ") -> (" << destin.first;
			std::cout << "," << destin.second << ") | cost: ";
			std::cout << path_info.second << " | path: ";
			for (auto node: path_info.first) {
				std::cout << node << " ";
			}
			std::cout << std::endl;
		}
	}

	/**
	 * Run the path finding algorithm for every square as an origin.
	 * 	@param bus: the bus network to be used
	 * 	@param metro: the metro network
	*/
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

				counter += 1;
				print_progress_bar(counter, total_sq);
			}
		}
		
	}

} // namespace urban

#endif