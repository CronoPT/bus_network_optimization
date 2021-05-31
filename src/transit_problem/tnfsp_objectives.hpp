#ifndef __TNFSP_OBJECTIVES_HPP__
#define __TNFSP_OBJECTIVES_HPP__

#include <bus_network.hpp>
#include <frequency_set.hpp>
#include <cost_function.hpp>

namespace transit_problem {

	class min_waiting_time: public genetic::cost_function<urban::frequency_set> {
		public:
		float compute(urban::frequency_set& sol) const {
			return sol.get_waiting_time();
		}
	};

	class min_operator_costs: public genetic::cost_function<urban::frequency_set> {
		public:
		float compute(urban::frequency_set& sol) const {
			return sol.get_operator_costs();
		}
	};

}

#endif