#ifndef __OSM_NET_HPP__
#define __OSM_NET_HPP__

#include <nlohmann/json.hpp>
#include <graph.hpp>
#include <fstream>
#include "osm_node.hpp"
#include "osm_edge.hpp"
#include "osm_net_configs.hpp"


namespace osm_net {

	class osm_net: public net::graph<osm_node, osm_edge> {
		
		static osm_net* _instance;
		osm_net();

		public:
		static osm_net* instance();

	};

} // namespace osm_net

#endif