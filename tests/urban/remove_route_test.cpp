#include <gtest/gtest.h>
#include <lisbon_bus.hpp>
#include <bus_network.hpp>

TEST(remove_route_test, remove_route_simple_test) {
	auto nodes      = urban::lisbon_bus::instance()->get_number_of_nodes();
	auto transfers  = urban::lisbon_bus::instance()->get_transfers();
	auto in_vehicle = urban::lisbon_bus::instance()->get_in_vehicle_time();
	auto total_len  = urban::lisbon_bus::instance()->get_total_length();
	auto un_demand  = urban::lisbon_bus::instance()->get_unsatisfied_demand();
	auto nu_routes  = urban::lisbon_bus::instance()->get_number_routes();
	auto shortest   = urban::lisbon_bus::instance()->get_shortest_route();
	auto longest    = urban::lisbon_bus::instance()->get_longest_route();

	float total_edge_cost_1 = 0;
	for (auto& e: urban::lisbon_bus::instance()->get_edges()) {
		total_edge_cost_1 += e.second.get_attributes().get_time_taken();
	}

	auto route_del = urban::lisbon_bus::instance()->get_routes().at(0);
	auto route_id  = route_del.get_route_id();

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

	urban::lisbon_bus::instance()->add_route(route_del);

	float total_edge_cost_3 = 0;
	for (auto& e: urban::lisbon_bus::instance()->get_edges()) {
		total_edge_cost_3 += e.second.get_attributes().get_time_taken();
	}

	ASSERT_FLOAT_EQ(total_edge_cost_1, total_edge_cost_3);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_total_length(), total_len);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_transfers(), transfers);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_in_vehicle_time(), in_vehicle);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_unsatisfied_demand(), un_demand);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_number_routes(), nu_routes);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_shortest_route(), shortest);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_longest_route(), longest);
}