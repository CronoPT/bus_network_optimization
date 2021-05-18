#ifndef __BUS_NETWORK_CPP__
#define __BUS_NETWORK_CPP__

#include "bus_network.hpp"

namespace urban {

	/**
	 * This function isn't even a part of the bus_network class.
	 * It's purpose it's to compute the time a given path takes
	 * in the road network (uses the maxspeed parameter).
	 * We have to use this function because the paths themselves
	 * are shortest paths between bus stops but computed in terms
	 * of distance. 
	 * 
	 * 	@param path_report: the cost distance and path of the target 
	 * for which we want to compute the travel time.
	 * 	
	 * 	@return the time it takes to traverse the path in seconds.
	*/
	float compute_path_time(
		std::pair<std::vector<int>, float>& path_report
	) {
		float time  = 0; 
		auto& path = path_report.first; 
		int origin = -1;
		for (auto destin: path) {
			if (origin != -1) {
				auto& adjs = osm_net::osm_net::instance()->get_nodes()[origin].get_adjacencies();

				float min_cost = std::numeric_limits<float>::max();
				int min_key = -1;
				for (auto& adj: adjs) {
					if (adj.second.get_destin() == destin) {
						if (adj.second.get_attributes().get_length() < min_cost) {
							min_cost = adj.second.get_attributes().get_length();
							min_key  = adj.first;
						}
					}
				}
				float length = adjs[min_key].get_attributes().get_length();
				float speed  = adjs[min_key].get_attributes().get_max_speed();
				time += (length / (speed / 3.6));
			}	
			origin = destin;
		}
		return time;
	}

	/**
	 * The main constructor for the bus_network class.
	 * It adds every bus stop as a node to the underlying
	 * graph and then computes the edges between those
	 * nodes given the sequence of stops in a set of
	 * routes.
	 * 
	 * 	@param routes: the set of routes that comprise the
	 * network.
	*/
	bus_network::bus_network(std::vector<route>& routes):
	 _evaluated(false),
	 _statics_computed(false),
	 _transfers(0),
	 _in_vehicle_time(0),
	 _total_length(0),
	 _unsatisfied_demand(0),
	 _number_routes(0),
	 _shortest_route(0),
	 _longest_route(0),
	 _routes(),
	 graph() {

		if (!_stops_loaded) {
			/* Loads the json file to all bus_networks only once */
			std::ifstream input_file(configs::stop_locations);
			_bus_stops = nlohmann::json::parse(input_file);
			_stops_loaded = true;
		}

		for (auto& stop: bus_network::_bus_stops) {
			/* Add bus stops as nodes */
			add_node(stop["stop_id"], bus_node(
				stop["stop_id"],
				stop["point"][0],
				stop["point"][1]
			));
		}

		for (auto& route: routes) {
			/* Add edges based on stop sequences */
			add_route(route);
		}

	}

	/**
	 * Adds all the edges implied by a given route.
	 * Remember, the cost of the edges is in seconds
	 * and it is the time it takes to go between
	 * two stops through the road network.
	 * 
	 * 	@param r: the route to build around
	*/
	void bus_network::embed_route(route& r) {
		int route_id   = r.get_route_id();
		auto& sequence = r.get_stop_sequence();

		int origin_stop = -1;
		for (auto destin_stop: sequence) {
			if (origin_stop != -1) {
				auto path_report = osm_net::osm_net::instance()->dijkstra(
					origin_stop,
					destin_stop,
					[](net::edge<osm_net::osm_edge>& e) -> float {
						auto length = e.get_attributes().get_length();
						auto speed  = e.get_attributes().get_max_speed();
						return (float) length;
					}
				);

				auto travel_time = compute_path_time(path_report);
				add_edge(origin_stop, destin_stop, bus_edge(
					origin_stop, 
					destin_stop, 
					route_id, 
					travel_time
				));
			}
			origin_stop = destin_stop;
		}
	}

	/**
	 * This is an empty constructor for when
	 * the bus_networks need to be stored in
	 * standard library containers.
	*/
	bus_network::bus_network():
	 _evaluated(false),
	 _statics_computed(false),
	 _transfers(0),
	 _in_vehicle_time(0),
	 _total_length(0),
	 _unsatisfied_demand(0),
	 _number_routes(0),
	 _shortest_route(0),
	 _longest_route(0),
	 _routes(),
	 graph() { /* Do Nothing */ }

	/**
	 * Some bus_network attributes are the product
	 * of relatively expensive computations, so the
	 * evaluations of those attributes is done in a
	 * lazy fation. That's why most getters have an
	 * if statment before actually returning the 
	 * attribute (it needs to be computed first).
	 * 
	 * In this laxy evaluation we distinguish two types.
	 * - Dynamic Evaluations: need to use the road
	 * and the whole transportation infrastructure to
	 * be computed. More expensive.
	 * - Static Evaluations: computed without the need
	 * to resort to any other structure. Faster.
	*/

	bool bus_network::evaluated() const {
		return _evaluated;
	}

	/** Needs Dynamic Evaluations */
	float bus_network::get_transfers() {
		if (!_evaluated) {
			evaluate();
		}
		return _transfers;
	}

	/** Needs Dynamic Evaluations */
	float bus_network::get_in_vehicle_time() {
		if (!_evaluated) {
			evaluate();
		}
		return _in_vehicle_time;
	}

	/** Needs Static Evaluations */
	float bus_network::get_total_length() {
		if (!_statics_computed) {
			static_computes();
		}
		return _total_length;
	}

	/** Needs Dynamic Evaluations */
	float bus_network::get_unsatisfied_demand() {
		if (!_evaluated) {
			evaluate();
		}
		return _unsatisfied_demand;
	}

	/** No need for lazy computations */
	int bus_network::get_number_routes() const {
		return _routes.size();
	}

	/** Needs Dynamic Evaluations */
	int bus_network::get_shortest_route() {
		if (!_statics_computed) {
			static_computes();
		}
		return _shortest_route;
	}

	/** Needs Static Evaluations */
	int bus_network::get_longest_route() {
		if (!_statics_computed) {
			static_computes();
		}
		return _longest_route;
	}

	/** No need for lazy computations */
	const std::vector<route> bus_network::get_routes() const {
		return _routes;
	}

	/**
	 * A bus_network should be a read-only object. However,
	 * in a genetic algorithm, adding and removing routes
	 * is part of the mutation process.This function adds
	 * a new route, then signals the object that a new
	 * evaluation is needed since the network changed.
	 * 
	 * @param new_route: the route to add to the network
	*/
	void bus_network::add_route(route& new_route) {
		/* Add needed edges */
		embed_route(new_route);

		/* Add the route to the vector of routes */
		_routes.push_back(new_route);

		/* Add the route id to the route id set */
		_route_check.insert(new_route.get_route_id());

		/* Flag changes in the object */
		_evaluated = false;
		_statics_computed = false;
	}

	/**
	 * A bus_network should be a read-only object. However,
	 * in a genetic algorithm, adding and removing routes
	 * is part of the mutation process.This function removes
	 * an existing route, then signals the object that a new
	 * evaluation is needed since the network changed.
	 * 
	 * @param position: the position, in the vector of routes,
	 * of the route to remove. This parameter is an index and
	 * not a route identifier because this is being used as part
	 * of a genetic algorithm. An identifier varient can be
	 * developed.
	*/
	void bus_network::delete_route(int position) {
		auto& to_delete = _routes.at(position);
		int route_id = to_delete.get_route_id();
		int origin_stop = -1;
		for (auto destin_stop: to_delete.get_stop_sequence()) {
			/* Remove all the edges that exist because of this route */
			if (origin_stop != -1) {
				int link_id = -1;
				for (auto& adj: get_nodes()[origin_stop].get_adjacencies()) {
					/* Find link that represents this particular connection */
					if (adj.second.get_attributes().get_route_id() == route_id &&
					 adj.second.get_destin() == destin_stop) {
						link_id = adj.second.get_id();
						break;
					}
				}
				remove_edge(
					origin_stop, 
					destin_stop,
					link_id
				);
			}
			origin_stop = destin_stop;
		}
		
		/* Remove the route from the vector of routes */
		_routes.erase(_routes.begin()+position);

		/* Remove the route id from the route id set */
		auto it = _route_check.find(route_id);
		_route_check.erase(it);

		/* Flag changes in the object */
		_evaluated = false;
		_statics_computed = false;
	}

	/**
	 * A simple function to quickly verify if a bus_network
	 * already has a certain route in its configuration.
	 * 
	 * 	@param route_id: the route whose presence we check for
	 * 
	 * 	@return whether the route exists in this network or not
	*/
	bool bus_network::has_route(int route_id) {
		return _route_check.find(route_id) != _route_check.end();
	}

	/**
	 * Make the Static Evalutions mentioned above.
	 * Computes the total length of the bus_network
	 * as well as the shortest and longest route
	 * (in terms of number of stops).
	*/
	void bus_network::static_computes() {
		_total_length = 0;
		_shortest_route = std::numeric_limits<int>::max();
		_longest_route  = std::numeric_limits<int>::min();

		for (auto& route: _routes) {
			int route_id   = route.get_route_id();
			auto& sequence = route.get_stop_sequence();
			
			if (route.size() > _longest_route) {
				_longest_route = route.size();
			}
			if (route.size() < _shortest_route) {
				_shortest_route = route.size();
			}

			int origin_stop = -1;
			for (auto destin_stop: sequence) {
				/* Compute total length of the network */
				if (origin_stop != -1) {
					auto path_report = osm_net::osm_net::instance()->dijkstra(
						origin_stop,
						destin_stop,
						[](net::edge<osm_net::osm_edge>& e) -> float {
							auto length = e.get_attributes().get_length();
							auto speed  = e.get_attributes().get_max_speed();
							return (float) length;
						}
					);

					_total_length += path_report.second;
				}
				origin_stop = destin_stop;
			}
		}

		_number_routes = _routes.size();

		/* Flag the Static Computations as being done */
		_statics_computed = true;
	}

	/**
	 * Make the Dynamic Evalutions mentioned above.
	 * Computes the unsatisfied demand, the average 
	 * number of transfers per passenger and the
	 * total in-vehicle-time amongst all the 
	 * passengers. Uses the urban::grid to predict
	 * how the network would be used by the passengers
	 * travelling between the different od-pairs 
	 * present in the urban::odx_matrix.
	*/
	void bus_network::evaluate() {
		_unsatisfied_demand = 0;
		_transfers = 0;
		_in_vehicle_time = 0;

		int no_trips = 0;
		int total = 0;

		auto& pairs = odx_matrix::instance()->get_all_pairs();
		auto  usage = grid::instance()->predict_all_od_pairs(*this);
		int total_passengers  = 0;
		
		for (auto& od_pair: pairs) {
			auto origin = od_pair.first;
			auto destin = od_pair.second;

			auto& use = usage.get_usage_between(origin, destin);
			int passengers = odx_matrix::instance()->get_total(origin, destin);
			total_passengers += passengers;

			if (origin.first  == destin.first && 
			    origin.second == destin.second) { 
				continue; 
			}

			if (use.get_stages().size() == 0) {
				// if (_routes.size() == 309) {
				std::cout << "Found no path between: (";
				std::cout << origin.first << "," << origin.second << ") -> (";
				std::cout << destin.first << "," << destin.second << ")";
				std::cout << std::endl;
				// }
				_unsatisfied_demand += passengers;
				no_trips += 1;

				continue;
			}

			_transfers += use.get_transfers()*passengers;
			total += use.get_transfers();

			for (auto& s: use.get_stages()) {
				if (s.get_mode() != WALKING_STAGE) {
					_in_vehicle_time += s.get_time() * passengers;
				}
			}
		}

		std::cout << "Total Passengers: " << total_passengers << std::endl;
		std::cout << "No trips: " << _unsatisfied_demand << std::endl;
		std::cout << "No trips (pairs): " << no_trips << std::endl;
		std::cout << "Raw transfers: " << _transfers << std::endl;
		std::cout << "Transfers (no multi): " << total << std::endl;

		_transfers /= total_passengers;
		_unsatisfied_demand /= total_passengers;

		/* Flag the Dynamic Computations as being done */
		_evaluated = true;

		// std::cout << "{transfers: "  << _transfers;
		// std::cout << ", un_demand: " << _unsatisfied_demand;
		// std::cout << ", total_passengers: " << total_passengers;
		// std::cout << ", routes: " << _routes.size() << "}" << std::endl;
	}

	/**
	 * Print a small summary of the network to the console.
	*/
	std::ostream& operator<<(std::ostream& os, const bus_network& s) {
		os << "Bus Network: { number_routes: ";
		os << s.get_number_routes() << "}\n";
		return os;
	}

	/**
	 * In the beginning the file with the stop locations
	 * still needs to be loaded.
	*/
	bool bus_network::_stops_loaded = false;

	/** 
	 * Initialize the object that will holf all the
	 * stop locations with an empty json object.
	*/
	nlohmann::json bus_network::_bus_stops = nlohmann::json();

} // namespace urban

#endif