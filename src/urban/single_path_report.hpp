#ifndef __SINGLE_PATH_REPORT_HPP__
#define __SINGLE_PATH_REPORT_HPP__

#include <vector>

namespace urban {

	class single_path_report {
		float _cost;
		std::pair<int, int> _destin;
		std::vector<int> _stopations;
		std::vector<int> _itineraries;
		std::vector<float> _costs_until;

		public:
		single_path_report(
			float cost, 
			std::pair<int, int> destin,
			std::vector<int> stopations,
			std::vector<int> itineraries,
			std::vector<float> costs_until
		);
		single_path_report();

		float get_cost() const;
		std::pair<int, int> get_destin() const;
		const std::vector<int>& get_stopations() const;
		const std::vector<int>& get_itineraries() const;
		const std::vector<float>& get_costs_until() const;

	};

} // namespace urban

#endif