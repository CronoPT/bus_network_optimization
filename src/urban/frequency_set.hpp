#ifndef __FREQUENCY_SET_HPP__
#define __FREQUENCY_SET_HPP__

#include <vector>
#include "bus_network.hpp"

namespace urban {

	class frequency_set {
		
		bus_network& _bus; // WARNING: carefull with _bus ownership  
		std::vector<float> _frequencies;
		int  _required_fleet;
		float _highest_f;
		float _lowest_f;
		float _waiting_time;
		float _highest_load;
		float _operator_costs;
		bool _evaluated;

		public:
		frequency_set(
			std::vector<float>& frequencies,
			bus_network& bus
		);

		int get_required_fleet();
		float get_highest_f() const;
		float get_lowest_f() const;
		float get_waiting_time();
		float get_highest_load();
		float get_operator_costs();

		void evaluate();

	};

} // namespace urban

#endif