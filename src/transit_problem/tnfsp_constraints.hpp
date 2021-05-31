#ifndef __TNFSP_CONSTRAINTS_HPP__
#define __TNFSP_CONSTRAINTS_HPP__

#include <cmath>
#include <bus_network.hpp>
#include <constraint.hpp>
#include "tnfsp_configs.hpp"

namespace transit_problem {

	class fleet: public genetic::constraint<urban::frequency_set> {
		public:
		bool satisfied(urban::frequency_set& sol) const {
			return sol.get_required_fleet() <= tnfsp_configs::available_fleet;
		}
		
		float transgression(urban::frequency_set& sol) const {
			int need = sol.get_required_fleet();
			if (satisfied(sol)) {
				return 0;
			} else {
				return std::pow((need-tnfsp_configs::available_fleet)+2, 2);
			}
		}
	};

	class load_factor: public genetic::constraint<urban::frequency_set> {
		public:
		bool satisfied(urban::frequency_set& sol) const {
			return sol.get_highest_load() <= tnfsp_configs::max_load;
		}
		
		float transgression(urban::frequency_set& sol) const {
			float load = sol.get_highest_load();
			if (satisfied(sol)) {
				return 0;
			} else {
				return std::pow((load-tnfsp_configs::max_load)+2, 2);
			}
		}
	};

	/**
	 * I don't think we can do this right now because we don't have
	 * a way to convert everything to monetary value. But we have 
	 * proxys for this like frequencies, fleets, travel time, etc.
	*/
	// class budget: public genetic::constraint<urban::frequency_set> {
	// 	public:
	// 	bool satisfied(urban::frequency_set& sol) const {

	// 	}
		
	// 	float transgression(urban::frequency_set& sol) const {

	// 	}
	// };

	class frequency: public genetic::constraint<urban::frequency_set> {
		public:
		bool satisfied(urban::frequency_set& sol) const {
			int high = sol.get_highest_f();
			int low  = sol.get_lowest_f();
			bool max = high <= tnfsp_configs::max_frequency && 
			           high >= tnfsp_configs::min_frequency;
			bool min = low  <= tnfsp_configs::max_frequency && 
			           low  >= tnfsp_configs::min_frequency;
			return max && min;
		}
		
		float transgression(urban::frequency_set& sol) const {
			if (satisfied(sol)) {
				return 0;
			}
			
			int lon = sol.get_highest_f();
			int sho = sol.get_lowest_f();

			float total = 0;
			if (lon > tnfsp_configs::max_frequency) {
				total += std::pow((lon-tnfsp_configs::max_frequency)+2, 2);
			}
			if (lon < tnfsp_configs::min_frequency) {
				total += std::pow((tnfsp_configs::min_frequency-lon)+2, 2);
			}
			if (sho > tnfsp_configs::max_frequency) {
				total += std::pow((sho-tnfsp_configs::max_frequency)+2, 2);
			}
			if (sho < tnfsp_configs::min_frequency) {
				total += std::pow((tnfsp_configs::min_frequency-sho)+2, 2);
			}
			return total;
		}
	};

}

#endif