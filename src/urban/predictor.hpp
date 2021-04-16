#ifndef __PREDICTOR_HPP__
#define __PREDICTOR_HPP__

#include <vector>
#include "bus_network.hpp"

namespace urban {

	class predictor {
		public:
		static std::pair<std::vector<int>, float> predict_path(
			road_graph road,
			bus_network bus
		);
	};

} // namespace urban

#endif