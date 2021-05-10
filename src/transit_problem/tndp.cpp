#ifndef __TNDP_CPP__
#define __TNDP_CPP__

#include "tndp.hpp"

namespace transit_problem {

	tndp::tndp(
		genetic::constraint_set<urban::bus_network> constraints, 
	    genetic::cost_function_set<urban::bus_network> cost_functions,
		urban::metro_network& metro,
		urban::walking_network& walk,
		urban::grid& grid,
		route_pool& pool,
		osm_net::osm_net& road
	):
	 _metro(metro),
	 _walk(walk),
	 _grid(grid),
	 _pool(pool),
	 _road(road),
	 problem(constraints, cost_functions) {
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
			0, _pool.size()
		);

		for (int i=0; i<pop_size; i++) {
			int net_size = net_size_dist(rng);
			auto routes  = std::vector<urban::route>();
			auto set = std::set<int>();
			for (int j=0; j<net_size; j++) {
				int route = route_dist(rng);
				while (set.find(route)==set.end()) {
					route = route_dist(rng);
				}
				routes.push_back(_pool.at(route));
				set.insert(route);
			}
			pop.push_back(genetic::solution<urban::bus_network>(
				urban::bus_network(routes, _road)
			));
		}

		return pop;
	}

	std::pair<urban::bus_network, urban::bus_network> tndp::crossover(
		urban::bus_network& i1, urban::bus_network& i2
	) {

	}

	void tndp::mutate(urban::bus_network& item) const {

	}

} // namespace transit_problem

#endif