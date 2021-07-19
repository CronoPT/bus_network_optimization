#ifndef __TRIP_CPP__
#define __TRIP_CPP__

#include "trip.hpp"

namespace urban {

	trip::trip(
		std::vector<stage>& stages,
		float passengers,
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
	 _destin_square(destin_square),
	 _time(0) {
		
		for (auto& s: stages) {
			if (s.get_mode() != WALKING_STAGE) {
				_transfers += 1;
			}

			_time += s.get_time();
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
	 _destin_square({-1, -1}),
	 _time(-1) {
		/* Do Nothing */
	}

	const std::vector<stage>& trip::get_stages() const {
		return _stages;
	}

	float trip::get_passengers() const {
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

	float trip::get_time() const {
		return _time;
	}

	float trip::get_walking_time() const {
		float total;
		for (auto& stage: _stages) {
			if (stage.get_mode() == WALKING_STAGE) {
				total += stage.get_time();
			}
		}
		return total;
	}

	int trip::get_number_modes() const {
		bool has_walk  = false;
		bool has_metro = false;
		bool has_bus   = false;
		for (auto& stage: _stages) {
			if (stage.get_mode() == WALKING_STAGE) {
				has_walk = true;
			} else if (stage.get_mode() == METRO_STAGE) {
				has_metro = true;
			} else if (stage.get_mode() == BUS_STAGE) {
				has_bus = true;
			}
		}

		int modes = 0;
		if (has_walk)  modes += 1;
		if (has_metro) modes += 1;
		if (has_bus)   modes += 1;
		return modes;
	}

	bool trip::uses_walk() const {
		for (auto& stage: _stages) {
			if (stage.get_mode() == WALKING_STAGE) {
				return true;
			}
		}
		return false;
	}

	bool trip::uses_metro() const {
		for (auto& stage: _stages) {
			if (stage.get_mode() == METRO_STAGE) {
				return true;
			}
		}
		return false;
	}

	bool trip::uses_bus() const {
		for (auto& stage: _stages) {
			if (stage.get_mode() == BUS_STAGE) {
				return true;
			}
		}
		return false;
	}

} // namespace urban

#endif