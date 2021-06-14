#ifndef __BUS_NETWORK_CPP__
#define __BUS_NETWORK_CPP__

#include "bus_network.hpp"

namespace urban {

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
	 _stop_in_routes(),
	 _stop_ids(),
	 _usage(),
	 graph() {

		if (!_stops_loaded) {
			/* Loads the json file to all bus_networks only once */
			load_stops();
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
	void bus_network::embed_route(const route& r) {

		int route_id = r.get_route_id();

		for (auto& e: r.get_edge_info()) {
			embed_node(e.get_origin(), route_id);
			embed_node(e.get_destin(), route_id);

			int o_node_id = node_id(e.get_origin(), route_id);
			int d_node_id = node_id(e.get_destin(), route_id);

			add_edge(
				o_node_id,
				d_node_id,
				bus_edge(
					e.get_origin(),
					e.get_destin(),
					route_id,
					e.get_time()
				)
			);
		}

	}

	void bus_network::embed_node(int stop_id, int route_id) {
		int id = node_id(stop_id, route_id);

		if (!has_node(id)) {
			add_node(id, bus_node(
				stop_id,
				_bus_stops[stop_id].first,
				_bus_stops[stop_id].second
			));
		

			for (int r: _stop_in_routes[stop_id]) {
				int other_id = node_id(stop_id, r);
				add_edge(id, other_id, bus_edge(
					stop_id,
					stop_id,
					-1,
					10*60
				));
				add_edge(other_id, id, bus_edge(
					stop_id,
					stop_id,
					-1,
					10*60
				));
			}

			_stop_in_routes[stop_id].insert(route_id);
			_stop_ids[stop_id].push_back(id);
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
	 _stop_in_routes(),
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

	network_usage bus_network::get_usage() {
		if (!_evaluated) {
			evaluate();
		}
		return _usage;
	}

	/** No need for lazy computations */
	const std::vector<route>& bus_network::get_routes() const {
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
	void bus_network::add_route(const route& new_route) {
		/**
		 * This avoids having to make a great number of checks
		 * while crossing over two bus_networks that might share
		 * a route.
		*/
		if (has_route(new_route.get_route_id())) {
			return;
		}
		
		/* Add needed edges */
		embed_route(new_route);

		_total_length += new_route.get_route_length();

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

		for (auto& e: to_delete.get_edge_info()) {
			int origin_stop = e.get_origin();
			int destin_stop = e.get_destin();

			int o_node_id = node_id(origin_stop, route_id);
			int d_node_id = node_id(destin_stop, route_id);

			int link_id = -1;
			for (auto& adj: get_nodes()[o_node_id].get_adjacencies()) {
				/* Find link that represents this particular connection */
				if (adj.second.get_attributes().get_route_id() == route_id &&
					adj.second.get_destin() == d_node_id) {
					link_id = adj.second.get_id();
					break;
				}
			}
			remove_edge(
				o_node_id, 
				d_node_id,
				link_id
			);
		}

		for (int stop: to_delete.get_stop_sequence()) {
			delete_node(stop, route_id);
		}

		_total_length -= to_delete.get_route_length();

		/* Remove the route from the vector of routes */
		_routes.erase(_routes.begin()+position);

		/* Remove the route id from the route id set */
		_route_check.erase(route_id);

		/* Flag changes in the object */
		_evaluated = false;
		_statics_computed = false;
	}

	void bus_network::delete_node(int stop_id, int route_id) {
		int id = node_id(stop_id, route_id);

		/**
		 * This has to be here for routes that
		 * have a repeated station. If we remove it,
		 * the erase bellow will cause a seg fault
		 * error.
		*/
		if (!has_node(id)) { return; }

		_stop_in_routes[stop_id].erase(route_id);
		auto it = std::find(
			_stop_ids[stop_id].begin(), 
			_stop_ids[stop_id].end(), 
			id
		);

		/**
		 * ! WARNING !
		 * If, for some reason, the genetic operators
		 * introduce the same route several times in
		 * the same bus network and it is removed more 
		 * than once, this will throw a Segmentation
		 * Fault. The guard in the beginning of
		 * bus_network::add_route(route) should prevent
		 * this, but if the guard is removed, this 
		 * line can be a source of problems.
		*/
		_stop_ids[stop_id].erase(it);

		for (int r: _stop_in_routes[stop_id]) {
			int other_id = node_id(stop_id, r);
			int link_id = -1;
			for (auto& adj: get_nodes()[id].get_adjacencies()) {
				/* Find link that represents this particular connection */
				if (adj.second.get_destin() == other_id) {
					link_id = adj.second.get_id();
					break;
				}
			}
			remove_edge(
				id, 
				other_id,
				link_id
			);

			link_id = -1;
			for (auto& adj: get_nodes()[other_id].get_adjacencies()) {
				/* Find link that represents this particular connection */
				if (adj.second.get_destin() == id) {
					link_id = adj.second.get_id();
					break;
				}
			}
			remove_edge(
				other_id, 
				id,
				link_id
			);
		}

		remove_node(id);
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
		return (_route_check.find(route_id) != _route_check.end());
	}

	std::vector<int>& bus_network::get_stop_variants(int stop_id) {
		return _stop_ids[stop_id];
	}

	/**
	 * Make the Static Evalutions mentioned above.
	 * Computes the total length of the bus_network
	 * as well as the shortest and longest route
	 * (in terms of number of stops).
	*/
	void bus_network::static_computes() {
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
		_usage = grid::instance()->predict_all_od_pairs(*this);
		float total_passengers  = 0;
		
		for (auto& od_pair: pairs) {
			auto origin = od_pair.first;
			auto destin = od_pair.second;

			auto& use = _usage.get_usage_between(origin, destin);
			float passengers = odx_matrix::instance()->get_total(origin, destin);
			total_passengers += passengers;

			if (origin.first  == destin.first && 
			    origin.second == destin.second) { 
				continue; 
			}

			if (use.get_stages().size() == 0) {
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

		_transfers /= total_passengers;
		_unsatisfied_demand /= total_passengers;

		/* Flag the Dynamic Computations as being done */
		_evaluated = true;
	}

	int bus_network::node_id(int stop_id, int route_id) {
		const int base = 100000;
		return route_id*base + stop_id;
	}

	std::pair<int, int> bus_network::node_id(int node_id) {
		const int base = 100000;
		int stop_id  = node_id % base;
		int route_id = node_id / base;
		return std::pair<int, int>(stop_id, route_id); 
	}	

	std::unordered_map<int, std::pair<double, double>> bus_network::get_stops() {
		if (!_stops_loaded) {
			load_stops();
		}
		return _bus_stops;
	}

	void bus_network::load_stops() {
		std::ifstream input_file(configs::stop_locations);
		nlohmann::json json = nlohmann::json::parse(input_file);
		for (auto& item: json) {
			_bus_stops[item["stop_id"]] = std::pair<double, double>(
				item["point"][0], item["point"][1]
			);
		} 
		_stops_loaded = true;
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
	std::unordered_map<int, std::pair<double, double>> 
		bus_network::_bus_stops = std::unordered_map<int, std::pair<double, double>>();

} // namespace urban

#endif