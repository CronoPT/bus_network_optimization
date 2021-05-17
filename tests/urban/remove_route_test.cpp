#include <gtest/gtest.h>
#include <lisbon_bus.hpp>
#include <bus_network.hpp>

TEST(remove_route_test, remove_route_simple_test) {
	auto transfers  = urban::lisbon_bus::instance()->get_transfers();
	auto in_vehicle = urban::lisbon_bus::instance()->get_in_vehicle_time();
	auto total_len  = urban::lisbon_bus::instance()->get_total_length();
	auto un_demand  = urban::lisbon_bus::instance()->get_unsatisfied_demand();
	auto nu_routes  = urban::lisbon_bus::instance()->get_number_routes();
	auto shortest   = urban::lisbon_bus::instance()->get_shortest_route();
	auto longest    = urban::lisbon_bus::instance()->get_longest_route();

	auto route_del = urban::lisbon_bus::instance()->get_routes().at(0);
	auto route_id  = route_del.get_route_id();

	std::cout << "1" << std::endl;
	std::cout << urban::lisbon_bus::instance()->get_number_routes() << std::endl;
	std::cout << urban::lisbon_bus::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::metro_network::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::metro_network::instance()->get_number_of_nodes() << std::endl;
	std::cout << urban::walking_network::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::walking_network::instance()->get_number_of_nodes() << std::endl;

	urban::lisbon_bus::instance()->delete_route(0);

	ASSERT_EQ(urban::lisbon_bus::instance()->get_number_routes(), nu_routes-1);	
	ASSERT_LT(urban::lisbon_bus::instance()->get_total_length(), total_len);
	ASSERT_LE(urban::lisbon_bus::instance()->get_unsatisfied_demand(), un_demand);
	ASSERT_GE(urban::lisbon_bus::instance()->get_transfers(), transfers);

	std::cout << "2" << std::endl;
	std::cout << urban::lisbon_bus::instance()->get_number_routes() << std::endl;
	std::cout << urban::lisbon_bus::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::metro_network::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::metro_network::instance()->get_number_of_nodes() << std::endl;
	std::cout << urban::walking_network::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::walking_network::instance()->get_number_of_nodes() << std::endl;

	urban::lisbon_bus::instance()->add_route(route_del);

	std::cout << "3" << std::endl;
	std::cout << urban::lisbon_bus::instance()->get_number_routes() << std::endl;
	std::cout << urban::lisbon_bus::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::metro_network::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::metro_network::instance()->get_number_of_nodes() << std::endl;
	std::cout << urban::walking_network::instance()->get_number_of_edges() << std::endl;
	std::cout << urban::walking_network::instance()->get_number_of_nodes() << std::endl;
	
	ASSERT_EQ(urban::lisbon_bus::instance()->get_transfers(), transfers);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_in_vehicle_time(), in_vehicle);
	std::cout << "Diff: " << (urban::lisbon_bus::instance()->get_total_length()-total_len) << std::endl;
	ASSERT_EQ(urban::lisbon_bus::instance()->get_total_length(), total_len);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_unsatisfied_demand(), un_demand);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_number_routes(), nu_routes);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_shortest_route(), shortest);
	ASSERT_EQ(urban::lisbon_bus::instance()->get_longest_route(), longest);

}