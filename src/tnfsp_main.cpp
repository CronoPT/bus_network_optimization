#include <tnfsp.hpp>
#include <tnfsp_objectives.hpp>
#include <tnfsp_constraints.hpp>
#include <bus_network.hpp>
#include <frequency_set.hpp>
#include <lisbon_bus.hpp>
#include <osm_net.hpp>
#include <metro_network.hpp>
#include <walking_network.hpp>
#include <grid.hpp>
#include <route_pool.hpp>
#include <nsga.hpp>

int main() {

	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	auto constraints = genetic::constraint_set<urban::frequency_set>();
	constraints.push_back(std::make_shared<transit_problem::fleet>(
		transit_problem::fleet()));
	constraints.push_back(std::make_shared<transit_problem::load_factor>(
		transit_problem::load_factor()));
	constraints.push_back(std::make_shared<transit_problem::frequency>(
		transit_problem::frequency()));

	auto cost_functs = genetic::cost_function_set<urban::frequency_set>();
	cost_functs.push_back(std::make_shared<transit_problem::min_waiting_time>(
		transit_problem::min_waiting_time()));
	cost_functs.push_back(std::make_shared<transit_problem::min_operator_costs>(
		transit_problem::min_operator_costs()));

	auto lisbon = urban::lisbon_bus::instance();

	auto problem = new transit_problem::tnfsp(
		constraints, 
		cost_functs,
		*lisbon
	);

	auto GA = new genetic::nsga<urban::frequency_set>((genetic::problem<urban::frequency_set>*) problem);

	auto solutions = GA->execute();

	delete GA;
	delete problem;
}