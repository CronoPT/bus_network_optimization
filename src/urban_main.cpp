#include <bus_edge.hpp>
#include <bus_metro_walks.hpp>
#include <bus_network.hpp>
#include <bus_node.hpp>
#include <lisbon_bus.hpp>
#include <metro_edge.hpp>
#include <metro_network.hpp>
#include <metro_node.hpp>
#include <predictor.hpp>
#include <route.hpp>
#include <walking_edge.hpp>
#include <walking_network.hpp>
#include <walking_node.hpp>

#include <chrono>
#include <iostream>

int main() {
	auto road  = osm_net::osm_net();
	std::cout << "Read road network" << std::endl;
	std::cout << "Number of nodes: " << road.get_number_of_nodes() << "\n";
	std::cout << "Number of edges: " << road.get_number_of_edges() << "\n";

	auto start = std::chrono::high_resolution_clock::now();
	auto bus   = urban::import_lisbon_bus(road);
	auto stop  = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Reading took " << duration.count() << " milliseconds" << std::endl;

	std::cout << "Read bus network" << std::endl;
	auto metro = urban::metro_network();
	std::cout << "Read metro network" << std::endl;
	auto walk  = urban::walking_network();
	std::cout << "Read walking network" << std::endl;
}