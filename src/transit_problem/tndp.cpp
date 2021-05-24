#ifndef __TNDP_CPP__
#define __TNDP_CPP__

#include "tndp.hpp"

namespace transit_problem {

	int generate_number_between(int low, int high) {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> dist(low, high);
		return dist(rng);
	}

	tndp::tndp(
		genetic::constraint_set<urban::bus_network> constraints, 
	    genetic::cost_function_set<urban::bus_network> cost_functions
	): problem(constraints, cost_functions) {
		/* Do Nothing */
	}

	std::vector<genetic::solution<urban::bus_network>> tndp::initialize_population() {
		auto pop = std::vector<genetic::solution<urban::bus_network>>();
		const int pop_size = 5;

		std::cout << "[TNDP] Pool Size: " << route_pool::instance()->size() << std::endl;
		std::cout << "[TNDP] Min N Routes: " << tndp_configs::min_number_routes << std::endl;
		std::cout << "[TNDP] Max N Routes: " << tndp_configs::max_number_routes << std::endl;
		std::cout << "[TNDP] Population Size: " << pop_size << std::endl;
		std::cout << "[TNDP] Going into the main cycle" << std::endl;


		pop.push_back(genetic::solution<urban::bus_network>(
			*urban::lisbon_bus::instance()
		));
		std::cout << "Read Lisbon Bus Network" << std::endl;
		for (int i=1; i<pop_size; i++) {
			auto routes  = std::vector<urban::route>();

			for (auto& route: route_pool::instance()->get_mandatory_routes()) {
				routes.push_back(route);
			}

			int net_size = generate_number_between(
				tndp_configs::min_number_routes-route_pool::instance()->get_number_mandatory(),
				tndp_configs::max_number_routes-route_pool::instance()->get_number_mandatory()
			);
			std::cout << "[TNDP] Bus " << i+1 << "/" << pop_size;
			std::cout << " generating net with " << net_size;
			std::cout << " routes" << std::endl;
			auto set = std::set<int>();
			for (int j=0; j<net_size; j++) {
				int route = generate_number_between(0, route_pool::instance()->size()-1);
				while (set.find(route)!=set.end()) {
					route = generate_number_between(0, route_pool::instance()->size()-1);
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
			route_pool::instance()->get_number_mandatory()+1, 
			min_size-1
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

	bool do_route_deletion() {
		float rand_nu = ((float) std::rand() / RAND_MAX);
		return 0.4 >= rand_nu;
	}

	bool do_route_addition() {
		float rand_nu = ((float) std::rand() / RAND_MAX);
		return 0.4 >= rand_nu;
	}

	void tndp::mutate(urban::bus_network& item) const {
		//replacing a random route
		int route_index = generate_number_between(0, route_pool::instance()->size()-1);
		while (!item.has_route(route_pool::instance()->at(route_index).get_route_id())) {
			route_index = generate_number_between(0, route_pool::instance()->size()-1);
		}
		int to_replace = generate_number_between(
			route_pool::instance()->get_number_mandatory(), 
			item.get_number_routes()-1
		);
		item.delete_route(to_replace);
		const auto& to_add = route_pool::instance()->at(route_index);
		item.add_route(to_add);

		//adding
		if (do_route_addition()) {
			int new_route_index = generate_number_between(0, route_pool::instance()->size()-1);
			while (!item.has_route(route_pool::instance()->at(route_index).get_route_id())) {
				new_route_index = generate_number_between(0, route_pool::instance()->size()-1);
			}
			const auto& new_route = route_pool::instance()->at(route_index);
			item.add_route(new_route);
		}

		//removing a random route
		if (do_route_deletion()) {
			int to_delete_index = generate_number_between(
				route_pool::instance()->get_number_mandatory(), 
				item.get_number_routes()-1
			);
			item.delete_route(to_delete_index);
		}

	}

} // namespace transit_problem

#endif