#ifndef __BUS_NETWORK_CPP__
#define __BUS_NETWORK_CPP__

#include "bus_network.hpp"

namespace urban {

	bus_network::bus_network(std::vector<std::vector<int>> stop_sequences,
	            road_graph road_network): _graph() {
		
		for (auto& route: stop_sequences) {
			int origin_stop = -1;
			for (auto destin_stop: route) {
				if (origin_stop != -1) {
					auto path_report = road_network.dijkstra(
						origin_stop,
						destin_stop,
						[](net::edge<osm_net::osm_edge>& e) -> float {
							return (float) e.get_attributes().get_length();
						}
					);
					auto distance = path_report.second;
					_graph.add_edge(origin_stop, destin_stop, osm_net::osm_edge(
						origin_stop, destin_stop, distance
					));
				}
				origin_stop = destin_stop;
			}
		}

	}

} // namespace urban

#endif