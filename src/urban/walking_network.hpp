#ifndef __WALKING_NETWORK_HPP__
#define __WALKING_NETWORK_HPP__

#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <graph.hpp>
#include "walking_edge.hpp"
#include "walking_node.hpp"

namespace urban {

	class walking_network: public net::graph<walking_node, walking_edge> {
		
		public:
		walking_network();

	};

} // namespace urban

#endif