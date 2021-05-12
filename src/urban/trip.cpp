#ifndef __TRIP_CPP__
#define __TRIP_CPP__

#include "trip.hpp"

namespace urban {

	trip::trip(
		std::vector<stage>& stages,
		int passengers,
		int origin,
		int destin,
		std::pair<int, int> origin_square,
		std::pair<int, int> destin_square
	):
	 _stages(stages),
	 _passengers(passengers),
	 _origin(origin),
	 _destin(destin),
	 _transfers(0),
	 _origin_square(origin_square),
	 _destin_square(destin_square) {
		
		for (auto& s: stages) {
			if (s.get_mode() != WALKING_STAGE) {
				_transfers += 1;
			}
		}

		// Discounting the first boarding which 
		// is not really transfer.
		if (_transfers > 0) {
			_transfers -= 1;
		}

	}

	trip::trip():
	 _stages(),
	 _passengers(-1),
	 _origin(-1),
	 _destin(-1),
	 _transfers(-1),
	 _origin_square({-1, -1}),
	 _destin_square({-1, -1}) {
		/* Do Nothing */
	}

	const std::vector<stage>& trip::get_stages() const {
		return _stages;
	}

	int trip::get_passengers() const {
		return _passengers;
	}

	int trip::get_origin() const {
		return _origin;
	}

	int trip::get_destin() const {
		return _destin;
	}

	int trip::get_transfers() const {
		return _transfers;
	}

	std::pair<int, int> trip::get_origin_square() const {
		return _origin_square;
	}

	std::pair<int, int> trip::get_destin_square() const {
		return _destin_square;
	}

} // namespace urban

#endif