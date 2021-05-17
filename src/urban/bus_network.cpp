#ifndef __BUS_NETWORK_CPP__
#define __BUS_NETWORK_CPP__

#include "bus_network.hpp"

namespace urban {

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
			std::ifstream input_file(configs::stop_locations);
			_bus_stops = nlohmann::json::parse(input_file);
			_stops_loaded = true;
		}

		// std::cout << "Bad alloc, where are you? 1" << std::endl;

		for (auto& stop: bus_network::_bus_stops) {
			add_node(stop["stop_id"], bus_node(
				stop["stop_id"],
				stop["point"][0],
				stop["point"][1]
			));
		}

		// std::cout << "Bad alloc, where are you? 2" << std::endl;

		for (auto& route: routes) {
			// std::cout << "--------------------------" << std::endl;
			add_route(route);
			// std::cout << "Vat is happening?" << std::endl;
		}


		// std::cout << "Bad alloc, where are you? 3" << std::endl;
	}

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

	bool bus_network::evaluated() const {
		return _evaluated;
	}

	float bus_network::get_transfers() {
		if (!_evaluated) {
			evaluate();
		}
		// std::cout << "Get Transfers" << std::endl;
		return _transfers;
	}

	float bus_network::get_in_vehicle_time() {
		if (!_evaluated) {
			evaluate();
		}
		// std::cout << "In Vehicle" << std::endl;
		return _in_vehicle_time;
	}

	float bus_network::get_total_length() {
		if (!_statics_computed) {
			static_computes();
		}
		// std::cout << "Total Length" << std::endl;
		return _total_length;
	}

	float bus_network::get_unsatisfied_demand() {
		if (!_evaluated) {
			evaluate();
		}
		// std::cout << "Unsatisfied Demand" << std::endl;
		return _unsatisfied_demand;
	}

	int bus_network::get_number_routes() const {
		// std::cout << "Get Number of Routes" << std::endl;
		return _routes.size();
	}

	int bus_network::get_shortest_route() {
		if (!_statics_computed) {
			static_computes();
		}
		// std::cout << "Get Shortest Route" << std::endl;
		return _shortest_route;
	}

	int bus_network::get_longest_route() {
		if (!_statics_computed) {
			static_computes();
		}
		// std::cout << "Get Longest Route" << std::endl;
		return _longest_route;
	}

	const std::vector<route> bus_network::get_routes() const {
		return _routes;
	}

	void bus_network::add_route(route& new_route) {
		// std::cout << "Bad alloc, where are you? 4" << std::endl;
		embed_route(new_route);
		// std::cout << "Bad alloc, where are you? 5" << std::endl;
		_routes.push_back(new_route);
		// std::cout << "Bad alloc, where are you? 6" << std::endl;
		_route_check.insert(new_route.get_route_id());
		// std::cout << "Bad alloc, where are you? 7" << std::endl;
		_evaluated = false;
		_statics_computed = false;
		// std::cout << "Bad alloc, where are you? 8" << std::endl;
	}

	void bus_network::delete_route(int position) {
		auto& to_delete = _routes.at(position);
		int id = to_delete.get_route_id();
		int origin_stop = -1;
		for (auto destin_stop: to_delete.get_stop_sequence()) {
			if (origin_stop != -1) {
				int link_id = -1;
				for (auto& adj: get_nodes()[origin_stop].get_adjacencies()) {
					if (adj.second.get_attributes().get_route_id() == id &&
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
		
		_routes.erase(_routes.begin()+position);
		auto it = _route_check.find(id);
		_route_check.erase(id);
		_evaluated = false;
		_statics_computed = false;
	}

	bool bus_network::has_route(int route_id) {
		return _route_check.find(route_id) != _route_check.end();
	}

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
		_statics_computed = true;
	}

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
		_evaluated = true;

		// std::cout << "{transfers: "  << _transfers;
		// std::cout << ", un_demand: " << _unsatisfied_demand;
		// std::cout << ", total_passengers: " << total_passengers;
		// std::cout << ", routes: " << _routes.size() << "}" << std::endl;
	}

	std::ostream& operator<<(std::ostream& os, const bus_network& s) {
		os << "Bus Network: { number_routes: ";
		os << s.get_number_routes() << "}\n";
		return os;
	}

	bool bus_network::_stops_loaded = false;
	nlohmann::json bus_network::_bus_stops = nlohmann::json();

} // namespace urban

#endif