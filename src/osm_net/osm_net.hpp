#ifndef __OSM_NET_HPP__
#define __OSM_NET_HPP__

#include <nlohmann/json.hpp>
#include <graph.hpp>
#include "osm_node.hpp"
#include "osm_edge.hpp"


namespace osm_net {

	net::graph<osm_node, osm_edge> import_network();

} // namespace osm_net

#endif