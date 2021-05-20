#include <gtest/gtest.h>
#include <lisbon_bus.hpp>
#include <bus_network.hpp>

TEST(remove_route_test, remove_route_simple_test) {
	auto nodes      = urban::lisbon_bus::instance()->get_number_of_nodes();
	std::cout << "Nodes line 7 -> " << nodes << std::endl;
	std::cout << "Nodes line 8 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto transfers  = urban::lisbon_bus::instance()->get_transfers();
	std::cout << "Nodes line 10 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto in_vehicle = urban::lisbon_bus::instance()->get_in_vehicle_time();
	std::cout << "Nodes line 12 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto total_len  = urban::lisbon_bus::instance()->get_total_length();
	std::cout << "Nodes line 14 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto un_demand  = urban::lisbon_bus::instance()->get_unsatisfied_demand();
	std::cout << "Nodes line 16 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto nu_routes  = urban::lisbon_bus::instance()->get_number_routes();
	std::cout << "Nodes line 18 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto shortest   = urban::lisbon_bus::instance()->get_shortest_route();
	std::cout << "Nodes line 20 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;
	auto longest    = urban::lisbon_bus::instance()->get_longest_route();
	std::cout << "Nodes line 22 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;


	auto& may = urban::lisbon_bus::instance()->get_routes().at(0);
	auto& mey = urban::lisbon_bus::instance()->get_routes().at(266);

	std::cout << "Nodes line 28 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;


	float total_edge_cost_1 = 0;
	for (auto& e: urban::lisbon_bus::instance()->get_edges()) {
		total_edge_cost_1 += e.second.get_attributes().get_time_taken();
	}

	std::cout << "Nodes line 48 -> " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;

	auto route_del = urban::lisbon_bus::instance()->get_routes().at(0);
	auto route_id  = route_del.get_route_id();

	std::cout << "Removed route n. " << route_id << " With "; 
	std::cout << route_del.size() << " stops" << std::endl;

	std::cout << "1" << std::endl;
	std::cout << "Lisbon Bus Routes: " << urban::lisbon_bus::instance()->get_number_routes() << std::endl;
	std::cout << "Lisbon Bus Shortest Route: " << urban::lisbon_bus::instance()->get_shortest_route() << std::endl;
	std::cout << "Lisbon Bus Longest Route: " << urban::lisbon_bus::instance()->get_longest_route() << std::endl;
	std::cout << "Lisbon Bus Total Length: " << urban::lisbon_bus::instance()->get_total_length() << std::endl;
	std::cout << "Lisbon Bus N. Edges: " << urban::lisbon_bus::instance()->get_number_of_edges() << std::endl;
	std::cout << "Lisbon Bus N. Nodes: " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;

	std::cout << "|||<<<<<<<<<<<<<<<<<<<<|REMOVE ROUTE|>>>>>>>>>>>>>>>>>>>>|||" << std::endl;
	urban::lisbon_bus::instance()->delete_route(0);

	float total_edge_cost_2 = 0;
	for (auto& e: urban::lisbon_bus::instance()->get_edges()) {
		total_edge_cost_2 += e.second.get_attributes().get_time_taken();
	}

	ASSERT_EQ(urban::lisbon_bus::instance()->get_number_of_nodes(), nodes-route_del.size());
	ASSERT_EQ(urban::lisbon_bus::instance()->get_number_routes(), nu_routes-1);	
	ASSERT_LT(urban::lisbon_bus::instance()->get_total_length(), total_len);
	ASSERT_LE(urban::lisbon_bus::instance()->get_unsatisfied_demand(), un_demand);
	ASSERT_GE(urban::lisbon_bus::instance()->get_transfers(), transfers);
	ASSERT_GT(total_edge_cost_1, total_edge_cost_2);

	std::cout << "2" << std::endl;
	std::cout << "Lisbon Bus Routes: " << urban::lisbon_bus::instance()->get_number_routes() << std::endl;
	std::cout << "Lisbon Bus Shortest Route: " << urban::lisbon_bus::instance()->get_shortest_route() << std::endl;
	std::cout << "Lisbon Bus Longest Route: " << urban::lisbon_bus::instance()->get_longest_route() << std::endl;
	std::cout << "Lisbon Bus Total Length: " << urban::lisbon_bus::instance()->get_total_length() << std::endl;
	std::cout << "Lisbon Bus N. Edges: " << urban::lisbon_bus::instance()->get_number_of_edges() << std::endl;
	std::cout << "Lisbon Bus N. Nodes: " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;

	std::cout << "|||<<<<<<<<<<<<<<<<<<<<|ADD ROUTE BACK IN|>>>>>>>>>>>>>>>>>>>>|||" << std::endl;
	urban::lisbon_bus::instance()->add_route(route_del);

	float total_edge_cost_3 = 0;
	for (auto& e: urban::lisbon_bus::instance()->get_edges()) {
		total_edge_cost_3 += e.second.get_attributes().get_time_taken();
	}

	auto& may_ = urban::lisbon_bus::instance()->get_routes().at(265);
	auto& mey_ = urban::lisbon_bus::instance()->get_routes().at(308);

	for (auto& stop: may_.get_stop_sequence()) { std::cout << stop << " "; }
	std::cout << std::endl;
	for (auto& stop: mey_.get_stop_sequence()) { std::cout << stop << " "; }
	std::cout << std::endl;
	std::cout << "Adjancencies for 4114: ";
	for (auto& adj: urban::lisbon_bus::instance()->get_nodes()[4114].get_adjacencies()) {
		if (adj.second.get_destin() == 4110) {
			std::cout << "{" << adj.first << "," << adj.second.get_destin(); 
			std::cout << "," << adj.second.get_attributes().get_route_id() << "} ";
		}
	}
	std::cout << std::endl;

	std::cout << "3" << std::endl;
	std::cout << "Lisbon Bus Routes: " << urban::lisbon_bus::instance()->get_number_routes() << std::endl;
	std::cout << "Lisbon Bus Shortest Route: " << urban::lisbon_bus::instance()->get_shortest_route() << std::endl;
	std::cout << "Lisbon Bus Longest Route: " << urban::lisbon_bus::instance()->get_longest_route() << std::endl;
	std::cout << "Lisbon Bus Total Length: " << urban::lisbon_bus::instance()->get_total_length() << std::endl;
	std::cout << "Lisbon Bus N. Edges: " << urban::lisbon_bus::instance()->get_number_of_edges() << std::endl;
	std::cout << "Lisbon Bus N. Nodes: " << urban::lisbon_bus::instance()->get_number_of_nodes() << std::endl;

	ASSERT_FLOAT_EQ(total_edge_cost_1, total_edge_cost_3);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_total_length(), total_len);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_transfers(), transfers);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_in_vehicle_time(), in_vehicle);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_unsatisfied_demand(), un_demand);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_number_routes(), nu_routes);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_shortest_route(), shortest);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_longest_route(), longest);

}