#ifndef __LISBON_BUS_CPP__
#define __LISBON_BUS_CPP__

#include "lisbon_bus.hpp"

namespace urban {

	bus_network lisbon_bus::import_lisbon_bus() {
		std::ifstream input_file(configs::stop_sequences);
		nlohmann::json json_routes = nlohmann::json::parse(input_file);

		std::vector<route> routes;
		int route_id = 1;
		for (auto& json_route: json_routes) {
			auto route_stops = std::vector<int>();
			for (auto stop: json_route["stops"]) {
				route_stops.push_back(stop);
			}
			routes.push_back(route(route_id, route_stops));
			route_id += 1;
		}
		
		return bus_network(routes);
	}
	
	bus_network* lisbon_bus::instance() {
		if (!_initialized) {
			_initialized = true;
			_instance = import_lisbon_bus();
		}
		return &_instance;
	}

	bus_network lisbon_bus::_instance = bus_network();
	bool lisbon_bus::_initialized = false;

} // namespace urban

#endif