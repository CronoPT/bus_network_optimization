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

	grid::grid(

	): 
	 _squares(), 
	 _min_lon(std::numeric_limits<double>::max()), 
	 _min_lat(std::numeric_limits<double>::max()), 
	 _max_lon(std::numeric_limits<double>::lowest()),
	 _max_lat(std::numeric_limits<double>::lowest()), 
	 _lon_step(0), _lat_step(0), _node_to_index(),
	 _total_nodes(0), _node_modes() {

		// Check for the grid limits
		for (auto& r_node: osm_net::osm_net::instance()->get_nodes()) {
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
		for (auto& bus_stop: bus_network::get_stops()) {
			auto stop_id = bus_stop.first;
			auto lon = bus_stop.second.first;
			auto lat = bus_stop.second.second; 
			auto sq = coordinates_to_squares(lon, lat);

			if (!exists_square(sq.first, sq.second)) {
				_squares[sq.first][sq.second] = square(
					sq.first, sq.second
				);
			}
			
			_squares[sq.first][sq.second].add_bus(stop_id);
		}

		// Place every metro node in the grid and initialize
		// the respective auxiliary structures.
		for (auto& m_node: metro_network::instance()->get_nodes()) {
			auto& attr  = m_node.second.get_attributes(); 
			auto sq = coordinates_to_squares(attr.get_lon(), attr.get_lat());

			if (!exists_square(sq.first, sq.second)) {
				_squares[sq.first][sq.second] = square(
					sq.first, sq.second
				);
			}

			_squares[sq.first][sq.second].add_metro(attr.get_station_id());	
		}

		// The auxiliary structures for the path finding algorithm
		// are initialized here. There is no need to realocate
		// memory each and every time we want to compute the 
		// shortest paths.
		// dist  = std::vector<float>(_total_nodes, 0);
		// prev  = std::vector<int>(_total_nodes, 0);
		// mode  = std::vector<int>(_total_nodes, 0);
		// prev_itinerary = std::vector<int>(_total_nodes, 0);

	}

	grid* grid::instance() {
		if (!_instance) {
			_instance = new grid();
		}
		return _instance;
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
	std::vector<single_path_report> grid::best_path_between_all(
		std::pair<int, int> origin, 
		bus_network& bus
	) {

		auto metro_dist = std::vector<float>(metro_network::instance()->get_number_of_nodes(), 0);
		auto metro_prev = std::vector<int>(metro_network::instance()->get_number_of_nodes(), 0);
		auto metro_extra = std::vector<int>(metro_network::instance()->get_number_of_nodes(), 0);
		auto metro_index = [] (int node_id) -> int {
			return metro_network::instance()->get_node_index(node_id);
		};

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
			for (auto variant: bus.get_stop_variants(origin_point)) {
				bus.mark_prev(variant, undefined);
				bus.mark_dist(variant, 0.0);
				bus.mark_extra(variant, START);
				queue.push(variant, 0.0);
			}
		}

		// considering metro stations as starting points
		for (auto origin_point: origin_square.get_metro()) {
			metro_prev[metro_index(origin_point)] = undefined;
			metro_dist[metro_index(origin_point)] = 0.0;
			metro_extra[metro_index(origin_point)] = START;
			queue.push(origin_point, 0.0);
		}

		while (!queue.empty()) {

			auto u = queue.pop();
			explo.insert(u);

			std::pair<int, int> i_j;
			if (!is_metro_node(u)) { // u is a bus node
				auto& u_node = bus.get_nodes()[u].get_attributes();
				i_j = coordinates_to_squares(u_node.get_lon(), u_node.get_lat());
			} else { // us is a metro node
				auto& u_node = metro_network::instance()->get_nodes()[u].get_attributes();
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
			if (!is_metro_node(u)) { // the node popped is from the bus network
				auto& u_node_bus  = bus.get_nodes()[u];
				auto  u_pair  = bus_network::node_id(u);
				auto  u_stop  = u_pair.first;
				auto  u_route = u_pair.second;
				auto& u_node_walk = walking_network::instance()->get_nodes()[u_stop];

				// checking for connectivity through the bus network
				for (auto& edge_info: u_node_bus.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin(); 

					if (queue.contains(v)) {
						float alt;
						alt = bus.get_dist(u) + edge.get_attributes().get_time_taken();
						if (alt < bus.get_dist(v)) {
							bus.mark_dist(v, alt);
							bus.mark_prev(v, u);
							bus.mark_extra(v, edge.get_attributes().get_route_id());
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						bus.mark_dist(v, bus.get_dist(u)+edge.get_attributes().get_time_taken());
						bus.mark_prev(v, u);
						bus.mark_extra(v, edge.get_attributes().get_route_id());
						queue.push(v, bus.get_dist(v));
					}
				}

				// checking for connectivity through the walking network
				for (auto& edge_info: u_node_walk.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();

					if (!is_metro_node(u)) { // bus to bus
						/**
						 * Accounts for the fact that now, every stop
						 * in the bus network has various node mappings,
						 * one for each route the stop is serving. 
						*/
						for (int v_variant: bus.get_stop_variants(v)) {
							if (queue.contains(v_variant)) {
								float alt = bus.get_dist(u) + edge.get_attributes().get_time_taken() 
											+ transfer_penalty;
								if (alt < bus.get_dist(v_variant) && bus.get_extra(v_variant)!=WALK) {
									bus.mark_dist(v_variant, alt);
									bus.mark_prev(v_variant, u);
									bus.mark_extra(v_variant, WALK);
									queue.update(v_variant, alt);
								}
							} else if (explo.find(v_variant) == explo.end()) {
								/* v hasn't been visited before */
								bus.mark_dist(v_variant, bus.get_dist(u)
									+ edge.get_attributes().get_time_taken()
									+ transfer_penalty);

								bus.mark_prev(v_variant, u);
								bus.mark_extra(v_variant, WALK);
								queue.push(v_variant, bus.get_dist(v_variant));
							}
						}
					} else { // bus to metro
						if (queue.contains(v)) {
							float alt = bus.get_dist(u) + edge.get_attributes().get_time_taken() 
										+ transfer_penalty;
							if (alt < metro_dist[metro_index(v)] && 
							 metro_extra[metro_index(v)]!=WALK) {
								metro_dist[metro_index(v)] = alt;
								metro_prev[metro_index(v)] = u;
								metro_extra[metro_index(v)] = WALK;
								queue.update(v, alt);
							}
						} else if (explo.find(v) == explo.end()) {
							/* v hasn't been visited before */
							metro_dist[metro_index(v)] = 
								bus.get_dist(u) + edge.get_attributes().get_time_taken() 
								+ transfer_penalty;
							metro_prev[metro_index(v)] = u;
							metro_extra[metro_index(v)] = WALK;
							queue.push(v, metro_dist[metro_index(v)]);
						}
					}
				}

			} else { // the node popped is from the metro network
				auto& u_node_metro = metro_network::instance()->get_nodes()[u];
				auto& u_node_walk  = walking_network::instance()->get_nodes()[u];

				// checking for connectivity through the metro network
				for (auto& edge_info: u_node_metro.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();

					if (queue.contains(v)) {
						float alt;
						if (needs_penalty_metro(edge, metro_extra[metro_index(u)])) {
							alt = metro_dist[metro_index(u)] 
							      + edge.get_attributes().get_time_taken() 
								  + transfer_penalty;
						} else {
							alt = metro_dist[metro_index(u)] 
							      + edge.get_attributes().get_time_taken();
						}
						if (alt < metro_dist[metro_index(v)] ) {
							metro_dist[metro_index(v)] = alt;
							metro_prev[metro_index(v)] = u;
							metro_extra[metro_index(v)] = edge.get_attributes().get_line_color();
							queue.update(v, alt);
						}
					} else if (explo.find(v) == explo.end()) {
						/* v hasn't been visited before */
						if (needs_penalty_metro(edge, metro_extra[metro_index(u)])) {
							metro_dist[metro_index(v)] =  
								metro_dist[metro_index(u)] 
								+ edge.get_attributes().get_time_taken() 
								+ transfer_penalty;
						} else {
							metro_dist[metro_index(v)] =  
								metro_dist[metro_index(u)] 
								+ edge.get_attributes().get_time_taken();
						}
						
						metro_prev[metro_index(v)] = u;
						metro_extra[metro_index(v)] = edge.get_attributes().get_line_color();
						queue.push(v, metro_dist[metro_index(v)] );
					}
				}

				// checking for connectivity through the walking network
				for (auto& edge_info: u_node_walk.get_adjacencies()) {
					auto   key  = edge_info.first;
					auto&  edge = edge_info.second;
					auto   v    = edge.get_destin();

					/**
					 * Metro only connects, through the walking network,
					 * to bus stops. Now we have to account for each and
					 * every node that corresponds to a stop and route
					 * pair.
					*/
					for (int v_variant: bus.get_stop_variants(v)) {
						if (queue.contains(v_variant)) {
							float alt = metro_dist[metro_index(u)]  
								        + edge.get_attributes().get_time_taken() 
										+ transfer_penalty;
							if (alt < bus.get_dist(v_variant) && bus.get_extra(v_variant)!=WALK) {
								bus.mark_dist(v_variant, alt);
								bus.mark_prev(v_variant, u);
								bus.mark_extra(v_variant, WALK);
								queue.update(v_variant, alt);
							}
						} else if (explo.find(v_variant) == explo.end()) {
							/* v hasn't been visited before */
							bus.mark_dist(v_variant,
								metro_dist[metro_index(u)] 
								+ edge.get_attributes().get_time_taken() 
								+ transfer_penalty);
							bus.mark_prev(v_variant, u);
							bus.mark_extra(v_variant, WALK);
							queue.push(v_variant, bus.get_dist(v_variant));
						}
					}
				}
			}
		}

		// This will build the paths from the prev vector.
		// A path is built for each square, hence the complex
		// return type. It is basically the path constrution
		// bit in the Dijskstra's Algorithm but for every
		// path between the origin and a given square.
		auto result = std::vector<single_path_report>();
		for (auto& column: square_finale) {
			auto i = column.first;
			for (auto& line: column.second) {
				auto j = line.first;
				
				auto stk   = std::stack<int>();
				auto i_stk = std::stack<int>();
				auto c_stk = std::stack<float>();
				int u      = line.second;
				float cost;
				if (is_metro_node(u)) {
					cost = metro_dist[metro_index(u)];
				} else { cost = bus.get_dist(u); }


				if (cost!=0.0) {
					while (u != undefined) {
						stk.push(u);
						if (is_metro_node(u)) {
							i_stk.push(metro_extra[metro_index(u)]);
							c_stk.push(metro_dist[metro_index(u)]);
							u = metro_prev[metro_index(u)];
						} else {
							i_stk.push(bus.get_extra(u));
							c_stk.push(bus.get_dist(u));
							u = bus.get_prev(u);
						} 
					} 
				}

				auto path = std::vector<int>();
				while (!stk.empty()) {
					path.push_back(stk.top());
					stk.pop();
				}

				auto itineraries = std::vector<int>();
				while (!i_stk.empty()) {
					itineraries.push_back(i_stk.top());
					i_stk.pop();
				}

				auto cost_until = std::vector<float>();
				while (!c_stk.empty()) {
					cost_until.push_back(c_stk.top());
					c_stk.pop();
				}
				
				auto destin_sq   = std::pair<int, int>(i, j);
				auto this_report = single_path_report(
					cost,
					destin_sq,
					path,
					itineraries,
					cost_until
				);
				result.push_back(this_report);
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
	void grid::print_report(std::pair<int, int> origin, std::vector<single_path_report>& report) {
		int i = origin.first;
		int j = origin.second;
		for (auto& individual_report: report) {
			auto destin = individual_report.get_destin();
			std::cout << "(" << i << "," << j;
			std::cout << ") -> (" << destin.first;
			std::cout << "," << destin.second << ") | cost: ";
			std::cout << individual_report.get_cost() << " | path: ";
			for (auto node: individual_report.get_stopations()) {
				std::cout << node << " ";
			}
			std::cout << " | itineraries: ";
			for (auto iti: individual_report.get_itineraries()) {
				std::cout << iti << " ";
			}
			std::cout << std::endl;
		}
	}

	/**
	 * Run the path finding algorithm for every square as an origin.
	 * 	@param bus: the bus network to be used
	 * 	@param metro: the metro network
	*/
	network_usage grid::predict_all_od_pairs(
		bus_network& bus
	) {
		auto result = network_usage();
		int total_sq = get_total_squares();
		int counter  = 0;
		int total = total_sq * (total_sq-1); 
		for (auto& column: _squares) {
			int i = column.first;
			for (auto& line: column.second) {
				int j = line.first;
				auto report = best_path_between_all(
					std::pair<int, int>(i, j),
					bus
				);
				auto trips = trip_from_report(
					std::pair<int, int>(i, j),
					report, bus
				);
				result.add_usage_from(
					std::pair<int, int>(i, j),
					trips
				);
				counter += 1;
				if (configs::verbose) 
					print_progress_bar(counter, total_sq);
			}
		}

		return result;
	}

	std::vector<int> slicing(const std::vector<int>& arr, int x, int y) {
	
		// Starting and Ending iterators
		auto start = arr.begin() + x;
		auto end   = arr.begin() + y + 1;
	
		// To store the sliced vector
		std::vector<int> result(y - x + 1);
	
		// Copy vector using copy function()
		std::copy(start, end, result.begin());
	
		// Return the final sliced vector
		return result;
	}

	/**
	 * The output the comes out of best_path_between_all is
	 * a bit to raw. This function essentially refines that
	 * output into a more understandable format.
	 * 
	 * 	@param origin_sq: the pair (i,j) that identifies the origin
	 * 	@param report: what came out of best_path_between_all, and
	 * what we want to be refining
	 * 	@param bus: the bus network that the path was based on
	 * 	@param metro: the metro network that the path was based on
	 * 	@param walk: the walking network that the path was based on
	 * 
	 * 	@return a set of trips, which are a much nicer way to compute
	 * statistics for the bus network which is always the subject of
	 * evaluation in this problem.
	*/
	std::vector<trip>  grid::trip_from_report(
		std::pair<int, int> origin_sq,
		std::vector<single_path_report>& report,
		bus_network& bus
	) {
		auto trips = std::vector<trip>();
		for (auto& single_path: report) {
			auto destin_sq = single_path.get_destin();

			if (!odx_matrix::instance()->travel_needed_between(
				origin_sq, destin_sq
			)) { continue; }

			auto time = single_path.get_cost();
			const auto& itiniraries = single_path.get_itineraries(); 
			const auto& path = single_path.get_stopations();
			const auto& costs = single_path.get_costs_until();
			
			if (path.size() > 0) {
				auto stages = std::vector<stage>();
				int prev_itinerary = itiniraries.at(0);
				int stage_start = path.at(0);
				int stage_start_i = 0;
				for (int i=1; i<path.size(); i++) {
					int curr_itinerary = itiniraries.at(i);

					if ((curr_itinerary != prev_itinerary && 
						prev_itinerary != START) || i==path.size()-1) {
						
						int i_node = _node_to_index[path.at(i)];
						int start_mode = _node_modes[stage_start];
						int final_mode = _node_modes[path.at(i)];
						int stage_type = -1;
						if (prev_itinerary == WALK) {
							stage_type = WALKING_STAGE;
						} else if (!is_metro_node(path.at(i))) {
							stage_type = BUS_STAGE;
						} else {
							stage_type = METRO_STAGE;
						}
						auto this_stage = stage(
							stage_start,
							path.at(i),
							stage_type,
							(costs.at(i)-costs.at(stage_start_i)),
							prev_itinerary,
							slicing(path, stage_start_i, i)
						);
						stages.push_back(this_stage);
						stage_start   = path.at(i);
						stage_start_i = i; 
					}

					prev_itinerary = curr_itinerary;
				}

				auto this_trip = trip(
					stages, 
					odx_matrix::instance()->get_total(origin_sq, destin_sq), 
					path[0], 
					path[path.size()-1], 
					origin_sq,
					destin_sq
				);
				trips.push_back(this_trip);
				
			}
		}

		return trips;
	}

	inline bool grid::is_metro_node(int node) {
		// return (node>=0 && node<100);
		return metro_network::instance()->has_node(node);
	}

	grid* grid::_instance = nullptr;

} // namespace urban

#endif