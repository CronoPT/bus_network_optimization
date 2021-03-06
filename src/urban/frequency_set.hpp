#ifndef __FREQUENCY_SET_HPP__
#define __FREQUENCY_SET_HPP__

#include <vector>
#include <cmath>
#include <unordered_map>
#include "bus_network.hpp"
#include "network_usage.hpp"
#include "odx_matrix.hpp"

namespace urban {

	class frequency_set {
		
		bus_network* _bus;
		// network_usage _bus_usage;
		float _operation_hours;  
		std::string _day_time;
		std::vector<float> _frequencies;
		int  _required_fleet;
		float _highest_f;
		float _lowest_f;
		float _waiting_time;
		float _highest_load;
		float _operator_costs;
		bool _evaluated;
		bool _bus_evaluated;

		std::unordered_map<int, int> _route_indexes;
		std::unordered_map<int, std::unordered_map<
		                   int, std::unordered_map<
		                   int, std::unordered_map<
		                   int, float>>>> _waiting_time_discriminated;

		public:
		frequency_set(
			std::vector<float>& frequencies,
			bus_network* bus,
			float operation_hours,
			std::string day_time
		);
		frequency_set();

		int get_required_fleet();
		float get_highest_f() const;
		float get_lowest_f() const;
		float get_waiting_time();
		float get_highest_load();
		float get_operator_costs();
		const std::vector<float>& get_frequencies() const;
		void set_frequency_at(int pos, float new_f); 

		void evaluate();

		int compute_required_fleet();
		float compute_waiting_time();
		float compute_highest_load();
		float compute_operator_costs();


		const std::unordered_map<int, std::unordered_map<
		                         int, std::unordered_map<
								 int, std::unordered_map<
								 int, float>>>>& get_waiting_time_discriminated() const;

		friend std::ostream& operator<<(std::ostream& os, const frequency_set& a); 

	};

} // namespace urban

#endif