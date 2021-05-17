#include <tndp.hpp>
#include <tndp_objectives.hpp>
#include <tndp_constraints.hpp>
#include <bus_network.hpp>
#include <lisbon_bus.hpp>
#include <osm_net.hpp>
#include <metro_network.hpp>
#include <walking_network.hpp>
#include <grid.hpp>
#include <bus_network.hpp>
#include <route_pool.hpp>
#include <nsga.hpp>

int main() {

	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	auto constraints = genetic::constraint_set<urban::bus_network>();
	constraints.push_back(std::make_shared<transit_problem::route_length>(
		transit_problem::route_length()));
	constraints.push_back(std::make_shared<transit_problem::number_routes>(
		transit_problem::number_routes()));
	constraints.push_back(std::make_shared<transit_problem::transfers>(
		transit_problem::transfers()));

	auto cost_functs = genetic::cost_function_set<urban::bus_network>();
	cost_functs.push_back(std::make_shared<transit_problem::min_total_length>(
		transit_problem::min_total_length()));
	cost_functs.push_back(std::make_shared<transit_problem::min_unsatisfied_demand>(
		transit_problem::min_unsatisfied_demand()));
	cost_functs.push_back(std::make_shared<transit_problem::min_in_vehicle_time>(
		transit_problem::min_in_vehicle_time()));
	cost_functs.push_back(std::make_shared<transit_problem::min_transfers>(
		transit_problem::min_transfers()));

	auto problem = new transit_problem::tndp(
		constraints, 
		cost_functs
	);

	auto GA = new genetic::nsga<urban::bus_network>((genetic::problem<urban::bus_network>*) problem);

	auto solutions = GA->execute(10, 0.0001, 30);

	delete GA;
	delete problem;

	urban::lisbon_bus::instance()->evaluate();

}