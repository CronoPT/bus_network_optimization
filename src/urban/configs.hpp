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

	};

} // namspace urban

#endif