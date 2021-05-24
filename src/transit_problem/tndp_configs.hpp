#ifndef __TNDP_CONFIGS_HPP__
#define __TNDP_CONFIGS_HPP__

#include <string>
#include <vector>

namespace transit_problem {

	class tndp_configs {
		public:
		static const int max_route_length;
		static const int min_route_length;
		static const int max_number_routes;
		static const int min_number_routes;
		static const float max_transfers;
		static const std::string  ge_routes;
		static const std::string  og_routes;
		static const std::vector<std::string> tram_routes;
	};

} // namespace transit_problem

#endif