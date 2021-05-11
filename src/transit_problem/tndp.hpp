#ifndef __TNDP_HPP__
#define __TNDP_HPP__

#include <bus_network.hpp>
#include <metro_network.hpp>
#include <walking_network.hpp>
#include <grid.hpp>
#include <problem.hpp>
#include <constraint.hpp>
#include <cost_function.hpp>
#include <vector>
#include <random>
#include <osm_net.hpp>

#include "tndp_configs.hpp"
#include "route_pool.hpp"

namespace transit_problem {

	class tndp: public genetic::problem<urban::bus_network> {

		public:
		tndp(
			genetic::constraint_set<urban::bus_network> constraints, 
			genetic::cost_function_set<urban::bus_network> cost_functions
		);

		std::vector<genetic::solution<urban::bus_network>> initialize_population() override;
		std::pair<urban::bus_network, urban::bus_network> crossover(
			urban::bus_network& i1, urban::bus_network& i2
		) override;
		void mutate(urban::bus_network& item) const override;
	};

} // namespace transit_problem

#endif