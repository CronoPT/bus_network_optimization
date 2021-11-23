#include <tnfsp.hpp>
#include <tnfsp_objectives.hpp>
#include <tnfsp_constraints.hpp>
#include <tndp.hpp>
#include <tndp_objectives.hpp>
#include <tndp_constraints.hpp>
#include <bus_network.hpp>
#include <lisbon_bus.hpp>
#include <osm_net.hpp>
#include <metro_network.hpp>
#include <walking_network.hpp>
#include <grid.hpp>
#include <route_pool.hpp>
#include <nsga.hpp>
#include <classic_ga.hpp>
#include <chrono>
#include <genetic_configs.hpp>
#include <frequency_set.hpp>

bool equal_sequences(std::vector<int> v1, std::vector<int> v2) {
	if (v1.size() != v2.size()) {
		return false;
	} else {
		for (int i=0; i<v1.size(); i++) {
			if (v1.at(i) != v2.at(i)) { return false; }
		}
	}
	return true;
}

urban::frequency_set read_ori_frequencies(urban::bus_network* bus) {
	std::ifstream input_file("../data/json/frequencies_complete.json");
	nlohmann::json json = nlohmann::json::parse(input_file);

	auto& routes = bus->get_routes();
	auto frequencies = std::vector<float>(routes.size());
	for (int i=0; i<routes.size(); i++) {
		auto& route = routes.at(i);
		auto& stop_sequence = route.get_stop_sequence();
		for (auto& json_obj: json) {
			auto& json_stop_sequence = json_obj["stops"];
			if (equal_sequences(stop_sequence, json_stop_sequence)) {
				frequencies.at(i) = json_obj["total"];
				break;
			}
		}
	}

	return urban::frequency_set(frequencies, bus, 24, "total");
}

void tndp() {

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

	// auto GA = new genetic::classic_ga<urban::bus_network>(
	// 	(genetic::problem<urban::bus_network>*) problem, 
	// 	{0, 1, 1, 1, 1},
	// 	{3561220.0, 0.0884952, 281127000.0, 0.271926},
	// 	{1904530.0, 0.00698877, 233077000.0, 0.180342}
	// );


	auto start = std::chrono::steady_clock::now();
	// auto GA = new genetic::nsga<urban::bus_network>(
	// 	(genetic::problem<urban::bus_network>*) problem
	// );
	auto GA = new genetic::classic_ga<urban::bus_network>(
		(genetic::problem<urban::bus_network>*) problem, 
		{0, 1, 1, 1, 1},
		{3561220.0, 0.0884952, 281127000.0, 0.271926},
		{1904530.0, 0.00698877, 233077000.0, 0.180342}
	);
	auto solutions = GA->execute();
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "TNDP (200 pop, 300 ite) -> " << duration << "seconds" << std::endl;

	start = std::chrono::steady_clock::now();
	urban::lisbon_bus::instance()->evaluate();
	end = std::chrono::steady_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Evaluate original network (309) -> " << duration << "seconds" << std::endl;

	start = std::chrono::steady_clock::now();
	solutions.at(0).get_item().evaluate();
	end = std::chrono::steady_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Evaluate solution network (" << solutions.at(0).get_item().get_number_routes()
	          << ") -> " << duration << "seconds" << std::endl;

	// std::ofstream file("../data/json/run_nets_single.json");
	// file << "[\n";
	// for (int i=0; i<solutions.size(); i++) {
	// 	auto solution = solutions.at(i);
	// 	file << "\t{\n";

	// 	auto costs = solution.get_costs();
	// 	file << "\t\t\"costs\": [\n";
	// 	for (int j=0; j<costs.size(); j++) {
	// 		file << "\t\t\t" << costs.at(j); 
	// 		if (j < costs.size()-1) {
	// 			file << ",";
	// 		}
	// 		file << "\n";
	// 	}
	// 	file << "\t\t],\n";

	// 	auto routes = solution.get_item().get_routes();
	// 	file << "\t\t\"routes\": [\n"; 
	// 	for (int j=0; j<routes.size(); j++) {

	// 		auto stops = routes.at(j).get_stop_sequence();
	// 		file << "\t\t\t[\n";
	// 		for (int k=0; k<stops.size(); k++) {
	// 			file << "\t\t\t\t" << stops.at(k);

	// 			if (k < stops.size()-1) {
	// 				file << ",";
	// 			}
	// 			file << "\n";
	// 		}

	// 		file << "\t\t\t]";
	// 		if (j < routes.size()-1) {
	// 			file << ",";
	// 		}
	// 		file << "\n";
	// 	}
	// 	file << "\t\t]\n";

	// 	file << "\t}";
	// 	if (i < solutions.size()-1) {
	// 		file << ",";
	// 	}
	// 	file << "\n";

	// }
	// file << "]\n";

	delete GA;
	delete problem;

}

void tnfsp() {
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
	// auto best_bus = read_generate_bus(0);

	auto problem = new transit_problem::tnfsp(
		constraints, 
		cost_functs,
		lisbon
	);

	// auto start = std::chrono::steady_clock::now();
	// auto GA = new genetic::classic_ga<urban::frequency_set>(
	// 	(genetic::problem<urban::frequency_set>*) problem,
	// 	{0, 1, 1},
	// 	{0.128749, 980918},
	// 	{0.0532552, 501343}
	// );
	// auto solutions = GA->execute();
	// auto end = std::chrono::steady_clock::now();
	// auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	// std::cout << "TNFSP (200 pop, 300 ite) -> " << duration << "seconds" << std::endl;

	auto set = read_ori_frequencies(urban::lisbon_bus::instance());

	auto start = std::chrono::steady_clock::now();
	set.evaluate();
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Evaluate solution freq. set -> " << duration << " milliseconds" << std::endl;
}

int main() {

	tndp();
	genetic::genetic_configs::threaded = false;
	// tnfsp();

}