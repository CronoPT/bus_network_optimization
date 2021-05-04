#ifndef __SINGLE_PATH_REPORT_CPP__
#define __SINGLE_PATH_REPORT_CPP__

#include "single_path_report.hpp"

namespace urban {

	single_path_report::single_path_report(
		float cost, 
		std::pair<int, int> destin,
		std::vector<int> stopations,
		std::vector<int> itineraries,
		std::vector<float> costs_until
	): 
	 _cost(cost), _destin(destin),
	 _stopations(stopations),
	 _itineraries(itineraries),
	 _costs_until(costs_until) {
		/* Do Nothing */
	}
	single_path_report::single_path_report(): 
	 _cost(-1), _destin({-1, -1}),
	 _stopations(),
	 _itineraries(),
	 _costs_until() {
		/* Do Nothing */
	}

	float single_path_report::get_cost() const {
		return _cost;
	}

	std::pair<int, int> single_path_report::get_destin() const {
		return _destin;
	}

	const std::vector<int>& single_path_report::get_stopations() const {
		return _stopations;
	}

	const std::vector<int>& single_path_report::get_itineraries() const {
		return _itineraries;
	}

	const std::vector<float>& single_path_report::get_costs_until() const {
		return _costs_until;
	}

} // namespace urban

#endif