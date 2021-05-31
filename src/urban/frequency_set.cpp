#ifndef __FREQUENCY_SET_CPP__
#define __FREQUENCY_SET_CPP__

#include "frequency_set.hpp"

namespace urban {

	frequency_set::frequency_set(
		std::vector<float>& frequencies,
		bus_network& bus
	):
	 _bus(bus),
	 _frequencies(frequencies),
	 _required_fleet(0),
	 _highest_f(std::numeric_limits<float>::lowest()),
	 _lowest_f(std::numeric_limits<float>::max()),
	 _waiting_time(0),
	 _highest_load(0),
	 _operator_costs(0),
	 _evaluated(false) {
		
		// checking the highest and lowest frequencies
		for (auto f: frequencies) {
			if (f > _highest_f) {
				_highest_f = f;
			}

			if (f < _lowest_f) {
				_lowest_f = f;
			}
		}

	}

	void frequency_set::evaluate() {

		//TODO: implement evaluation things

		_evaluated = true;
	}

	int frequency_set::get_required_fleet() {
		if (!_evaluated) { evaluate(); }
		return _required_fleet;
	}

	float frequency_set::get_highest_f() const {
		return _highest_f;
	}

	float frequency_set::get_lowest_f() const {
		return _lowest_f;
	}

	float frequency_set::get_waiting_time() {
		if (!_evaluated) { evaluate(); }
		return _waiting_time;
	}

	float frequency_set::get_highest_load() {
		if (!_evaluated) { evaluate(); }
		return _waiting_time;
	}

	float frequency_set::get_operator_costs() {
		if (!_evaluated) { evaluate(); }
		return _operator_costs;
	}

} // namespace urban

#endif