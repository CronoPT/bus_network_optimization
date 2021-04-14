#ifndef __OSM_NODE_CPP__
#define __OSM_NODE_CPP__

#include "osm_node.hpp"

namespace osm_net {

	osm_node::osm_node(int id, double lon, 
	                   double lat, std::string type):
	 _osm_id(id), _lon(lon), _lat(lat), _type(type) {

	}

	osm_node::osm_node():
	 _osm_id(-1), _lon(-1), 
	 _lat(-1), _type("undefined") {

	}

	int osm_node::get_osm_id() const {
		return _osm_id;
	}

	double osm_node::get_lon() const {
		return _lon;
	}

	double osm_node::get_lat() const {
		return _lat;
	}

	const std::string& osm_node::get_type() const {
		return _type;
	}

} // namespace osm_net

#endif