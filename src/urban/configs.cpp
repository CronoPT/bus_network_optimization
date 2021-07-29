#ifndef __CONFIGS_CPP__
#define __CONFIGS_CPP__

#include "configs.hpp"

namespace urban {

	const std::string configs::stop_locations = 
		"../data/json/clustered_stop_locations.json";

	const std::string configs::stop_sequences =
		"../data/json/clustered_routes_stop_sequence.json";

	const std::string configs::metro_network = 
		"../data/json/metro_networknx.json";

	const std::string configs::walking_network = 
		"../data/json/walks.json";

	const int configs::divisions = 25;

	const std::string configs::odx_matrix_data = 
		"../data/json/odx_matrix.json";
	
	const float configs::metro_frequency = 12; // trains per hour

	const int configs::bus_capacity = 60;

	const bool configs::verbose = false;

	// const std::string configs::stop_locations = 
	// 	"../data/json/generated_bus_stops.json";

	// const std::string configs::stop_sequences =
	// 	"../data/json/generated_routes_stop_sequences.json";

	// const std::string configs::metro_network = 
	// 	"../data/json/generated_metro_network.json";

	// const std::string configs::walking_network = 
	// 	"../data/json/generated_walks.json";

	// const int configs::divisions = 6;

	// const std::string configs::odx_matrix_data = 
	// 	"../data/json/generated_odx_matrix.json";

} // namespace urban

#endif