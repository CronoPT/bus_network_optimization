#ifndef __LISBON_BUS_CPP__
#define __LISBON_BUS_CPP__

#include "lisbon_bus.hpp"

namespace urban {

	bus_network import_lisbon_bus(osm_net::osm_net& graph) {
		std::ifstream input_file(configs::stop_sequences);
		nlohmann::json json_routes = nlohmann::json::parse(input_file);

		std::vector<route> routes;
		int route_id = 0;
		for (auto& json_route: json_routes) {
			auto route_stops = std::vector<int>();
			for (auto stop: json_route["stops"]) {
				route_stops.push_back(stop);
			}
			routes.push_back(route(route_id, route_stops));
			route_id += 1;
		}

		std::cout << "Did you read all?" << std::endl;

		return bus_network(routes, graph);
	}

} // namespace urban

#endif