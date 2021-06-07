#ifndef __TNFSP_HPP__
#define __TNFSP_HPP__

#include <bus_network.hpp>
#include <metro_network.hpp>
#include <walking_network.hpp>
#include <grid.hpp>
#include <problem.hpp>
#include <constraint.hpp>
#include <cost_function.hpp>
#include <vector>
#include <random>
#include <cstdlib>
#include <osm_net.hpp>
#include <frequency_set.hpp>

#include "tnfsp_configs.hpp"

namespace transit_problem {

	class tnfsp: public genetic::problem<urban::frequency_set> {

		urban::bus_network _network;

		public:
		tnfsp(
			genetic::constraint_set<urban::frequency_set> constraints, 
			genetic::cost_function_set<urban::frequency_set> cost_functions,
			urban::bus_network& network
		);

		std::vector<genetic::solution<urban::frequency_set>> 
			initialize_population(int pop_size) override;
		std::pair<urban::frequency_set, urban::frequency_set> crossover(
			urban::frequency_set& i1, urban::frequency_set& i2
		) override;
		void mutate(urban::frequency_set& item) const override;

	};

}

#endif