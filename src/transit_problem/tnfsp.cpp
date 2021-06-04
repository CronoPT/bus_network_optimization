#ifndef __TNFSP_CPP__
#define __TNFSP_CPP__

#include "tnfsp.hpp"

namespace transit_problem {

	int generate_number_between(int low, int high) {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> dist(low, high);
		return dist(rng);
	}

	tnfsp::tnfsp(
		genetic::constraint_set<urban::frequency_set> constraints, 
	    genetic::cost_function_set<urban::frequency_set> cost_functions,
		urban::bus_network& network
	): problem(constraints, cost_functions), _network(network) {
		/* Do Nothing */
	}

	std::vector<genetic::solution<urban::frequency_set>> 
		tnfsp::initialize_population(int pop_size) {

	}

	std::pair<urban::frequency_set, urban::frequency_set> tnfsp::crossover(
		urban::frequency_set& i1, urban::frequency_set& i2
	) {

	}

	void tnfsp::mutate(urban::frequency_set& item) const {

	}

} // namespace transit_problem

#endif