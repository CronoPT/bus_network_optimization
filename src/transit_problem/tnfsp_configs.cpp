#ifndef __TNDP_CONFIGS_CPP__
#define __TNDP_CONFIGS_CPP__

#include "tnfsp_configs.hpp"

namespace transit_problem {

	const int tnfsp_configs::available_fleet = 500;
	const int tnfsp_configs::bus_capacity = 60;
	const float tnfsp_configs::max_frequency = 25.0;
	const float tnfsp_configs::min_frequency = 1.0;
	const float tnfsp_configs::max_load = 1.0;

	/**
	 * The granularity of the frequency values
	*/
	const float tnfsp_configs::frequency_step = 0.25;

} // namespace transit_problem

#endif