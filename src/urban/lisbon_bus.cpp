#ifndef __LISBON_BUS_CPP__
#define __LISBON_BUS_CPP__

#include "lisbon_bus.hpp"

namespace urban {

	bus_network import_lisbon_bus(road_graph graph) {
		std::ifstream input_file(
			"../data/json/clustered_routes_stop_sequence.json"
		);
		nlohmann::json json_routes = nlohmann::json::parse(input_file);

		std::vector<std::vector<int>> routes;
		for (auto& json_route: json_routes) {
			auto route = std::vector<int>();
			for (auto stop: json_route['stops']) {
				route.push_back(stop);
			}
			routes.push_back(route);
		}

		return bus_network(routes, graph);
	}

} // namespace urban

#endif