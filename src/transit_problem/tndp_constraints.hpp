#ifndef __TNDP_CONSTRAINTS_HPP__
#define __TNDP_CONSTRAINTS_HPP__

#include <bus_network.hpp>
#include <constraint.hpp>

namespace transit_problem {

	/**
	 * There can be a minimum and maximum allowable 
	 * route length.
	*/
	class route_length: public genetic::constraint<urban::bus_network> {
		public:
		bool satisfied(const urban::bus_network& sol) const {

		}

		float transgression(const urban::bus_network& sol) const {

		}
	};

	/**
	 * There can be a minimum and maximum allowable
	 * number of routes in a network. 
	*/
	class number_routes: public genetic::constraint<urban::bus_network> {
		public:
		bool satisfied(const urban::bus_network& sol) const {

		}

		float transgression(const urban::bus_network& sol) const {

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

		}

		float transgression(const urban::bus_network& sol) const {

		}
	};

} // transit_problem

#endif