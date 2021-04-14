#ifndef __BUS_NETWORK_HPP__
#define __BUS_NETWORK_HPP__

#include <osm_node.hpp>
#include <osm_edge.hpp>
#include <graph.hpp>
#include <vector>

namespace urban {

	typedef net::graph<osm_net::osm_node, osm_net::osm_edge> road_graph;

	class bus_network {
		road_graph _graph;

		public:
			bus_network(std::vector<std::vector<int>> stop_sequences,
				        road_graph road_network);
	};

} // namespace urban

#endif