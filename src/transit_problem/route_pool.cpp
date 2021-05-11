#ifndef __ROUTE_POOL_CPP__
#define __ROUTE_POOL_CPP__

#include "route_pool.hpp"

namespace transit_problem {

	const std::string route_pool::og_routes = 
		"../data/json/clustered_routes_stop_sequence.json"; 

	const std::string route_pool::ge_routes = 
		"../data/json/route_pool.json"; 

	route_pool::route_pool(): _routes() {
		std::ifstream input_file(route_pool::og_routes);
		nlohmann::json json_routes = nlohmann::json::parse(input_file);

		int route_id = 0;
		for (auto& json_route: json_routes) {
			auto route_stops = std::vector<int>();
			for (auto stop: json_route["stops"]) {
				route_stops.push_back(stop);
			}
			_routes.push_back(urban::route(route_id, route_stops));
			route_id += 1;
		}

		input_file  = std::ifstream(route_pool::ge_routes);
		json_routes = nlohmann::json::parse(input_file);
		for (auto& json_route: json_routes) {
			auto route_stops = std::vector<int>();
			for (auto stop: json_route) {
				route_stops.push_back(stop);
			}
			_routes.push_back(urban::route(route_id, route_stops));
			route_id += 1;
		}
	}

	int route_pool::size() const { 
		return _routes.size(); 
	}

	urban::route route_pool::at(int i) const {
		return _routes.at(i);
	}

	route_pool* route_pool::instance() {
		if (!_instance) {
			_instance = new route_pool();
		}
		return _instance;
	}

	route_pool* route_pool::_instance = nullptr;

}

#endif