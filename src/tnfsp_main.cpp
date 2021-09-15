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
#include <classic_ga.hpp>

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
		lisbon
	);

	auto GA = new genetic::classic_ga<urban::frequency_set>(
		(genetic::problem<urban::frequency_set>*) problem,
		{0, 1, 1},
		{0.120607, 326317},
		{0.0495152, 160220}
	);

	std::cout << "Executing" << std::endl; 

	auto solutions = GA->execute();

	std::ofstream file("../data/json/run_frequencies_single_night.json");
	file << "[\n";
	for (int i=0; i<solutions.size(); i++) {
		auto solution = solutions.at(i);
		file << "\t{\n";

		auto costs = solution.get_costs();
		file << "\t\t\"costs\": [\n";
		for (int j=0; j<costs.size(); j++) {
			file << "\t\t\t" << costs.at(j); 
			if (j < costs.size()-1) {
				file << ",";
			}
			file << "\n";
		}
		file << "\t\t],\n";

		auto frequencies = solution.get_item().get_frequencies();
		file << "\t\t\"frequencies\": [\n"; 
		for (int j=0; j<frequencies.size(); j++) {

			auto frequency = frequencies.at(j);
			file << "\t\t\t" << frequency;
			if (j < frequencies.size()-1) {
				file << ",";
			}
			file << "\n";

		}
		file << "\t\t]\n";

		file << "\t}";
		if (i < solutions.size()-1) {
			file << ",";
		}
		file << "\n";

	}
	file << "]\n";

	delete GA;
	delete problem;
}