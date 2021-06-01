#ifndef __FREQUENCY_SET_CPP__
#define __FREQUENCY_SET_CPP__

#include "frequency_set.hpp"

namespace urban {

	frequency_set::frequency_set(
		std::vector<float>& frequencies,
		bus_network& bus,
		float operation_hours
	):
	 _bus(bus),
	 _bus_usage(),
	 _operation_hours(operation_hours),
	 _frequencies(frequencies),
	 _required_fleet(0),
	 _highest_f(std::numeric_limits<float>::lowest()),
	 _lowest_f(std::numeric_limits<float>::max()),
	 _waiting_time(0),
	 _highest_load(0),
	 _operator_costs(0),
	 _evaluated(false),
	 _bus_evaluated(false),
	 _route_indexes() {
		
		if (frequencies.size() != bus.get_routes().size()) {
			throw "There must be a frequency per route in the bus network";
		}

		// checking the highest and lowest frequencies
		for (auto f: frequencies) {
			if (f > _highest_f) {
				_highest_f = f;
			}

			if (f < _lowest_f) {
				_lowest_f = f;
			}
		}


		for (int i=0; i<bus.get_routes().size(); i++) {
			auto& r = bus.get_routes().at(i);
			_route_indexes[r.get_route_id()] = i;
		} 

	}

	void frequency_set::evaluate() {
		if (!_bus_evaluated) {
			_bus_usage = grid::instance()->predict_all_od_pairs(_bus);
			_bus_evaluated = true;
		}

		_required_fleet = compute_required_fleet();
		_waiting_time   = compute_waiting_time();
		_highest_load   = compute_highest_load();
		_operator_costs = compute_operator_costs();

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

	/**
	 * Computes the number of buses that need to be 
	 * available for the network to be effectivelly
	 * served with the current frequency spread.
	 * Essencially, we need to compute, for each route,
	 * how many buses must be in ciculation simulateously
	 * for that route to work with the proposed frequency.
	*/
	int frequency_set::compute_required_fleet() {
		float total = 0;
		for (int i=0; i<_frequencies.size(); i++) {
			// the time it takes to traverse the whole route (in hours)
			auto total_time = _bus.get_routes().at(i).get_total_time()/60;

			// the frequency of buses in the route (in buses/hour)
			auto frequency  = _frequencies.at(i);

			// multiplying both gives us the ammount of buses needed 
			// to serve the route
			total += frequency * total_time;
		}

		return std::ceil(total);
	}
	
	/**
	 * Computes the average waiting time (in hours) 
	 * for all OD pairs with the current frequency
	 * spread. The average time a person takes when
	 * catching a bus, should be half the time between
	 * buses in the specific route (1/2f).
	*/
	float frequency_set::compute_waiting_time() {
		float total = 0;

		auto& pairs = odx_matrix::instance()->get_all_pairs();
		int total_passengers  = 0;
		
		for (auto& od_pair: pairs) {
			auto origin = od_pair.first;
			auto destin = od_pair.second;

			auto& use = _bus_usage.get_usage_between(origin, destin);
			int passengers = odx_matrix::instance()->get_total(origin, destin);

			if (origin.first  == destin.first && 
			    origin.second == destin.second) { 
				continue; 
			}

			if (use.get_stages().size() == 0) {
				continue;
			}

			total_passengers += passengers;
			for (auto& s: use.get_stages()) {
				if (s.get_mode() != WALKING_STAGE) {
					float frequency;
					if (s.get_mode() == METRO_STAGE) {
						frequency = configs::metro_frequency;
					} else if (s.get_mode() == BUS_STAGE) {
						int route = s.get_itinerary();
						int route_index = _route_indexes[route];
						frequency = _frequencies[route_index];
					}
					float average_waiting_time = 1 / (2*frequency);
					total += average_waiting_time * passengers;
				}
			}
		}

		//average waiting time amongst all passengers
		total /= total_passengers;

		return total;
	}

	/**
	 * Computes the average the percentual load
	 * (how packed it is) of the route that has
	 * a higher passenger density (relative to
	 * the bus frequency).
	 * We first need to traverse all the trips 
	 * in the network usage object because 
	 * trips can take advantage of a multitude
	 * of routes at the same time.
	*/
	float frequency_set::compute_highest_load() {
		auto counts = std::unordered_map<
			int, 
			std::unordered_map<
				int, 
				std::unordered_map<int, int>
			>
		>();

		// Initialize the weird structure above
		for (auto& r: _bus.get_routes()) {
			int route_id = r.get_route_id();
			int origin   = -1;

			for (int destin: r.get_stop_sequence()) {
				if (origin != -1) {
					counts[route_id][origin][destin] = 0;
				}
				origin = destin;
			} 
		}

		// Filling in the structure
		// We have to see, how many passengers go from
		// stop x to stop y (adjancent stops in
		// the stop connection graph) in the context of
		// each route.
		auto& pairs = odx_matrix::instance()->get_all_pairs();		
		for (auto& od_pair: pairs) {
			auto origin = od_pair.first;
			auto destin = od_pair.second;

			auto& use = _bus_usage.get_usage_between(origin, destin);
			int passengers = odx_matrix::instance()->get_total(origin, destin);

			if (origin.first  == destin.first && 
			    origin.second == destin.second) { 
				continue; 
			}

			if (use.get_stages().size() == 0) {
				continue;
			}

			for (auto& s: use.get_stages()) {
				if (s.get_mode() == BUS_STAGE) {
					int route_id = s.get_itinerary();
					int origin = -1;
					for (int destin: s.get_sequence()) {
						if (origin != -1) {
							counts[route_id][origin][destin] += passengers;
						}
						origin = destin;
					}
				}
			}
		}

		// Now we know the passenger quantity, so we need
		// to know how many buses will be serving the routes
		// so we know how packed they will be (assuming
		// a uniform distribution of passenger arrival).
		float highest_load = std::numeric_limits<float>::lowest();
		for (auto& pair: counts) {
			int route_id = pair.first;
			int route_index = _route_indexes[route_id];

			for (auto& origin_pair: pair.second) {
				auto origin = origin_pair.first;
				for (auto& destin_pair: origin_pair.second) {
					auto destin = destin_pair.first;
					auto passengers = destin_pair.second;

					auto frequency = _frequencies.at(route_index);
					auto total_traversals = frequency * _operation_hours;
					auto load_factor = passengers / (configs::bus_capacity*total_traversals);  

					if (load_factor > highest_load) {
						highest_load = load_factor;
					}
				} 
			}
		}	

		return highest_load;
	}

	/**
	 * Computes the total operator cost. Here
	 * the total Km's that the fleet will have
	 * to cover in the opertion hours is a proxy
	 * for those costs.
	 * The higher the frequency on a route, the more
	 * times it will be traversed in the working time.
	*/
	float frequency_set::compute_operator_costs() {
		float total_km = 0;
		for (auto& r: _bus.get_routes()) {
			int route_id = r.get_route_id();
			int route_index = _route_indexes[route_id];
			auto frequency = _frequencies.at(route_index);
			auto total_traversals = frequency * _operation_hours;
			total_km += (r.get_route_length() * total_traversals) /	1000;
		}
		return total_km;
	}

} // namespace urban

#endif