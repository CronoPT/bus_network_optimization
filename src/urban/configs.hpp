#ifndef __CONFIGS_HPP__
#define __CONFIGS_HPP__

#include <string>

namespace urban {

	class configs {
		public:
		static const std::string stop_locations;
		static const std::string stop_sequences;
		static const std::string metro_network;
		static const std::string walking_network;
		static const int divisions;
		static const std::string odx_matrix_data;
		static const float metro_frequency;
		static const int bus_capacity;
	};

} // namspace urban

#endif