#ifndef __BUS_NODE_CPP__
#define __BUS_NODE_CPP__

#include "bus_node.hpp"

namespace urban {

	bus_node::bus_node(int stop_id, double lon, double lat):
	 _stop_id(stop_id), _lon(lon), _lat(lat) {
		/* Do Nothing */
	}

	bus_node::bus_node():
	 _stop_id(-1), _lon(-1), _lat(-1) {
		/* Do Nothing */
	}

	int bus_node::get_stop_id() const {

	}

	double bus_node::lon() const {

	}

	double bus_node::lat() const {

	}

} // namespace urban

#endif