#ifndef __WALKING_NODE_CPP__
#define __WALKING_NODE_CPP__

#include "walking_node.hpp"

namespace urban {

	walking_node::walking_node(
		int id, std::string type, 
		double lon, double lat):
	 _id(id), _type(type),
	 _lon(lon), _lat(lat) {
		/* Do Nothing */
	}

	walking_node::walking_node():
	 _id(-1), _type("undefined"),
	 _lon(-1), _lat(-1) {
		/* Do Nothing */
	}

	int walking_node::get_id() const {
		return _id;
	}

	const std::string& walking_node::get_type() const {
		return _type;
	}

	double walking_node::get_lon() const {
		return _lon;
	}

	double walking_node::get_lat() const {
		return _lat;
	}

} // namespace urban

#endif