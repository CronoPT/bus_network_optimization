#ifndef __TNDP_CONFIGS_CPP__
#define __TNDP_CONFIGS_CPP__

#include "tndp_configs.hpp"

namespace transit_problem {

	const int tndp_configs::max_route_length  = 55;
	const int tndp_configs::min_route_length  = 10;
	const int tndp_configs::max_number_routes = 400;
	const int tndp_configs::min_number_routes = 200;
	const float tndp_configs::max_transfers   = 1;
	const std::string tndp_configs::og_routes = 
		"../data/json/clustered_routes_stop_sequence.json"; 
	const std::string tndp_configs::ge_routes = 
		"../data/json/route_pool.json";
	// const int tndp_configs::max_route_length  = 15;
	// const int tndp_configs::min_route_length  = 4;
	// const int tndp_configs::max_number_routes = 30;
	// const int tndp_configs::min_number_routes = 10;
	// const float tndp_configs::max_transfers   = 1;
	// const std::string tndp_configs::og_routes = 
	// 	"../data/json/generated_routes_stop_sequences.json"; 
	// const std::string tndp_configs::ge_routes = 
	// 	"../data/json/generated_route_pool.json";  

} // namespace transit_problem

#endif