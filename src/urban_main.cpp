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

int main() {
	urban::grid::instance()->print_report();
	auto start = std::chrono::high_resolution_clock::now();
	auto use = urban::grid::instance()->predict_all_od_pairs(*urban::lisbon_bus::instance());
	auto stop  = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Predicted all od pairs in " << duration.count() << " milliseconds" << std::endl;	
}