#include <bus_edge.hpp>
#include <bus_network.hpp>
#include <bus_node.hpp>
#include <lisbon_bus.hpp>
#include <metro_edge.hpp>
#include <metro_network.hpp>
#include <metro_node.hpp>
#include <grid.hpp>
#include <route.hpp>
#include <walking_edge.hpp>
#include <walking_network.hpp>
#include <walking_node.hpp>
#include <odx_matrix.hpp>
#include <tndp.hpp>
#include <route_pool.hpp>
#include <limits>
#include <sstream>

#include <chrono>
#include <iostream>

int main() {
	std::ifstream input_file("../data/json/run_nets.json");
	nlohmann::json json = nlohmann::json::parse(input_file);

	std::vector<urban::bus_network> nets;
	for (auto& network: json) {
		std::vector<urban::route> routes;
		int route_id = 1;
		for (auto& json_route: network["routes"]) {
			auto route_stops = std::vector<int>();
			for (auto stop: json_route) {
				route_stops.push_back(stop);
			}
			routes.push_back(urban::route(route_id, route_stops));
			route_id += 1;
		}
		nets.push_back(urban::bus_network(routes));
	}
	
	std::vector<float> averages;
	for (auto& net: nets) {
		averages.push_back(net.get_total_length()/net.get_number_routes());
	}

	std::cout << "[\n";
	for (int i=0; i<averages.size(); i++) {
		std::cout << "\t" << averages.at(i);
		if (i<averages.size()-1) {
			std::cout << ",";
		}
		std::cout << "\n";
	}
	std::cout << "]" << std::endl;

	std::cout << "[\n";
	for (int i=0; i<nets.size(); i++) {
		std::cout << "\t" << nets.at(i).get_number_routes();
		if (i<nets.size()-1) {
			std::cout << ",";
		}
		std::cout << "\n";
	}
	std::cout << "]" << std::endl;
}