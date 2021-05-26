#ifndef __ROUTE_POOL_CPP__
#define __ROUTE_POOL_CPP__

#include "route_pool.hpp"

namespace transit_problem {

	bool is_tram_route(std::string route_id) {
		for (auto& tram: tndp_configs::tram_routes) {
			if (route_id.find(tram) != std::string::npos) {
				return true;
			}
		}
		return false;
	}

	route_pool::route_pool(): _routes(), _mandatory_routes() {
		std::ifstream input_file(tndp_configs::og_routes);
		nlohmann::json json_routes = nlohmann::json::parse(input_file);

		int route_id = 1;
		for (auto& json_route: json_routes) {
			auto route_stops = std::vector<int>();
			for (auto stop: json_route["stops"]) {
				route_stops.push_back(stop);
			}
			if (is_tram_route(json_route["route_id"])) {
				_mandatory_routes.push_back(urban::route(route_id, route_stops));
			} else { _routes.push_back(urban::route(route_id, route_stops)); }
			
			route_id += 1;
		}

		input_file  = std::ifstream(tndp_configs::ge_routes);
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

	urban::route& route_pool::at(int i) {
		return _routes.at(i);
	}

	route_pool* route_pool::instance() {
		if (!_instance) {
			_instance = new route_pool();
		}
		return _instance;
	}

	const std::vector<urban::route>& route_pool::get_mandatory_routes() const {
		return _mandatory_routes;
	}

	int route_pool::get_number_mandatory() const {
		return _mandatory_routes.size();
	}

	route_pool* route_pool::_instance = nullptr;

}

#endif