#ifndef __TNDP_CONFIGS_HPP__
#define __TNDP_CONFIGS_HPP__

#include <string>
#include <vector>

namespace transit_problem {

	class tnfsp_configs {
		public:
		static const int available_fleet;
		static const int bus_capacity;
		static const float max_frequency;
		static const float min_frequency;
		static const float max_load;
	};

} // namespace transit_problem

#endif