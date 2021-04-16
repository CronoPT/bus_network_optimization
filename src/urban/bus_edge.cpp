#ifndef __BUS_EDGE_CPP__
#define __BUS_EDGE_CPP__

#include "bus_edge.hpp"

namespace urban {

	bus_edge::bus_edge(int origin, int destin, int route, float time):
	 _origin_id(origin), _destin_id(destin), 
	 _route_id(route), _time_taken(time) {
		/* Do Nothing */
	}

	bus_edge::bus_edge():
	 _origin_id(-1), _destin_id(-1),
	 _route_id(-1), _time_taken(-1) {
		/* Do Nothing */
	} 

	int bus_edge::get_origin_id() const {
		return _origin_id;
	}

	int bus_edge::get_destin_id() const {
		return _destin_id;
	}

	int bus_edge::get_route_id() const {
		return _route_id;
	}

	float bus_edge::get_time_taken() const {
		return _time_taken;
	}

} // namespace urban

#endif