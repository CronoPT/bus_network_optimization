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
#include <frequency_set.hpp>

#include <chrono>
#include <iostream>

#include <nlohmann/json.hpp>

bool equal_sequences(std::vector<int> v1, std::vector<int> v2) {
	if (v1.size() != v2.size()) {
		return false;
	} else {
		for (int i=0; i<v1.size(); i++) {
			if (v1.at(i) != v2.at(i)) { return false; }
		}
	}
	return true;
}

urban::frequency_set read_ori_frequencies(urban::bus_network* bus) {
	std::ifstream input_file("../data/json/frequencies_complete.json");
	nlohmann::json json = nlohmann::json::parse(input_file);

	auto& routes = bus->get_routes();
	auto frequencies = std::vector<float>(routes.size());
	for (int i=0; i<routes.size(); i++) {
		auto& route = routes.at(i);
		auto& stop_sequence = route.get_stop_sequence();
		for (auto& json_obj: json) {
			auto& json_stop_sequence = json_obj["stops"];
			if (equal_sequences(stop_sequence, json_stop_sequence)) {
				frequencies.at(i) = json_obj["total"];
				break;
			}
		}
	}

	return urban::frequency_set(frequencies, bus, 24, "total");
}

urban::frequency_set read_new_frequencies(urban::bus_network* bus) {
	std::ifstream input_file("../data/json/run_frequencies.json");
	nlohmann::json json = nlohmann::json::parse(input_file);

	auto frequencies = std::vector<float>();
	for (auto frequency: json[0]["frequencies"]) {
		frequencies.push_back(frequency);
	}

	return urban::frequency_set(frequencies, bus, 24, "total");
}

int main() {
	auto lisbon = urban::lisbon_bus::instance();
	auto ori_lisbon_frequencies = read_ori_frequencies(lisbon);
	auto new_lisbon_frequencies = read_new_frequencies(lisbon);

	ori_lisbon_frequencies.compute_waiting_time();
	new_lisbon_frequencies.compute_waiting_time();

	const auto& ori_waiting = ori_lisbon_frequencies.get_waiting_time_discriminated();
	const auto& new_waiting = new_lisbon_frequencies.get_waiting_time_discriminated();

	// std::cout << ori_waiting.at(400).at(400).at(12).at(12) << std::endl;
}