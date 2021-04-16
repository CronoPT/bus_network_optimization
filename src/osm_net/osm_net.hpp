#ifndef __OSM_NET_HPP__
#define __OSM_NET_HPP__

#include <nlohmann/json.hpp>
#include <graph.hpp>
#include <fstream>
#include "osm_net.hpp"
#include "osm_node.hpp"
#include "osm_edge.hpp"


namespace osm_net {

	class osm_net: public net::graph<osm_node, osm_edge> {

		public:
		osm_net();

	};

} // namespace osm_net

#endif