#ifndef __METRO_NETWORK_HPP__
#define __METRO_NETWORK_HPP__

#include "metro_node.hpp"
#include "metro_edge.hpp"
#include <graph.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

namespace urban {

	class metro_network: public net::graph<metro_node, metro_edge> {

		public:
		metro_network();

	};

} // namespace urban

#endif