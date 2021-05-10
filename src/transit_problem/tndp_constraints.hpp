#ifndef __TNDP_CONSTRAINTS_HPP__
#define __TNDP_CONSTRAINTS_HPP__

#include <cmath>
#include <bus_network.hpp>
#include <constraint.hpp>
#include "tndp_configs.hpp"

namespace transit_problem {

	/**
	 * There can be a minimum and maximum allowable 
	 * route length.
	*/
	class route_length: public genetic::constraint<urban::bus_network> {
		public:
		bool satisfied(const urban::bus_network& sol) const {
			int longest  = sol.get_longest_route();
			int shortest = sol.get_shortest_route();
			bool max = longest  <= tndp_configs::max_route_length && 
			           longest  >= tndp_configs::min_route_length;
			bool min = shortest <= tndp_configs::max_route_length && 
			           shortest >= tndp_configs::min_route_length;
			return max && min;
		}

		float transgression(const urban::bus_network& sol) const {
			if (satisfied(sol)) {
				return 0;
			}
			
			int lon = sol.get_longest_route();
			int sho = sol.get_shortest_route();

			float total = 0;
			if (lon > tndp_configs::max_route_length) {
				total += std::pow((lon-tndp_configs::max_route_length)+2, 2);
			}
			if (lon < tndp_configs::min_route_length) {
				total += std::pow((tndp_configs::min_route_length-lon)+2, 2);
			}
			if (sho > tndp_configs::max_route_length) {
				total += std::pow((sho-tndp_configs::max_route_length)+2, 2);
			}
			if (sho < tndp_configs::min_route_length) {
				total += std::pow((tndp_configs::min_route_length-sho)+2, 2);
			}
			return total;
		}
	};

	/**
	 * There can be a minimum and maximum allowable
	 * number of routes in a network. 
	*/
	class number_routes: public genetic::constraint<urban::bus_network> {
		public:
		bool satisfied(const urban::bus_network& sol) const {
			int routes = sol.get_number_routes();
			return routes >= tndp_configs::min_number_routes &&
			       routes <= tndp_configs::max_number_routes;
		}

		float transgression(const urban::bus_network& sol) const {
			int routes = sol.get_number_routes();
			
			if (satisfied(sol)) {
				return 0;
			} else if (routes < tndp_configs::min_number_routes) {
				return std::pow((tndp_configs::min_number_routes-routes)+2, 2);
			} else /*if (routes > tndp_configs::max_number_routes)*/ {
				return std::pow((routes-tndp_configs::max_number_routes)+2, 2);
			}
		}
	};

	/**
	 * The network should assure that the passengers reach
	 * their destinations within a reasonable number of
	 * transfers.
	*/
	class transfers: public genetic::constraint<urban::bus_network> {
		public:
		bool satisfied(const urban::bus_network& sol) const {
			return sol.get_transfers() <= tndp_configs::max_transfers;
		}

		float transgression(const urban::bus_network& sol) const {
			if (satisfied(sol)) {
				return 0;
			} else { 
				return std::pow(
					(sol.get_transfers()-tndp_configs::max_transfers)+2, 2
				); 
			}
		}
	};

} // transit_problem

#endif