#ifndef __TNDP_CPP__
#define __TNDP_CPP__

#include "tndp.hpp"

namespace transit_problem {

	tndp::tndp(
		genetic::constraint_set<urban::bus_network> constraints, 
	    genetic::cost_function_set<urban::bus_network> cost_functions,
		urban::metro_network& metro,
		urban::walking_network& walk,
		urban::grid& grid
	):
	 _metro(metro),
	 _walk(walk),
	 _grid(grid),
	 problem(constraints, cost_functions) {
		/* Do Nothing */
	}

	std::vector<genetic::solution<urban::bus_network>> tndp::initialize_population() {

	}

	std::pair<urban::bus_network, urban::bus_network> tndp::crossover(
		urban::bus_network& i1, urban::bus_network& i2
	) {

	}

	void tndp::mutate(urban::bus_network& item) const {

	}

} // namespace transit_problem

#endif