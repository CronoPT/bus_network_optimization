#ifndef __BUS_NETWORK_CPP__
#define __BUS_NETWORK_CPP__

#include "bus_network.hpp"

namespace urban {

	bus_network::bus_network(
		std::vector<route> routes,
	    osm_net::osm_net road_network
	): graph() {
		std::ifstream input_file("../data/json/clustered_stop_locations.json");
		nlohmann::json bus_stops = nlohmann::json::parse(input_file);

		for (auto& stop: bus_stops) {
			add_node(stop["stop_id"], bus_node(
				stop["stop_id"],
				stop["point"][0],
				stop["point"][1]
			));
		}
		
		for (auto& route: routes) {
			int route_id   = route.get_route_id();
			auto& sequence = route.get_stop_sequence();

			int origin_stop = -1;
			for (auto destin_stop: sequence) {
				if (origin_stop != -1) {
					auto path_report = road_network.dijkstra(
						origin_stop,
						destin_stop,
						[](net::edge<osm_net::osm_edge>& e) -> float {
							auto length = e.get_attributes().get_length();
							auto speed  = e.get_attributes().get_max_speed();
							return (float) (length / (speed / 3.6));
							// return length;
						}
					);
					auto travel_time = path_report.second;
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

	}

} // namespace urban

#endif