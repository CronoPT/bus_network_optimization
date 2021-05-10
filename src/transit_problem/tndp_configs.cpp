#ifndef __TNDP_CONFIGS_CPP__
#define __TNDP_CONFIGS_CPP__

#include "tndp_configs.hpp"

namespace transit_problem {

	const int tndp_configs::max_route_length  = 55;
	const int tndp_configs::min_route_length  = 10;
	const int tndp_configs::max_number_routes = 400;
	const int tndp_configs::min_number_routes = 200;
	const float tndp_configs::max_transfers   = 1;

} // namespace transit_problem

#endif