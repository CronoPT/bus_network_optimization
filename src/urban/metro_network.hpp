#ifndef __METRO_NETWORK_HPP__
#define __METRO_NETWORK_HPP__

#include "metro_node.hpp"
#include "metro_edge.hpp"
#include <graph.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "configs.hpp"

namespace urban {

	class metro_network: public net::graph<metro_node, metro_edge> {
		
		static metro_network* _instance;
		metro_network();

		public:
		static metro_network* instance();

	};

} // namespace urban

#endif