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
#include <classic_ga.hpp>

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

	auto GA = new genetic::classic_ga<urban::bus_network>(
		(genetic::problem<urban::bus_network>*) problem, {
			-15.851532967952707, 
			1.0391450846092443e-06, 
			53.132458885735744, 
			7.747345808405972e-09, 
			72.38214508631111
		}
	);

	auto solutions = GA->execute();

	std::ofstream file("../data/json/run_nets_single.json");
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

		auto routes = solution.get_item().get_routes();
		file << "\t\t\"routes\": [\n"; 
		for (int j=0; j<routes.size(); j++) {

			auto stops = routes.at(j).get_stop_sequence();
			file << "\t\t\t[\n";
			for (int k=0; k<stops.size(); k++) {
				file << "\t\t\t\t" << stops.at(k);

				if (k < stops.size()-1) {
					file << ",";
				}
				file << "\n";
			}

			file << "\t\t\t]";
			if (j < routes.size()-1) {
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