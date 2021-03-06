#ifndef __TNFSP_CONFIGS_HPP__
#define __TNFSP_CONFIGS_HPP__

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
		static const float frequency_step;
		static const int operating_hours;
		static const std::string day_time;
	};

} // namespace transit_problem

#endif