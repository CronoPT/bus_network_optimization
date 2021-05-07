#ifndef __BUS_NETWORK_CPP__
#define __BUS_NETWORK_CPP__

#include "bus_network.hpp"

namespace urban {

	float compute_path_time(
		osm_net::osm_net& road_network, 
		std::pair<std::vector<int>, float>& path_report
	) {
		float time  = 0; 
		auto& path = path_report.first; 
		int origin = -1;
		for (auto destin: path) {
			if (origin != -1) {
				auto& adjs = road_network.get_nodes()[origin].get_adjacencies();

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

	bus_network::bus_network(
		std::vector<route> routes,
	    osm_net::osm_net road_network
	):
	 _evaluated(false),
	 _transfers(0),
	 _in_vehicle_time(0),
	 _total_length(0),
	 _unsatisfied_demand(0),
	 _number_routes(0),
	 _shortest_route(0),
	 _longest_route(0),
	 graph() {
		std::ifstream input_file(configs::stop_locations);
		nlohmann::json bus_stops = nlohmann::json::parse(input_file);

		for (auto& stop: bus_stops) {
			add_node(stop["stop_id"], bus_node(
				stop["stop_id"],
				stop["point"][0],
				stop["point"][1]
			));
		}
		
		_shortest_route = std::numeric_limits<int>::max();
		_longest_route  = std::numeric_limits<int>::min();

		for (auto& route: routes) {
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
					auto path_report = road_network.dijkstra(
						origin_stop,
						destin_stop,
						[](net::edge<osm_net::osm_edge>& e) -> float {
							auto length = e.get_attributes().get_length();
							auto speed  = e.get_attributes().get_max_speed();
							return (float) length;
						}
					);

					auto travel_time = compute_path_time(road_network, path_report);
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

		_number_routes = routes.size();
	}

	bool bus_network::evaluated() const {
		return _evaluated;
	}

	int bus_network::get_transfers() const {
		return _transfers;
	}

	float bus_network::get_in_vehicle_time() const {
		return _in_vehicle_time;
	}

	float bus_network::get_total_length() const {
		return _total_length;
	}

	float bus_network::get_unsatisfied_demand() const {
		return _unsatisfied_demand;
	}

	int bus_network::get_number_routes() const {
		return _number_routes;
	}

	int bus_network::get_shortest_route() const {
		return _shortest_route;
	}

	int bus_network::get_longest_route() const {
		return _longest_route;
	}

	void bus_network::evaluate(
		network_usage usage, 
		metro_network metro, 
		walking_network walk,
		odx_matrix odx
	) {

		auto& pairs = odx.get_all_pairs();
		int unsatisfied_pairs = 0;
		
		for (auto& od_pair: pairs) {
			auto origin = od_pair.first;
			auto destin = od_pair.second;

			auto& use = usage.get_usage_between(origin, destin);

			if (use.get_stages().size() == 0) {
				unsatisfied_pairs += 1;
				continue;
			}

			int passengers = odx.get_total(origin, destin);
			_transfers += use.get_transfers()*passengers;

			for (auto& s: use.get_stages()) {
				if (s.get_mode() != WALKING_STAGE) {
					_in_vehicle_time += s.get_time() * passengers;
				}
			}
		}

		_unsatisfied_demand = unsatisfied_pairs/pairs.size();
		_evaluated = true;
	}

} // namespace urban

#endif