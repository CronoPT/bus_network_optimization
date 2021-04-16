#ifndef __WALKING_EDGE_CPP__
#define __WALKING_EDGE_CPP__

#include "walking_edge.hpp"

namespace urban {

	walking_edge::walking_edge(
		int origin_id,
		int destin_id,
		float time_taken,
		std::string origin_type,
		std::string destin_type
	):
	 _origin_id(origin_id), _destin_id(destin_id),
	 _time_taken(time_taken), _origin_type(origin_type),
	 _destin_type(destin_type) {
		/* Do Nothing */
	}

	walking_edge::walking_edge():
	 _origin_id(-1), _destin_id(-1),
	 _time_taken(-1), _origin_type("undefined"),
	 _destin_type("undefined") {
		/* Do Nothing */
	}

	int walking_edge::get_origin_id()  const {
		return _origin_id;
	}

	int walking_edge::get_destin_id()  const {
		return _destin_id;
	}

	int walking_edge::get_time_taken() const {
		return _time_taken;
	}

	const std::string& walking_edge::get_origin_type() const {
		return _origin_type;
	}

	const std::string& walking_edge::get_destin_type() const {
		return _destin_type;
	}

} // namespace urban

#endif