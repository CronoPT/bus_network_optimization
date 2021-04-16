#ifndef __METRO_NODE_CPP__
#define __METRO_NODE_CPP__

#include "metro_node.hpp"

namespace urban {

	metro_node::metro_node(int station_id, double lon, double lat):
	 _station_id(station_id), _lon(lon), _lat(lat) {
		/* Do Nothing */
	}

	metro_node::metro_node():
	 _station_id(-1), _lon(-1), _lat(-1) {
		/* Do Nothing */
	}

	int metro_node::get_station_id() const {
		return _station_id;
	}

	double metro_node::get_lon() const {
		return _lon;
	}

	double metro_node::get_lat() const {
		return _lat;
	}

} // namespace urban

#endif