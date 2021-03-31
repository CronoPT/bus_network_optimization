#include "osm_net.hpp"

namespace osm_net {

	net::graph<osm_node, osm_edge> import_network() {
		return net::graph<osm_node, osm_edge>();
	}

} // namespace osm_net