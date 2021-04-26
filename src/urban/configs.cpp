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

	const int configs::divisions = 30;

	// const std::string configs::stop_locations = 
	// 	"../data/json/generated_bus_stops.json";

	// const std::string configs::stop_sequences =
	// 	"../data/json/generated_routes_stop_sequences.json";

	// const std::string configs::metro_network = 
	// 	"../data/json/generated_metro_network.json";

	// const std::string configs::walking_network = 
	// 	"../data/json/generated_walks.json";

	// const int configs::divisions = 30;

} // namespace urban

#endif