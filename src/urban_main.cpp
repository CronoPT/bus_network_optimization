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

	auto grid = urban::grid(road, bus, metro);
	std::cout << "Built grid" << std::endl;
	grid.generate_geojson();
	std::cout << "Made geojson file with grid" << std::endl;
	grid.print_report();
	start = std::chrono::high_resolution_clock::now();
	auto use = grid.predict_all_od_pairs(bus, metro, walk);
	stop  = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Predicted all od pairs in " << duration.count() << " milliseconds" << std::endl;
	auto odx = urban::odx_matrix();
	bus.evaluate(use, metro, walk, odx);
	std::cout << "Evaluated bus network" << std::endl;
}