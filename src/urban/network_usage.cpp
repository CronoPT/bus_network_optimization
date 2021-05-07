#ifndef __NETWORK_USAGE_CPP__
#define __NETWORK_USAGE_CPP__

#include "network_usage.hpp"

namespace urban {

	network_usage::network_usage(): 
	 _usage() { /* Do Nothing */ }

	void network_usage::add_usage_from(
		std::pair<int, int> origin, 
		const std::vector<trip>& trips
	) {
		int origin_i = origin.first;
		int origin_j = origin.second;
		for (auto& single_trip: trips) {
			auto destin_sq = single_trip.get_destin_square();
			int  destin_i  = destin_sq.first;
			int  destin_j  = destin_sq.second;
			_usage[origin_i][origin_j][destin_i][destin_j] = single_trip;
		}
	}

	const trip& network_usage::get_usage_between(
		std::pair<int, int> origin, 
		std::pair<int, int> destin
	) {
		int origin_i = origin.first;
		int origin_j = origin.second;
		int destin_i = destin.first;
		int destin_j = destin.second;
		return _usage[origin_i][origin_j][destin_i][destin_j];
	}

}

#endif