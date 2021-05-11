#ifndef __TNDP_OBJECTIVES_HPP__
#define __TNDP_OBJECTIVES_HPP__

#include <bus_network.hpp>
#include <cost_function.hpp>

namespace transit_problem {

	/**
	 * Minimize the total lenght of the routes.
	 * The more you can do with less distance to cover,
	 * the better.
	*/
	class min_total_length: public genetic::cost_function<urban::bus_network> {
		public:
		float compute(const urban::bus_network& sol) const {
			return sol.get_total_length();
		}
	};

	/**
	 * Minimize unsatisfied demand.
	*/
	class min_unsatisfied_demand: public genetic::cost_function<urban::bus_network> {
		public:
		float compute(const urban::bus_network& sol) const {
			return sol.get_unsatisfied_demand();
		}
	};

	/**
	 * Minimize distance to other modes od transportation.
	 * TODO: talk about this.
	 * I think it makes no sense seeing as we do not get to
	 * decide locations for new stops.
	*/
	// class mode_connectivity: public genetic::cost_function<urban::bus_network> {
	// 	public:
	// 	float compute(const urban::bus_network& sol) const {
			
	// 	}
	// };

	/**
	 * The less time spent inside vehicles by the users, the better.
	*/
	class min_in_vehicle_time: public genetic::cost_function<urban::bus_network> {
		public:
		float compute(const urban::bus_network& sol) const {
			return sol.get_in_vehicle_time();
		}
	};

	/**
	 * Perhaps the biggest objective, from the user point of view.
	 * The service is much better if people do not have to transfer
	 * a lot to get to their desired destinations.
	*/
	class min_transfers: public genetic::cost_function<urban::bus_network> {
		public:
		float compute(const urban::bus_network& sol) const {
			return sol.get_transfers();
		}
	};

} // transit_problem

#endif