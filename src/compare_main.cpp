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

osm_net::coord_sequence filter_coord_sequence(osm_net::coord_sequence& sequence) {
	auto filtered = osm_net::coord_sequence();

	auto prev_pair = std::pair<double, double>(-1, -1);
	for (auto& pair: sequence) {
		if (pair.first  == prev_pair.first &&
		    pair.second == prev_pair.second) {
			prev_pair = pair;
			continue;
		}
		filtered.push_back(pair);
		prev_pair = pair; 
	}

	return filtered;
}

osm_net::coord_sequence get_line_from_path(std::vector<int>& path) {
	auto& nodes = osm_net::osm_net::instance()->get_nodes();
	auto coordinates = osm_net::coord_sequence();

	int origin = -1;
	for (int destin : path) {
		if (origin != -1) {
			auto& node = nodes[origin];

			float min_length = std::numeric_limits<float>::max();
			net::edge<osm_net::osm_edge> min_edge;
			for (auto& adj: node.get_adjacencies()) {
				float length = adj.second.get_attributes().get_length();
				if (adj.second.get_destin() == destin && length < min_length) {
					min_length = length;
					min_edge   = adj.second; 
				}
			}

			if (min_edge.get_attributes().has_geometry()) {
				auto& geo = min_edge.get_attributes().get_geometry();
				coordinates.insert(coordinates.end(), geo.begin(), geo.end());
			} else {
				auto pair = std::pair<double, double>(
					nodes[origin].get_attributes().get_lon(),
					nodes[origin].get_attributes().get_lat()
				);
				coordinates.push_back(pair);
				pair = std::pair<double, double>(
					nodes[destin].get_attributes().get_lon(),
					nodes[destin].get_attributes().get_lat()
				);
				coordinates.push_back(pair);
			}
		}
		origin = destin;
	}

	return coordinates;
}

urban::bus_network read_generate_bus(int index) {
	std::ifstream input_file("../data/json/run_nets.json");
	nlohmann::json json = nlohmann::json::parse(input_file);
	
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
	int index = 199; // number of the generated network

	urban::bus_network original_bus = *urban::lisbon_bus::instance();
	urban::bus_network generate_bus = read_generate_bus(index);

	std::cout << "[O] Total Length: " << original_bus.get_total_length() << std::endl;
	std::cout << "[O] Unsatisfied: "  << original_bus.get_unsatisfied_demand() << std::endl;
	std::cout << "[O] In Vehicle: "   << original_bus.get_in_vehicle_time() << std::endl;
	std::cout << "[O] Transfers: "    << original_bus.get_transfers() << std::endl;
	std::cout << "[O] Num Nodes: "    << original_bus.get_number_of_nodes() << std::endl;
	std::cout << "[O] Num Edges: "    << original_bus.get_number_of_edges() << std::endl;
	std::cout << "[O] Num Routes: "   << original_bus.get_number_routes() << std::endl;
	std::cout << std::endl;

	std::cout << "<><><><>" << std::endl;

	std::cout << "[G] Total Length: " << generate_bus.get_total_length() << std::endl;
	std::cout << "[G] Unsatisfied: "  << generate_bus.get_unsatisfied_demand() << std::endl;
	std::cout << "[G] In Vehicle: "   << generate_bus.get_in_vehicle_time() << std::endl;
	std::cout << "[G] Transfers: "    << generate_bus.get_transfers() << std::endl;
	std::cout << "[G] Num Nodes: "    << generate_bus.get_number_of_nodes() << std::endl;
	std::cout << "[G] Num Edges: "    << generate_bus.get_number_of_edges() << std::endl;
	std::cout << "[G] Num Routes: "   << generate_bus.get_number_routes() << std::endl;
	std::cout << std::endl;

	std::cout << "________" << std::endl;

	urban::network_usage original_use = original_bus.get_usage();
	urban::network_usage generate_use = generate_bus.get_usage();

	float total_passengers = 0;
	float unsatisfied = 0;

	std::ostringstream stream;
	stream << "../data/json/comparisons/comparison-" << index << ".json";
	std::string file_name = stream.str();
	std::ofstream file(file_name);
	int i = 0;
	int size = urban::odx_matrix::instance()->get_all_pairs().size();
	file << "[\n";
	for (auto& pair : urban::odx_matrix::instance()->get_all_pairs()) {
		auto origin = pair.first;
		auto destin = pair.second;

		urban::trip original_trip = original_use.get_usage_between(origin, destin);
		urban::trip generate_trip = generate_use.get_usage_between(origin, destin); 

		float passengers = urban::odx_matrix::instance()->get_total(origin, destin);
		total_passengers += passengers;

		if (origin.first  == destin.first &&
		    origin.second == destin.second) {
			i += 1;
			continue;
		}

		if (generate_trip.get_stages().size() == 0) {
			i += 1;
			unsatisfied += passengers;
			continue;
		}

		int original_transfers = original_trip.get_transfers();
		int generate_transfers = generate_trip.get_transfers();

		float original_time = original_trip.get_time();
		float generate_time = generate_trip.get_time();

		float original_walking_time = original_trip.get_walking_time();
		int   original_used_modes   = original_trip.get_number_modes();

		float generate_walking_time = generate_trip.get_walking_time();
		int   generate_used_modes   = generate_trip.get_number_modes();  

		if (passengers == 0) {
			i += 1;
			continue;
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
		file << "\t\t\"generate_travel_time\": " << generate_time << ",\n";
		file << "\t\t\"original_walking_time\": " << original_walking_time << ",\n";
		file << "\t\t\"generate_walking_time\": " << generate_walking_time << ",\n";
		file << "\t\t\"original_uses_walking\": " << (original_trip.uses_walk() ? "true":"false") << ",\n";
		file << "\t\t\"generate_uses_walking\": " << (generate_trip.uses_walk() ? "true":"false") << ",\n";
		file << "\t\t\"original_uses_metro\": " << (original_trip.uses_metro() ? "true":"false") << ",\n";
		file << "\t\t\"generate_uses_metro\": " << (generate_trip.uses_metro() ? "true":"false") << ",\n";
		file << "\t\t\"original_uses_bus\": " << (original_trip.uses_bus() ? "true":"false") << ",\n";
		file << "\t\t\"generate_uses_bus\": " << (generate_trip.uses_bus() ? "true":"false") << ",\n";
		file << "\t\t\"original_modes\": " << original_used_modes << ",\n";
		file << "\t\t\"generate_modes\": " << generate_used_modes << "\n";
		file << "\t}";
		if (i<size-1) { file << ","; }
		file << "\n";
		i = i+1;
	}
	file << "]\n";
	file.close();

	auto lines = std::vector<osm_net::coord_sequence>();
	for (auto& route: generate_bus.get_routes()) {
		auto this_line = osm_net::coord_sequence();
		int origin_stop = -1;
		for (auto destin_stop: route.get_stop_sequence()) {
			if (origin_stop != -1) {
				auto path_report = osm_net::osm_net::instance()->dijkstra(
					origin_stop,
					destin_stop,
					[](net::edge<osm_net::osm_edge>& e) -> float {
						auto length = e.get_attributes().get_length();
						auto speed  = e.get_attributes().get_max_speed();
						return (float) length;
					}
				);

				auto& path = path_report.first;
				osm_net::coord_sequence coords = get_line_from_path(path);
				this_line.insert(this_line.end(), coords.begin(), coords.end());
			}
			origin_stop = destin_stop;
		}
		this_line = filter_coord_sequence(this_line);
		lines.push_back(this_line);
	}

	stream.str("");
	stream.clear();
	stream << "../data/geojson/results/network-" << index << ".geojson";
	file_name = stream.str();
	file = std::ofstream(file_name);

	file << "{\n";
	file << "\t\"type\":\"MultiLineString\",\n";
	file << "\t\"coordinates\": [\n";
	for (int i=0; i<lines.size(); i++) {
		auto line = lines.at(i);
		file << "\t\t[\n";
		for (int j=0; j<line.size(); j++) {
			auto pair = line.at(j);
			file << "\t\t\t[";
			file << pair.first << ", " << pair.second;
			file << "]";
			if (j < line.size()-1) { file << ","; }
			file << "\n";
		}
		file << "\t\t]";
		if (i < lines.size()-1) { file << ","; }
		file << "\n";
	}
	file << "\t]\n";
	file << "}\n";
	file.close();

	std::cout << (unsatisfied / total_passengers) << std::endl;
	std::cout << generate_bus.get_unsatisfied_demand() << std::endl;

}