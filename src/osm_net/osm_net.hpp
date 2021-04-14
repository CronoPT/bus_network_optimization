#ifndef __OSM_NET_HPP__
#define __OSM_NET_HPP__

#include <nlohmann/json.hpp>
#include <graph.hpp>
#include <fstream>
#include "osm_net.hpp"
#include "osm_node.hpp"
#include "osm_edge.hpp"


namespace osm_net {

	class osm_network {
		//TODO implement this class which is supposed to be an encapsulation of a graph that makes it easier
		//to compute paths with specific modality needs
		// net::graph<osm_node, osm_edge> _some;
	};

	net::graph<osm_node, osm_edge> import_network();

} // namespace osm_net

#endif