#ifndef __NETWORK_USAGE_HPP__
#define __NETWORK_USAGE_HPP__

#include "trip.hpp"
#include <vector>
#include <unordered_map>

namespace urban {

	class network_usage {

		std::unordered_map<
			int, // origin i, as in (i, j)
			std::unordered_map<
				int, // origin j, as in (i, j)
				std::unordered_map<
					int, // destin i as in (i, j)
					std::unordered_map<
						int, // destin j as in (i, j)
						trip
					>
				>
			>
		> _usage;

		public:
		network_usage();

		void add_usage_from(
			std::pair<int, int> origin, 
			const std::vector<trip>& trips
		);
		const trip& get_usage_between(
			std::pair<int, int> origin, 
			std::pair<int, int> destin
		);

	};

}

#endif