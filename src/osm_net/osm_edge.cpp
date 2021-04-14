#ifndef __OSM_EDGE_CPP__
#define __OSM_EDGE_CPP__

#include "osm_edge.hpp"

namespace osm_net {

	osm_edge::osm_edge(int origin, int destin, float len, 
				int m_speed, coord_sequence geometry):
	 _origin_id(origin), _destin_id(destin), _length(len),
	 _max_speed(m_speed), _geometry(geometry) {

	}

	osm_edge::osm_edge(int origin, int destin, float len, 
				int m_speed): 
	 _origin_id(origin), _destin_id(destin), _length(len),
	 _max_speed(m_speed), _geometry() {

	}

	osm_edge::osm_edge(int origin, int destin, float len): 
	 _origin_id(origin), _destin_id(destin), _length(len),
	 _max_speed(-1), _geometry() {

	}

	osm_edge::osm_edge():
	 _origin_id(-1), _destin_id(-1), _length(-1),
	 _max_speed(-1), _geometry() {

	}

	int osm_edge::get_origin_id() const {
		return _origin_id;
	}

	int osm_edge::get_destin_id() const {
		return _destin_id;
	}

	float osm_edge::get_length()  const {
		return _length;
	}

	int osm_edge::get_max_speed() const {
		return _max_speed;
	}

	const coord_sequence& osm_edge::get_geometry() const {
		return _geometry;
	}

	// const std::string& osm_edge::get_name() const {
	// 	return _name;
	// }

} // namespace osm_net

#endif