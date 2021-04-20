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
		return _stop_id;
	}

	double bus_node::get_lon() const {
		return _lon;
	}

	double bus_node::get_lat() const {
		return _lat;
	}

} // namespace urban

#endif