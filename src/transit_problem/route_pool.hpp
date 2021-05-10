#ifndef __ROUTE_POOL_HPP__
#define __ROUTE_POOL_HPP__

#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <route.hpp>
#include <string>

namespace transit_problem {

	class route_pool {
		std::vector<urban::route> _routes;
		static const std::string  ge_routes;
		static const std::string  og_routes;
		
		public:
		route_pool();
		int size() const;
		urban::route at(int i) const;

	};

}

#endif