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

#include <chrono>
#include <iostream>

urban::bus_network read_generate_bus() {
	std::ifstream input_file("../data/json/run_nets.json");
	nlohmann::json json = nlohmann::json::parse(input_file);
	
	int index = 1; // number of the generated route
	std::vector<urban::route> routes;
	int route_id = 1;
	for (auto& json_route: json[index]["routes"]) {
		auto route_stops = std::vector<int>();
		for (auto stop: json_route) {
			route_stops.push_back(stop);
		}
		routes.push_back(urban::route(route_id, route_stops));
		route_id += 1;
	}
	
	return urban::bus_network(routes);
}

int main() {
	urban::bus_network original_bus = *urban::lisbon_bus::instance();
	urban::bus_network generate_bus = read_generate_bus();

	std::cout << original_bus.get_unsatisfied_demand() << std::endl;
	std::cout << generate_bus.get_unsatisfied_demand() << std::endl;

	urban::network_usage original_use = original_bus.get_usage();
	urban::network_usage generate_use = generate_bus.get_usage();

	float total_passengers = 0;
	float unsatisfied = 0;

	std::ofstream file("../data/json/comparison.json");
	int i = 0;
	int size = urban::odx_matrix::instance()->get_all_pairs().size();
	file << "[\n";
	for (auto& pair : urban::odx_matrix::instance()->get_all_pairs()) {
		auto origin = pair.first;
		auto destin = pair.second;

		if (origin == destin) {
			i = i+1;
			continue;
		}

		urban::trip original_trip = original_use.get_usage_between(origin, destin);
		urban::trip generate_trip = generate_use.get_usage_between(origin, destin); 

		float passengers = urban::odx_matrix::instance()->get_total(origin, destin);
		total_passengers += passengers;
		if (passengers == 0) {
			i = i+1;
			continue;
		}

		int original_transfers = original_trip.get_transfers();
		int generate_transfers = generate_trip.get_transfers();

		float original_time = original_trip.get_time();
		float generate_time = generate_trip.get_time();

		if (generate_trip.get_stages().size() == 0) {
			unsatisfied += passengers;
		}

		file << "\t{\n";
		file << "\t\t\"origin\": [";
		file << origin.first << ", " << origin.second << "],\n";
		file << "\t\t\"destin\": [";
		file << destin.first << ", " << destin.second << "],\n";
		file << "\t\t\"passengers\": " << passengers << ",\n";
		file << "\t\t\"original_transfers\": " << original_transfers << ",\n";
		file << "\t\t\"generate_transfers\": " << generate_transfers << ",\n";
		file << "\t\t\"original_travel_time\": " << original_time << ",\n";
		file << "\t\t\"generate_travel_time\": " << generate_time << "\n";
		file << "\t}";
		if (i<size-1) { file << ","; }
		file << "\n";
		i = i+1;
	}
	file << "]\n";

	std::cout << (unsatisfied / total_passengers) << std::endl;
	std::cout << generate_bus.get_unsatisfied_demand() << std::endl;

}