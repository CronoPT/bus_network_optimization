#ifndef __BUS_NETWORK_HPP__
#define __BUS_NETWORK_HPP__

#include "bus_node.hpp"
#include "bus_edge.hpp"
#include "route.hpp"
#include <graph.hpp>
#include <vector>
#include <osm_net.hpp>
#include <fstream>
#include <string>

namespace urban {

	class bus_network: public net::graph<bus_node, bus_edge> {
		
		public:
		bus_network(
			std::vector<route> routes,
			osm_net::osm_net road_network
		);

	};

} // namespace urban

#endif