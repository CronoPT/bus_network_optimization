#ifndef __TNDP_CPP__
#define __TNDP_CPP__

#include "tndp.hpp"

namespace transit_problem {

	tndp::tndp(
		genetic::constraint_set<urban::bus_network> constraints, 
	    genetic::cost_function_set<urban::bus_network> cost_functions
	): problem(constraints, cost_functions) {
		/* Do Nothing */
	}

	std::vector<genetic::solution<urban::bus_network>> tndp::initialize_population() {
		auto pop = std::vector<genetic::solution<urban::bus_network>>();
		const int pop_size = 20;

		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> net_size_dist(
			tndp_configs::min_number_routes,
			tndp_configs::max_number_routes
		);
		std::uniform_int_distribution<int> route_dist(
			0, route_pool::instance()->size()-1
		);

		std::cout << "[TNDP] Pool Size: " << route_pool::instance()->size() << std::endl;
		std::cout << "[TNDP] Min N Routes: " << tndp_configs::min_number_routes << std::endl;
		std::cout << "[TNDP] Max N Routes: " << tndp_configs::max_number_routes << std::endl;
		std::cout << "[TNDP] Population Size: " << pop_size << std::endl;
		std::cout << "[TNDP] Going into the main cycle" << std::endl;

		for (int i=0; i<pop_size; i++) {
			int net_size = net_size_dist(rng);
			std::cout << "[TNDP] Bus " << i << "/" << pop_size;
			std::cout << " generating net with " << net_size;
			std::cout << " routes" << std::endl;
			auto routes  = std::vector<urban::route>();
			auto set = std::set<int>();
			for (int j=0; j<net_size; j++) {
				int route = route_dist(rng);
				while (set.find(route)!=set.end()) {
					route = route_dist(rng);
				}
				routes.push_back(route_pool::instance()->at(route));
				set.insert(route);
			}
			pop.push_back(genetic::solution<urban::bus_network>(
				urban::bus_network(routes)
			));
		}

		std::cout << "[TNDP] Exiting the main cycle" << std::endl;

		return pop;
	}

	std::pair<urban::bus_network, urban::bus_network> tndp::crossover(
		urban::bus_network& i1, urban::bus_network& i2
	) {
		const auto& routes_1 = i1.get_routes();
		const auto& routes_2 = i2.get_routes();

		int min_size = (routes_1.size() < routes_2.size()) ? 
		                routes_1.size() : routes_2.size();

		auto new_routes_1 = std::vector<urban::route>();
		auto new_routes_2 = std::vector<urban::route>();

		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> route_size_dist(
			1, min_size-1
		);
		int crossover_point = route_size_dist(rng);

		int i;
		for (i=0 ; i<crossover_point; i++) {
			new_routes_1.push_back(routes_2.at(i));
			new_routes_2.push_back(routes_1.at(i));
		}
		for (    ; i<min_size; i++) {
			new_routes_1.push_back(routes_1.at(i));
			new_routes_2.push_back(routes_2.at(i));
		}
		if (min_size == routes_1.size()) { // first network had less routes
			for ( ; i<routes_2.size(); i++) {
				new_routes_2.push_back(routes_2.at(i));
			}
		} else { // second network had less routes
			for ( ; i<routes_1.size(); i++) {
				new_routes_1.push_back(routes_1.at(i));
			}		
		}

		auto new_bus_1 = urban::bus_network(new_routes_1);
		auto new_bus_2 = urban::bus_network(new_routes_2);

		return std::pair<urban::bus_network, urban::bus_network>(
			new_bus_1, new_bus_2
		);
	}

	void tndp::mutate(urban::bus_network& item) const {

	}

} // namespace transit_problem

#endif