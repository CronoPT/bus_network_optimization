#ifndef __METRO_EDGE_CPP__
#define __METRO_EDGE_CPP__

#include "metro_edge.hpp"

namespace urban {

	metro_edge::metro_edge(
		int origin, 
		int destin, 
		std::string color, 
		float time_taken
	): 
	 _origin_id(origin), _destin_id(destin),
	 _line_color(color), _time_taken(time_taken) {
		/* Do Nothing */
	}

	metro_edge::metro_edge():
	 _origin_id(-1), _destin_id(-1),
	 _line_color("undefined"), _time_taken(-1) {
		/* Do Nothing */
	}

	int metro_edge::get_origin_id() const {
		return _origin_id;
	}

	int metro_edge::get_destin_id() const {
		return _destin_id;
	}

	const std::string& metro_edge::get_line_color() const {
		return _line_color;
	}

	float metro_edge::get_time_taken() const {
		return _time_taken;
	}

} // namespace urban

#endif