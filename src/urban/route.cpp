#ifndef __ROUTE_CPP__
#define __ROUTE_CPP__

#include "route.hpp"

namespace urban {

	route::route(int route_id, std::vector<int> stop_sequence):
	 _route_id(route_id), _stop_sequence(stop_sequence) {
		/* Do Nothing */
	}
	route::route():
	 _route_id(-1), _stop_sequence() {
		/* Do Nothing */
	}

	int route::get_route_id() const {
		return _route_id;
	}

	const std::vector<int>& route::get_stop_sequence() const {
		return _stop_sequence;
	}

	int route::size() const { return _stop_sequence.size(); }

} // namespace urban

#endif