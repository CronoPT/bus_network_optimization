#ifndef __ROUTE_POOL_HPP__
#define __ROUTE_POOL_HPP__

#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <route.hpp>
#include <string>
#include "tndp_configs.hpp"

namespace transit_problem {

	class route_pool {

		static route_pool* _instance;

		std::vector<urban::route> _routes;
		std::vector<urban::route> _mandatory_routes;
		
		route_pool();

		public:
		static route_pool* instance();
		int size() const;
		urban::route at(int i) const;
		const std::vector<urban::route>& 
			get_mandatory_routes() const;
		int get_number_mandatory() const; 

	};

}

#endif